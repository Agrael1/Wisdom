#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/vulkan/vk_handle_traits.h>
#endif

WIS_EXPORT namespace wis
{
    template<typename HandleType>
    class shared_handle;

    template<typename DestructorType, typename Deleter>
    struct shared_header {
        shared_header(shared_handle<DestructorType> parent, Deleter deleter = Deleter()) noexcept
            : parent(std::move(parent)), deleter(std::move(deleter))
        {
        }

        shared_handle<DestructorType> parent;
        Deleter deleter;
    };

    template<typename Deleter>
    struct shared_header<empty_type, Deleter> {
        shared_header(Deleter deleter = Deleter()) noexcept
            : deleter(std::move(deleter)) { }

        Deleter deleter;
    };

    template<typename DestructorType, typename PoolType, typename Deleter>
    struct pool_header {
        pool_header(shared_handle<DestructorType> parent, shared_handle<PoolType> pool, Deleter deleter = Deleter()) noexcept
            : parent(std::move(parent)), pool(std::move(pool)), deleter(std::move(deleter))
        {
        }

        shared_handle<DestructorType> parent;
        shared_handle<PoolType> pool;
        Deleter deleter;
    };

    template<typename HandleType>
    using deleter_of_t = std::conditional_t<std::is_same_v<typename handle_traits<HandleType>::deleter_pool, empty_type>,
                                            deleter<HandleType>, pool_deleter<HandleType>>;

    template<typename HandleType>
    using header_of_t = std::conditional_t<std::is_same_v<typename handle_traits<HandleType>::deleter_pool, empty_type>,
                                           shared_header<typename handle_traits<HandleType>::deleter_parent, deleter<HandleType>>,
                                           pool_header<typename handle_traits<HandleType>::deleter_parent, typename handle_traits<HandleType>::deleter_pool, pool_deleter<HandleType>>>;

    //=====================================================================================================================

    template<typename HeaderType>
    class control_block
    {
    public:
        template<typename... Args>
        control_block(Args&&... control_args)
            : m_header(std::forward<Args>(control_args)...)
        {
        }

        control_block(const control_block&) = delete;
        control_block& operator=(const control_block&) = delete;

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
        HeaderType m_header{};
    };

    template<>
    class control_block<empty_type>
    {
    public:
        control_block() = default;
        control_block(const control_block&) = delete;
        control_block& operator=(const control_block&) = delete;

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

    //=====================================================================================================================

    template<typename HandleType, typename HeaderType = header_of_t<HandleType>, typename ForwardType = shared_handle<HandleType>>
    class shared_handle_base
    {
    public:
        using parent_type = typename handle_traits<HandleType>::deleter_parent;

        static inline constexpr bool has_header = !std::is_same<HeaderType, empty_type>::value;
        static inline constexpr bool has_parent = has_header && !std::is_same_v<parent_type, empty_type>;
        static inline constexpr bool has_pool = !std::is_same_v<typename handle_traits<HandleType>::deleter_pool, empty_type>;

    public:
        shared_handle_base() = default;

        template<typename... Args>
        shared_handle_base(HandleType handle, Args&&... control_args) noexcept
            : m_control((control_block<HeaderType>*)std::malloc(sizeof(control_block<HeaderType>))), m_handle(handle)
        {
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
                delete m_control;
            }
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
            shared_handle_base().swap(*this);
        }

        void swap(shared_handle_base& o) noexcept
        {
            std::swap(m_handle, o.m_handle);
            std::swap(m_control, o.m_control);
        }

        const HeaderType& header() const noexcept
            requires has_header
        {
            return m_control->m_header;
        }
        const shared_handle<parent_type>& parent() const noexcept
            requires has_parent
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
            requires !has_header
        {
        }
        void internal_destroy() noexcept
            requires has_parent && !has_pool
        {
            m_control->m_header.deleter(m_control->m_header.parent.get(), m_handle);
        }
        void internal_destroy() noexcept
            requires !has_parent
        {
            m_control->m_header.deleter(m_handle);
        }
        void internal_destroy() noexcept
            requires has_pool
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

    template<typename HandleType, typename HeaderType = header_of_t<HandleType>, typename ForwardType = managed_handle<HandleType>>
    class managed_handle_base
    {
    public:
        using parent_type = typename handle_traits<HandleType>::deleter_parent;

        static inline constexpr bool has_header = !std::is_same<HeaderType, empty_type>::value;
        static inline constexpr bool has_parent = has_header && !std::is_same_v<parent_type, empty_type>;
        static inline constexpr bool has_pool = !std::is_same_v<typename handle_traits<HandleType>::deleter_pool, empty_type>;

    public:
        managed_handle_base() = default;

        template<typename... Args>
        managed_handle_base(HandleType handle, Args&&... control_args) noexcept
            : m_header(std::forward<Args>(control_args)...), m_handle(handle)
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
        const shared_handle<parent_type>& parent() const noexcept
            requires has_parent
        {
            return m_header.parent;
        }

    protected:
        void internal_destroy() noexcept
            requires !has_header
        {
        }
        void internal_destroy() noexcept
            requires has_parent && !has_pool
        {
            m_header.deleter(m_header.parent.get(), m_handle);
        }
        void internal_destroy() noexcept
            requires !has_parent
        {
            m_header.deleter(m_handle);
        }
        void internal_destroy() noexcept
            requires has_pool
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

    struct image_header : public header_of_t<VkImage> {
        image_header(shared_handle<handle_traits<VkImage>::deleter_parent> parent, wis::deleter<VkImage> deleter, owned_by_swapchain owned = owned_by_swapchain::no) noexcept
            : header_of_t<VkImage>(std::move(parent), std::move(deleter)), m_owned(owned)
        {
        }
        owned_by_swapchain m_owned;
    };
    struct swapchain_header {
        swapchain_header(shared_handle<typename handle_traits<VkSwapchainKHR>::deleter_parent> parent,
                         deleter<VkSwapchainKHR> deleter,
                         shared_handle<VkSurfaceKHR> surface) noexcept
            : surface(std::move(surface)), parent(std::move(parent)), deleter(std::move(deleter))
        {
        }
        shared_handle<VkSurfaceKHR> surface;
        shared_handle<typename handle_traits<VkSwapchainKHR>::deleter_parent> parent;
        deleter<VkSwapchainKHR> deleter;
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
