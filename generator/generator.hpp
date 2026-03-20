#pragma once
#include <tinyxml2.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <span>
#include <array>
#include <optional>

#include "types.hpp"
#include "../src/include/wisdom/bridge/format.hpp"

class Generator
{
    static constexpr std::string_view main_output_dir     = CPP_OUTPUT_DIR;
    static constexpr std::string_view platform_output_dir = PLATFORM_OUTPUT_DIR;
    static constexpr std::string_view doc_output_dir      = DOC_OUTPUT_DIR;
    static constexpr std::string_view empty_doc           = " * ";

    static constexpr inline std::array<std::string_view, 5> impls{
        "",
        "DX12",
        "VK"
    };

public:
    Generator() = default;

public:
    void ParseFile(std::filesystem::path file);
    void ParsePlatformFile(std::filesystem::path file);
    void WriteMainAPI();
    void WritePlatformAPI();
    void WriteModuleAPIDoc(std::string_view module_name = {});
    auto GetFiles() const
    {
        return std::span<const std::filesystem::path>{ files };
    }

public:
    void ParseIncludes(tinyxml2::XMLElement* includes);
    void ParseFile(tinyxml2::XMLDocument& doc);
    void ParseTypes(tinyxml2::XMLElement* types);
    void ParseEnum(tinyxml2::XMLElement* type);
    void ParseStruct(tinyxml2::XMLElement* type);
    void ParseHandles(tinyxml2::XMLElement* handles);
    void ParseVariant(tinyxml2::XMLElement* type);
    void ParseFunctions(tinyxml2::XMLElement* functions);
    void ParseValidations(tinyxml2::XMLElement* validations);
    void ParseBitmask(tinyxml2::XMLElement* type);
    void ParseDelegate(tinyxml2::XMLElement* type);
    void ParseConstants(tinyxml2::XMLElement* constants);
    void ParseRegistrySections(tinyxml2::XMLElement* root);

    // Make
    std::string MakeCEnum(const WisEnum& s, DocKind kind = DocKind::Full);
    std::string MakeCBitmask(const WisBitmask& s, DocKind kind = DocKind::Full);
    std::string MakeCStruct(const WisStruct& s, DocKind kind = DocKind::Full);
    std::string MakeCVariant(const WisStruct& s, Backend backend = Backend::Any, DocKind kind = DocKind::Full);
    std::string MakeCHandle(const WisHandle& s, Backend backend = Backend::Any, DocKind kind = DocKind::Full);
    std::string MakeCFunctionProto(const WisFunction& func, Backend backend = Backend::Any, std::string_view pre_decl = "WISDOM_API", DocKind kind = DocKind::Full);
    std::string MakeCFunctionDecl(const WisFunction& func, Backend backend = Backend::Any, std::string_view pre_decl = "WISDOM_API", DocKind kind = DocKind::Full);
    std::string MakeCDelegate(const WisFunction& func, DocKind kind = DocKind::Full);
    std::string MakeCConstant(const WisConstant& c, DocKind kind = DocKind::Full);
    std::string MakeCPlatform(const WisModule& p, DocKind kind = DocKind::Full);
    std::string MakeConstantDescription(const WisConstant& c);

    std::string MakeEnumDescription(const WisEnum& s);
    std::string MakeBitmaskDescription(const WisBitmask& s);
    std::string MakeEnumConverter(const WisEnum& s, Backend backend);
    std::string MakeBitmaskConverter(const WisBitmask& s, Backend backend);
    std::string MakeStructDescription(const WisStruct& s);
    std::string MakeVariantDescription(const WisStruct& s);
    std::string MakeFunctionDescription(const WisFunction& s);
    std::string MakeDelegateDescription(const WisFunction& s);
    std::string MakeValidationDescription(const Validation& v);
    std::string MakeValidationForType(std::string_view type_name);
    std::string MakeCMemberDeclaration(const WisStructMember& member, size_t align_width, Backend backend = Backend::Any);
    std::string MakeCPPMemberDeclaration(const WisStructMember& member, size_t align_width, Backend backend);

