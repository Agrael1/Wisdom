#include <string>
#include <optional>
#include <vector>

enum class DocKind {
    Full,
    VersionOnly,
};

enum class ProtoType {
    Prefixed,
    Universal,
    ClassMember
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
    Function,
    Alias,
};
enum ImplementedFor {
    Both,
    DX12,
    Vulkan,
};
enum Modifier {
    None             = 0,
    Pointer          = 1 << 0,
    Reference        = 1 << 1,
    Const            = 1 << 2,
    Nodiscard        = 1 << 4,
    PointerToPointer = 1 << 5,
    Span             = 1 << 6,
    Destroy          = 1 << 7,
    COnly            = 1 << 8,
};
enum ReturnTypeKind {
    Void,
    Direct,
    ResultOnly,
    ResultAndValue,
};
enum Severity {
    Info,
    Warning,
    Error,
};
enum class Lang {
    C,
    CPP,
};

struct InlineTypeInfo {
    std::string_view type;
    std::string_view value;
    std::size_t      pos;
    std::size_t      after;
};

struct Dependencies {
    std::vector<std::string_view> dependencies;
};

struct WisConvert {
    std::string_view value;
    bool             direct = false;
};
struct WisEnumValue {
    std::string_view                name;
    std::string_view                doc;
    std::string_view                version;
    std::array<std::string_view, 3> converts;
    int64_t                         value = 0;
};
struct WisEnum {
    std::string_view          name;
    std::string_view          type;
    std::string_view          doc;
    std::string_view          version;
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
    Modifier         modifier;
    std::string_view default_value;
    std::string_view doc;
};

struct WisStruct {
    std::string_view             name;
    std::string_view             doc;
    std::string_view             version;
    Modifier                     modifier = Modifier::None;
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
    std::string_view        name;
    std::string_view        doc;
    std::string_view        version;
    std::array<uint32_t, 2> sizes{};

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

//-----------------------------------------------------------------------------
// struct ReplacedParameter {
//    Language replace_for = Language::None;
//
//    TypeInfo type_info = TypeInfo::None;
//    std::string_view type;
//    std::string_view name;
//    std::string_view modifier;
//    std::string_view default_value;
//    std::string_view doc;
//};

struct WisFunctionParameter {
    // std::optional<ReplacedParameter> replaced;
    std::string_view type;
    std::string_view doc;
    std::string_view name;
    Modifier         modifier = Modifier::None;
    std::string_view default_value;
};

struct WisReturnType {
    bool             has_result = false;
    std::string_view type;
    std::string_view doc;
    std::string_view opt_name;
    Modifier         modifier = Modifier::None;

    ReturnTypeKind GetKind() const noexcept
    {
        if (IsVoid()) {
            return ReturnTypeKind::Void;
        }
        if (IsDirect()) {
            return ReturnTypeKind::Direct;
        }
        if (IsResultOnly()) {
            return ReturnTypeKind::ResultOnly;
        }
        return ReturnTypeKind::ResultAndValue;
    }

    bool IsVoid() const noexcept
    {
        return type.empty() && !has_result;
    }
    bool IsRV() const noexcept
    {
        return has_result && !type.empty();
    }
    bool IsDirect() const noexcept
    {
        return !has_result && !type.empty();
    }
    bool IsResultOnly() const noexcept
    {
        return has_result && type.empty();
    }
};
struct WisFunction {
    std::string_view name;
    std::string_view doc;
    std::string_view this_type;
    std::string_view version;
    Modifier         modifier = Modifier::None;

    WisReturnType                     return_type;
    std::vector<WisFunctionParameter> parameters;

    std::optional<WisFunctionParameter> HasValue(std::string_view name) const noexcept
    {
        if (name.empty()) {
            return {};
        }
        auto enum_value = std::find_if(parameters.begin(), parameters.end(), [&](auto& v) {
            return v.name == name;
        });
        if (enum_value == parameters.end()) {
            // it can be return value
            if (return_type.opt_name == name) {
                return WisFunctionParameter{ return_type.type, return_type.doc, return_type.opt_name, return_type.modifier, "" };
            }
            return {};
        }
        return *enum_value;
    }
};

static inline constexpr Severity from_chars(std::string_view input) noexcept
{
    if (input == "info") {
        return Severity::Info;
    }
    if (input == "warning") {
        return Severity::Warning;
    }
    if (input == "error") {
        return Severity::Error;
    }
    return Severity::Info;
}

struct Validation {
    std::string_view type_name;
    std::string_view id;
    Severity         severity;
    std::string_view message;
};

using ValidationList = std::vector<Validation>;
using MethodList     = std::vector<std::string_view>;
