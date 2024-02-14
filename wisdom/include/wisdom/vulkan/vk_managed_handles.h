#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_handle_traits.h>
#endif

WIS_EXPORT namespace wis
{
    template<typename HandleType>
    using parent_of_t = typename handle_traits<HandleType>::deleter_parent;

    template<typename HandleType>
    using pool_of_t = typename handle_traits<HandleType>::deleter_pool;

    template<typename HandleType>
    constexpr inline bool has_parent_v = !std::is_same_v<parent_of_t<HandleType>, empty_type>;

    template<typename HandleType>
    constexpr inline bool has_pool_v = !std::is_same_v<pool_of_t<HandleType>, empty_type>;

    template<typename HandleType>
    constexpr inline bool has_header_v = !std::is_same_v<typename handle_traits<HandleType>::deleter_pfn, empty_type>;

    template<typename HandleType>
    class single_deleter
    {
        using deleter_pfn = typename handle_traits<HandleType>::deleter_pfn;

    public:
        single_deleter(deleter_pfn pfn = handle_traits<HandleType>::default_deleter(), VkAllocationCallbacks* pallocator = nullptr) noexcept
            : m_pfn(pfn), m_pallocator(pallocator)
        {
        }

    public:
        void operator()(parent_of_t<HandleType> parent, HandleType handle) const noexcept
                requires(has_parent_v<HandleType>)
        {
            m_pfn(parent, handle, m_pallocator);
        }

        void operator()(HandleType handle) const noexcept
                requires(!has_parent_v<HandleType>)
        {
            m_pfn(handle, m_pallocator);
        }

    public:
        deleter_pfn m_pfn;
        VkAllocationCallbacks* m_pallocator;
    };

    template<typename HandleType>
    class pool_deleter
    {
        using deleter_pfn = typename handle_traits<HandleType>::deleter_pfn;

    public:
        pool_deleter(deleter_pfn pfn = handle_traits<HandleType>::default_deleter()) noexcept
            : m_pfn(pfn) { }

    public:
        void operator()(parent_of_t<HandleType> parent, pool_of_t<HandleType> pool, uint32_t num_handles, HandleType* handles) const noexcept
        {
            m_pfn(parent, pool, num_handles, handles);
        }

    public:
        deleter_pfn m_pfn;
    };

    template<typename HandleType>
    using deleter_of_t = std::conditional_t<std::is_same_v<pool_of_t<HandleType>, empty_type>,
                                            single_deleter<HandleType>, pool_deleter<HandleType>>;

    //=====================================================================================================================

    template<typename HandleType>
    class shared_handle;

    template<typename HandleType>
    struct managed_header {
        deleter_of_t<HandleType> deleter;
    };

    template<typename HandleType>
    requires has_parent_v<HandleType>
    struct managed_header<HandleType> {
        shared_handle<parent_of_t<HandleType>> parent;
        deleter_of_t<HandleType> deleter;
    };

    template<typename HandleType>
    requires has_parent_v<HandleType> && has_pool_v<HandleType>
    struct managed_header<HandleType> {
        shared_handle<parent_of_t<HandleType>> parent;
        shared_handle<pool_of_t<HandleType>> pool;
        deleter_of_t<HandleType> deleter;
    };

    //=====================================================================================================================

    class control_block_base
    {
    public:
        control_block_base() = default;
        control_block_base(const control_block_base&) = delete;
        control_block_base& operator=(const control_block_base&) = delete;

    public:
        size_t add_ref() noexcept
        {
            // Relaxed memory order is sufficient since this does not impose any ordering on other operations
            return m_ref_cnt.fetch_add(1, std::memory_order_relaxed);
        }

        size_t release() noexcept
        {
            // A release memory order to ensure that all releases are ordered
            return m_ref_cnt.fetch_sub(1, std::memory_order_release);
        }

    public:
        std::atomic_size_t m_ref_cnt{ 1 };
    };

    template<typename HeaderType>
    class control_block : public control_block_base
    {
    public:
        template<typename... Args>
        control_block(Args&&... control_args)
            : m_header{ std::forward<Args>(control_args)... }
        {
        }

    public:
        HeaderType m_header{};
    };

    template<>
    class control_block<empty_type> : public control_block_base
    {
    };

    //=====================================================================================================================

    template<typename HandleType, typename HeaderType = managed_header<HandleType>, typename ForwardType = shared_handle<HandleType>>
    class shared_handle_base
    {
    public:
        shared_handle_base() = default;

        template<typename... Args>
        shared_handle_base(HandleType handle, Args&&... control_args) noexcept
            : m_control(reinterpret_cast<control_block<HeaderType>*>(std::malloc(sizeof(control_block<HeaderType>))))
            , m_handle(handle)
        {
            // placement new for noexcept construction
            new (m_control) control_block<HeaderType>(std::forward<Args>(control_args)...);
        }

        shared_handle_base(const shared_handle_base& o) noexcept
        {
            o.add_ref();
            m_handle = o.m_handle;
            m_control = o.m_control;
        }

        shared_handle_base(shared_handle_base&& o) noexcept
            : m_control(o.m_control), m_handle(o.m_handle)
        {
            o.m_handle = nullptr;
            o.m_control = nullptr;
        }

        shared_handle_base& operator=(const shared_handle_base& o) noexcept
        {
            shared_handle_base(o).swap(*this);
            return *this;
        }

        shared_handle_base& operator=(shared_handle_base&& o) noexcept
        {
            shared_handle_base(std::move(o)).swap(*this);
            return *this;
        }

        ~shared_handle_base() noexcept
        {
            // only this function owns the last reference to the control block
            // the same principle is used in the default deleter of std::shared_ptr
            if (m_control && (m_control->release() == 1)) {
                // noop in x86, but does thread synchronization in ARM
                // it is required to ensure that last thread is getting to destroy the control block
                // by ordering all atomic operations before this fence
                std::atomic_thread_fence(std::memory_order_acquire);
                static_cast<ForwardType*>(this)->internal_destroy();
                m_control->~control_block<HeaderType>();
                free(m_control);
            }
        }

    public:
        HandleType get() const noexcept
        {
            return m_handle;
        }

        template<typename... Args>
        HandleType* put_unsafe(Args&&... control_args) noexcept
        {
            if (m_control)
                reset();
            m_control = reinterpret_cast<control_block<HeaderType>*>(std::malloc(sizeof(control_block<HeaderType>)));
            new (m_control) control_block<HeaderType>(std::forward<Args>(control_args)...);
            return &m_handle;
        }

        HandleType operator*() const noexcept
        {
            return m_handle;
        }

        explicit operator bool() const noexcept
        {
            return m_handle != nullptr;
        }

        void reset() noexcept
        {
            shared_handle_base().swap(*this);
        }

        void swap(shared_handle_base& o) noexcept
        {
            std::swap(m_handle, o.m_handle);
            std::swap(m_control, o.m_control);
        }

        const HeaderType& header() const noexcept
        {
            return m_control->m_header;
        }
        const shared_handle<parent_of_t<HandleType>>& parent() const noexcept
                requires(has_parent_v<HandleType>)
        {
            return m_control->m_header.parent;
        }

    protected:
        void add_ref() const noexcept
        {
            if (m_control)
                m_control->add_ref();
        }

        void internal_destroy() noexcept
                requires(!has_header_v<HandleType>)
        {
        }
        void internal_destroy() noexcept
                requires(!has_parent_v<HandleType>)
        {
            m_control->m_header.deleter(m_handle);
        }
        void internal_destroy() noexcept
                requires(has_parent_v<HandleType> && !has_pool_v<HandleType>)
        {
            m_control->m_header.deleter(m_control->m_header.parent.get(), m_handle);
        }
        void internal_destroy() noexcept
                requires(has_pool_v<HandleType>)
        {
            m_control->m_header.deleter(m_control->m_header.parent.get(), m_control->m_header.pool.get(), m_handle);
        }

    protected:
        control_block<HeaderType>* m_control = nullptr;
        HandleType m_handle{};
    };

    template<typename HandleType>
    class shared_handle : public shared_handle_base<HandleType>
    {
    public:
        using shared_handle_base<HandleType>::shared_handle_base;
    };

    //=====================================================================================================================

    template<typename HandleType>
    class managed_handle;

    template<typename HandleType, typename HeaderType = managed_header<HandleType>, typename ForwardType = managed_handle<HandleType>>
    class managed_handle_base
    {
    public:
        managed_handle_base() = default;

        template<typename... Args>
        managed_handle_base(HandleType handle, Args&&... control_args) noexcept
            : m_header{ std::forward<Args>(control_args)... }, m_handle(handle)
        {
        }

        managed_handle_base(const managed_handle_base& o) noexcept = delete;
        managed_handle_base& operator=(const managed_handle_base& o) noexcept = delete;

        managed_handle_base(managed_handle_base&& o) noexcept
            : m_header(o.m_header), m_handle(o.m_handle)
        {
            o.m_header = nullptr;
            o.m_handle = nullptr;
        }

        managed_handle_base& operator=(managed_handle_base&& o) noexcept
        {
            managed_handle_base(std::move(o)).swap(*this);
            return *this;
        }

        ~managed_handle_base() noexcept
        {
            if (m_handle)
                static_cast<ForwardType*>(this)->internal_destroy();
        }

    public:
        HandleType get() const noexcept
        {
            return m_handle;
        }

        HandleType operator*() const noexcept
        {
            return m_handle;
        }

        explicit operator bool() const noexcept
        {
            return m_handle != nullptr;
        }

        void reset() noexcept
        {
            managed_handle_base().swap(*this);
        }

        void swap(managed_handle_base& o) noexcept
        {
            std::swap(m_handle, o.m_handle);
            std::swap(m_header, o.m_header);
        }

        const HeaderType& header() const noexcept
        {
            return m_header;
        }
        const shared_handle<parent_of_t<HandleType>>& parent() const noexcept
                requires(has_parent_v<HandleType>)
        {
            return m_header.parent;
        }

    protected:
        void internal_destroy() noexcept
                requires(!has_header_v<HandleType>)
        {
        }
        void internal_destroy() noexcept
                requires(!has_parent_v<HandleType> && !has_pool_v<HandleType>)
        {
            m_header.deleter(m_handle);
        }
        void internal_destroy() noexcept
                requires(has_parent_v<HandleType> && !has_pool_v<HandleType>)
        {
            m_header.deleter(m_header.parent.get(), m_handle);
        }
        void internal_destroy() noexcept
                requires(has_pool_v<HandleType>)
        {
            m_header.deleter(m_header.parent.get(), m_header.pool.get(), m_handle);
        }

    protected:
        HeaderType m_header;
        HandleType m_handle;
    };

    template<typename HandleType>
    class managed_handle : public managed_handle_base<HandleType>
    {
    public:
        using managed_handle_base<HandleType>::managed_handle_base;
    };

    //=====================================================================================================================

    enum class owned_by_swapchain {
        yes,
        no
    };

    struct image_header : public managed_header<VkImage> {
        owned_by_swapchain m_owned;
    };
    struct swapchain_header {
        shared_handle<VkSurfaceKHR> surface;
        shared_handle<parent_of_t<VkSwapchainKHR>> parent;
        single_deleter<VkSwapchainKHR> deleter;
    };

    template<>
    class shared_handle<VkImage> : public shared_handle_base<VkImage, image_header>
    {
        friend shared_handle_base<VkImage, image_header>;
        using shared_handle_base<VkImage, image_header>::shared_handle_base;

    protected:
        void internal_destroy() noexcept
        {
            if (m_control->m_header.m_owned == owned_by_swapchain::no)
                m_control->m_header.deleter(m_control->m_header.parent.get(), m_handle);
        }
    };
    template<>
    class managed_handle<VkImage> : public managed_handle_base<VkImage, image_header>
    {
        friend managed_handle_base<VkImage, image_header>;
        using managed_handle_base<VkImage, image_header>::managed_handle_base;

    protected:
        void internal_destroy() noexcept
        {
            if (m_header.m_owned == owned_by_swapchain::no)
                m_header.deleter(m_header.parent.get(), m_handle);
        }
    };

    template<>
    class shared_handle<VkSwapchainKHR> : public shared_handle_base<VkSwapchainKHR, swapchain_header>
    {
    public:
        using shared_handle_base<VkSwapchainKHR, swapchain_header>::shared_handle_base;
        const shared_handle<VkSurfaceKHR>& surface() const noexcept
        {
            return m_control->m_header.surface;
        }
    };
    template<>
    class managed_handle<VkSwapchainKHR> : public managed_handle_base<VkSwapchainKHR, swapchain_header>
    {
    public:
        using managed_handle_base<VkSwapchainKHR, swapchain_header>::managed_handle_base;
        const shared_handle<VkSurfaceKHR>& surface() const noexcept
        {
            return m_header.surface;
        }
    };
}