    // Make C++
    std::string MakeCPPEnum(const WisEnum& s, DocKind kind = DocKind::Full);
    std::string MakeCPPBitmask(const WisBitmask& s, DocKind kind = DocKind::Full);
    std::string MakeCPPStruct(const WisStruct& s, DocKind kind = DocKind::Full);
    std::string MakeCPPVariant(const WisStruct& s, Backend backend = Backend::Any, DocKind kind = DocKind::Full);
    std::string MakeCPPHandle(const WisHandle& s, Backend backend = Backend::Any, DocKind kind = DocKind::Full);
    std::string MakeCPPView(const WisHandle& s, Backend backend = Backend::Any, DocKind kind = DocKind::Full);
    std::string MakeCPPFunctionProto(const WisFunction& func, Backend backend = Backend::Any, std::string_view pre_decl = "WISDOM_API", DocKind kind = DocKind::Full, ProtoType type = ProtoType::Prefixed);
    std::string MakeCPPFunctionImpl(const WisFunction& func, Backend backend = Backend::Any, std::string_view pre_decl = "WISDOM_API", DocKind kind = DocKind::Full, ProtoType type = ProtoType::Prefixed);
    std::string MakeCPPDelegate(const WisFunction& func, DocKind kind = DocKind::Full);
    std::string MakeCPPConstant(const WisConstant& c, DocKind kind = DocKind::Full);
    std::string MakeCPPPlatform(const WisModule& p, DocKind kind = DocKind::Full);
    std::string MakeCIndependentPlatform(const WisModule& p, Backend backend = Backend::Any, DocKind kind = DocKind::Full);
    std::string MakeCPPIndependentPlatform(const WisModule& p, Backend backend = Backend::Any, DocKind kind = DocKind::Full);

    // Write
    void WriteCAPI(std::filesystem::path path);
    void WriteCPPAPI(std::filesystem::path path);
    void WriteCDependentAPI(std::filesystem::path path);
    void WriteCIndependentAPI(std::filesystem::path path);
    void WriteCPPDependentAPI(std::filesystem::path path);
    void WriteCPPIndependentAPI(std::filesystem::path path);
    void WriteCPlatformAPI(std::filesystem::path path);
    void WriteCPPPlatformAPI(std::filesystem::path path);
    void WriteCIndependentPlatformAPI(std::filesystem::path path);
    void WriteCPPIndependentPlatformAPI(std::filesystem::path path);
    void WriteConversions(std::filesystem::path path);
    void WriteEnumDocumentation(std::filesystem::path enum_output_path);
    void WriteBitmaskDocumentation(std::filesystem::path bitmask_output_path);
    void WriteStructDocumentation(std::filesystem::path struct_output_path);
    void WriteVariantDocumentation(std::filesystem::path struct_output_path);
    void WriteHandleDocumentation(std::filesystem::path handle_output_path);
    void WriteFunctionDocumentation(std::filesystem::path func_output_path);
    void WriteDelegateDocumentation(std::filesystem::path func_output_path);
    void WriteConstantDocumentation(std::filesystem::path const_output_path);
    void WriteDocumentation(std::filesystem::path doc_output_path,
                            std::string_view      doc_template,
                            std::string_view      object_name,
                            std::string_view      code,
                            std::string_view      vuids,
                            std::string_view      desc,
                            std::string_view      refs);

    // Helpers
    std::string        GetCFullTypename(std::string_view type, Backend backend = Backend::Any);
    std::string        GetCFullFunctionName(FunctionKey type, Backend backend = Backend::Any);
    std::string        GetCPPFullTypename(std::string_view type, Backend backend = Backend::Any);
    std::string        GetCPPFullFunctionName(FunctionKey type, Backend backend = Backend::Any);
    std::string        FinalizeCDocumentation(std::string doc, std::string_view this_type, Backend backend = Backend::Any);
    std::string        FinalizeCPPDocumentation(std::string doc, std::string_view this_type, Backend backend = Backend::Any);
    std::string        GetSpecificationCode(std::string_view c_code, std::string_view c_impl_code, std::string_view cpp_code, std::string_view cpp_impl_code);

    TypeKind    GetType(std::string_view type_name) const noexcept;
    XBackend    GetTypeBackendSupport(std::string_view type_name) const noexcept;
    void        TryMakeRef(std::string_view type, std::string_view from);
    void        TryMakeRef(std::string_view type, FunctionKey from);
    std::string GetRefs(std::string_view for_type);

