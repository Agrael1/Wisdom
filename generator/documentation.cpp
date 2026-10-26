#include "generator.hpp"

const std::string_view Generator::struct_documentation_template =
    R"(/**
 * @struct {0}
 * @ingroup Structures {3}
 *
 *
 * @section {0}_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * {1}
 * \endcond
 *
 * @section {0}_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * {2}
 * \endcond
 *
 * @section {0}_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section {0}_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */)";

std::string Generator::MakeStructDescription(const WisStruct& s)
{
    std::string description;
    for (auto& m : s.members) {
        description += std::format("- `{}` {}\n", m.name, m.doc.empty() ? "No description." : m.doc);
    }
    return description;
}

std::string Generator::FinalizeNormativeDocumentation(std::string doc)
{
    // Keep inline comments and the brief section unchanged.
    size_t start_replacing = 0;
    if (doc.find("///<") != std::string::npos || doc.find("/// @brief") != std::string::npos) {
        start_replacing = doc.length();
    } else {
        size_t brief_start = doc.find("@brief");
        if (brief_start != std::string::npos) {
            size_t brief_end = doc.find("\n * \n * ", brief_start);
            if (brief_end == std::string::npos) {
                brief_end = doc.find("\n * \n", brief_start);
            }
            if (brief_end != std::string::npos) {
                start_replacing = brief_end;
            } else {
                start_replacing = doc.length();
            }
        }
    }

    if (start_replacing < doc.length()) {
        std::string big_doc = doc.substr(start_replacing);
        ReplaceAll(big_doc, " should not", " @wis_shouldnot ");
        ReplaceAll(big_doc, " must not", " @wis_mustnot");
        ReplaceAll(big_doc, " may not", " @wis_maynot");

        ReplaceAll(big_doc, " should ", " @wis_should ");
        ReplaceAll(big_doc, " must ", " @wis_must ");
        ReplaceAll(big_doc, " may ", " @wis_may ");

        doc.replace(start_replacing, std::string::npos, big_doc);
    }
    return doc;
}

void Generator::ReplaceAll(std::string& str, std::string_view from, std::string_view to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}