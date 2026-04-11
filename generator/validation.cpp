#include "generator.hpp"

//----------------------------------------------------------------------------------------------------------------------
void Generator::ParseValidations(tinyxml2::XMLElement* validations)
{
    for (auto* validation = validations->FirstChildElement("validation"); validation;
            validation = validation->NextSiblingElement("validation")) {
        auto name = validation->FindAttribute("for")->Value();
        auto& ref = validation_map[name];

        for (auto* check = validation->FirstChildElement("vuid"); check; check = check->NextSiblingElement("vuid")) {
            Validation vcheck;
            vcheck.type_name = name;

            // Severity
            if (auto* severity = check->FindAttribute("severity")) {
                std::string_view sev = severity->Value();
                vcheck.severity = from_chars(sev);
            } else {
                vcheck.severity = Severity::Error; // default
            }

            // ID
            if (auto* id_attr = check->FindAttribute("id")) {
                vcheck.id = id_attr->Value();
            } else {
                throw std::runtime_error(wis::format("Validation for {} is missing id attribute.", name));
            }

            // Message
            if (auto* msg = check->FindAttribute("msg")) {
                vcheck.message = msg->Value();
            } else {
                throw std::runtime_error(wis::format("Validation for {} is missing message attribute.", name));
            }

            ref.push_back(vcheck);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeValidationDescription(const Validation& v)
{
    auto doc = FinalizeCDocumentation(std::string(v.message), v.type_name);
    return wis::format(" * @vuid_begin{{WIS-{}-{}}} {} @vuid_end\n", GetCFullTypename(v.type_name), v.id, doc);
}

//----------------------------------------------------------------------------------------------------------------------
std::string Generator::MakeValidationForType(std::string_view type_name)
{
    std::string vuids;
    auto vuid_list = validation_map.find(type_name);
    if (vuid_list != validation_map.end()) {
        for (auto& vuid : vuid_list->second) {
            vuids += MakeValidationDescription(vuid);
        }
    }

    if (vuids.empty()) {
        vuids = empty_doc;
    } else {
        vuids = std::format(" * @validusage_begin\n{} * @validusage_end\n * ", vuids);
    }
    return vuids;
}