    static Backend                    ParseBackend(std::string_view backend) noexcept;
    static ImplOs                     GetImplOs(std::string_view os) noexcept;
    static Extends                    GetExtends(std::string_view extends_str) noexcept;
    static void                       ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    static InlineTypeInfo             FindInlineType(std::string_view str);
    static std::string                MakeVersionString(std::string_view version, bool newline = false);
    static std::string                MakeSnakeCase(std::string_view str);
    static std::string                MakeUpperSnakeCase(std::string_view str);
    static Modifier                   GetModifiers(std::string_view mod_str) noexcept;
    static constexpr std::string_view GetBackendSuffix(Backend backend) noexcept
    {
        switch (backend) {
        case Backend::Any:
            return "";
        case Backend::DX12:
            return "DX12";
        case Backend::Vulkan:
            return "VK";
        default:
            return "";
        }
    }
    static constexpr std::string_view GetBackendTag(Backend backend) noexcept
    {
        switch (backend) {
        case Backend::Any:
            return "";
        case Backend::DX12:
            return "dx";
        case Backend::Vulkan:
            return "vk";
        default:
            return "";
        }
    }
    static constexpr std::string_view GetExtendsString(Extends impl) noexcept
    {
        switch (impl) {
        case Extends::Instance:
            return "Instance";
        case Extends::Device:
            return "Device";
        default:
        case Extends::None:
            return "";
        }
    }

public:
    template<Lang lang = Lang::C, typename T, typename V>
    std::string MakeValueDocumentation(const T& type, const V& value, std::string_view value_decl, DocKind kind)
    {
        std::string version_info;
        if constexpr (requires { value.version; }) {
            version_info = MakeVersionString(value.version);
        }

        if (kind == DocKind::VersionOnly) {
            if constexpr (requires { value.version; }) {
                if (value.version.empty()) {
                    return wis::format("{}\n", value_decl);
                }
                return wis::format("// {}{}\n", version_info, value_decl);
            }
            return wis::format("{}\n", value_decl);
        }

        auto doc       = value.doc;
        auto type_name = type.name;

        std::string documentation;
        bool        pre_doc = false;
        if (!doc.empty()) {
            if (doc.find('\n') != std::string_view::npos) {
                pre_doc       = true;
                documentation = wis::format("/**\n@brief {}\n{}\n*/", version_info, doc);
                ReplaceAll(documentation, "\n", "\n * ");
            } else {
                documentation = wis::format(" ///< {}{}", version_info, doc);
            }
            if constexpr (lang == Lang::CPP) {
                documentation = FinalizeCPPDocumentation(documentation, type_name);
            } else {
                documentation = FinalizeCDocumentation(documentation, type_name);
            }
        }
        return pre_doc
                ? wis::format("    {}\n    {}\n", documentation, value_decl)
                : wis::format("{}{}\n", value_decl, documentation);
    }

    template<Lang lang = Lang::C, typename T>
    std::string MakeTypeDocumentation(const T& type, DocKind kind)
    {
        std::string version_info = MakeVersionString(type.version);
        if (!type.doc.empty() && kind == DocKind::Full) {
            std::string args;
            if constexpr (std::same_as<T, WisFunction>) {
                if constexpr (lang == Lang::C) {
                    // This arg
                    if (!type.this_type.empty()) {
                        args += wis::format("@param self is a pointer to the valid {{{}::}} instance.\n", type.this_type);
                    }

                    // Function arguments
                    for (auto& param : type.parameters) {
                        args += wis::format("@param {} {}\n", param.name, param.doc);
                    }

                    if (type.return_type.IsRV()) {
                        args += wis::format("@param {} {}\n", type.return_type.opt_name, type.return_type.doc);
                        args += wis::format("@return {} {}\n", "Result", "denoting the outcome of operation.");
                    } else if (type.return_type.IsDirect()) {
                        args += wis::format("@return {} {}\n", type.return_type.type, type.return_type.doc);
                    } else if (type.return_type.IsResultOnly()) {
                        args += wis::format("@return {} {}\n", "Result", "denoting the outcome of operation.");
                    }
                } else {
                    // Function arguments, beware of spans
                    bool last_was_span = false;
                    for (auto& param : type.parameters) {
                        if (last_was_span) {
                            last_was_span = false;
                            continue;
                        }
                        if (param.modifier & Modifier::Span) {
                            last_was_span = true;
                        }
                        args += wis::format("@param {} {}\n", param.name, param.doc);
                    }

                    auto kind = type.return_type.GetKind();
                    switch (kind) {
                    case ReturnTypeKind::Direct:
                        args += wis::format("@return {} {}\n", type.return_type.type, type.return_type.doc);
                        break;
                    case ReturnTypeKind::ResultOnly:
                        args += wis::format("@return {} {}\n", "Result", "denoting the outcome of operation.");
                        break;
                    case ReturnTypeKind::ResultAndValue:
                        args += wis::format("@param {} {}\n", "out_result", "denoting the outcome of operation.");
                        args += wis::format("@return {} {}\n",
                                            type.return_type.opt_name,
                                            type.return_type.doc);
                        break;
                    default:
                        break;
                    }
                }
            }

            std::string documentation = wis::format("/**\n@brief {}{}\n{}\n", version_info, type.doc, args);
            if constexpr (requires { type.doc_translates; }) {
                documentation += type.doc_translates;
            }
            documentation += "*/";

            ReplaceAll(documentation, "\n", "\n * ");

            if constexpr (lang == Lang::CPP) {
                return FinalizeCPPDocumentation(documentation, type.name);
            } else {
                return FinalizeCDocumentation(documentation, type.name);
            }
        }
        return wis::format("// {}", version_info);
    }

