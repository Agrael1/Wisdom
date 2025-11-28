#include <string>
#include <optional>
#include <vector>

enum class DocKind {
    Full,
    VersionOnly,
};

//-----------------------------------------------------------------------------
enum class TypeKind {
    None,
    Base,
    Struct,
    Variant,
    Union,
    Enum,
    Bitmask,
    Handle,
    FuncPointer,
    Alias,
};
enum ImplementedFor {
    Both,
    DX12,
    Vulkan,
};
enum Modifier {
    None = 0,
    Pointer = 1 << 0,
    Reference = 1 << 1,
    Const = 1 << 2,
    Nodiscard = 1 << 4,
    PointerToPointer = 1 << 5,
    Span = 1 << 6,
};

struct InlineTypeInfo {
    std::string_view type;
    std::string_view value;
    std::size_t pos;
    std::size_t after;
};

struct Dependencies {
    std::vector<std::string_view> dependencies;
};

struct WisConvert {
    std::string_view value;
    bool direct = false;
};
struct WisEnumValue {
    std::string_view name;
    std::string_view doc;
    std::string_view version;
    std::array<std::string_view, 3> converts;
    int64_t value = 0;
};
struct WisEnum {
    std::string_view name;
    std::string_view type;
    std::string_view doc;
    std::string_view version;
    std::string doc_translates;
    std::vector<WisEnumValue> values;
    std::array<WisConvert, 3> conversion_type;

public:
    std::optional<WisEnumValue> HasValue(std::string_view name) const noexcept
    {
        auto enum_value = std::find_if(values.begin(), values.end(), [&](auto& v) {
            return v.name == name;
        });
        return enum_value != values.end() ? std::optional<WisEnumValue>{ *enum_value } : std::nullopt;
    }
};

//-----------------------------------------------------------------------------

struct WisStructMember {
    std::string_view name;
    std::string_view type;
    std::string_view array_size;
    Modifier modifier;
    std::string_view default_value;
    std::string_view doc;
};

struct WisStruct {
    std::string_view name;
    std::string_view doc;
    std::string_view version;
    Modifier modifier = Modifier::None;
    std::vector<WisStructMember> members;

public:
    std::optional<WisStructMember> HasValue(std::string_view name) const noexcept
    {
        if (name.empty()) {
            return {};
        }

        auto enum_value = std::find_if(members.begin(), members.end(), [&](auto& v) {
            return v.name == name;
        });
        return *enum_value;
    }
};

//-----------------------------------------------------------------------------
struct WisHandle {
    std::string_view name;
    std::string_view doc;
    std::string_view version;
    std::array<uint32_t, 2> sizes;

    std::vector<std::string_view> functions;
public:
    uint32_t GetSize(ImplementedFor impl) const noexcept
    {
        if (impl == ImplementedFor::DX12) {
            return sizes[0];
        }
        if (impl == ImplementedFor::Vulkan) {
            return sizes[1];
        }
        return 0;
    }
};