    template<Lang lang = Lang::C, typename T>
    std::string GetMemberTypeString(const T& member, Backend backend = Backend::Any)
    {
        std::string attributes_pre;
        std::string attributes_inter;
        if (member.modifier & Modifier::Const) {
            attributes_pre += "const ";
        }
        if (member.modifier & Modifier::Pointer) {
            attributes_inter += "*";
        }
        if (member.modifier & Modifier::PointerToPointer) {
            attributes_inter += "**";
        }
        if constexpr (lang == Lang::CPP) {
            if (member.modifier & Modifier::Reference) {
                attributes_inter += "&";
            }
            if (member.modifier & Modifier::Span) {
                return wis::format("wis::span<{}>", attributes_pre + GetCPPFullTypename(member.type, backend) + attributes_inter);
            }
            return attributes_pre + GetCPPFullTypename(member.type, backend) + attributes_inter;
        } else {
            if (member.modifier & Modifier::Reference || member.modifier & Modifier::Span) {
                attributes_inter += "*";
            }
            return attributes_pre + GetCFullTypename(member.type, backend) + attributes_inter;
        }
    }

private:
    std::unordered_map<std::filesystem::path, tinyxml2::XMLDocument> documents;

    std::unordered_map<std::string_view, TypeKind> type_map; // For quick lookup of type kinds

    std::unordered_map<std::string_view, WisEnum>     enum_map;
    std::unordered_map<std::string_view, WisBitmask>  bitmask_map;
    std::unordered_map<std::string_view, WisStruct>   struct_map;
    std::unordered_map<std::string_view, WisStruct>   variant_map;
    std::unordered_map<std::string_view, WisHandle>   handle_map;
    std::unordered_map<FunctionKey, WisFunction>      function_map;
    std::unordered_map<std::string_view, WisFunction> delegate_map;
    std::unordered_map<std::string_view, WisConstant> constant_map;
    std::unordered_map<std::string_view, WisModule>   module_map;
    std::unordered_set<std::string_view>              view_set;

    std::unordered_map<std::string_view, Dependencies>   dependency_tree;
    std::unordered_map<std::string_view, ValidationList> validation_map;

    // Ordered members
    std::vector<std::string_view> enums_in_order;
    std::vector<std::string_view> bitmasks_in_order;
    std::vector<std::string_view> structs_in_order;
    std::vector<std::string_view> variants_in_order;
    std::vector<std::string_view> handles_in_order;
    std::vector<FunctionKey>      functions_in_order;
    std::vector<std::string_view> delegates_in_order;
    std::vector<std::string_view> constants_in_order;
    std::vector<std::string_view> free_functions_in_order;
    std::vector<std::string_view> views_in_order;
    std::vector<std::string_view> modules_in_order;

    std::string_view active_module_name;
    std::string_view platform_module_name;

    std::vector<std::filesystem::path> files;

    // Standard type translations
    const std::unordered_map<std::string_view, std::string_view> standard_types{
        {      "bool",           "bool" },
        {      "void",           "void" },
        {        "u8",        "uint8_t" },
        {       "u16",       "uint16_t" },
        {       "u32",       "uint32_t" },
        {       "u64",       "uint64_t" },
        {        "i8",         "int8_t" },
        {       "i16",        "int16_t" },
        {       "i32",        "int32_t" },
        {       "i64",        "int64_t" },
        {      "size",         "size_t" },

        {       "f32",          "float" },
        {       "f64",         "double" },

        {      "char",           "char" },
        {  "u8string",     "const char" },
        { "u16string", "const char16_t" },
        { "u32string", "const char32_t" },
    };

    const std::unordered_map<std::string_view, std::string_view> standard_types_cpp{
        {      "bool",                "bool" },
        {      "void",                "void" },
        {        "u8",        "std::uint8_t" },
        {       "u16",       "std::uint16_t" },
        {       "u32",       "std::uint32_t" },
        {       "u64",       "std::uint64_t" },
        {        "i8",         "std::int8_t" },
        {       "i16",        "std::int16_t" },
        {       "i32",        "std::int32_t" },
        {       "i64",        "std::int64_t" },
        {      "size",         "std::size_t" },

        {       "f32",               "float" },
        {       "f64",              "double" },

        {      "char",                "char" },
        {  "u8string",          "const char" },
        { "u16string", "const std::char16_t" },
        { "u32string", "const std::char32_t" },
    };
};
//
