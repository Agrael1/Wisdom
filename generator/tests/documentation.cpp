#include "generator.hpp"
#include <iostream>
#include <string_view>

struct DocumentationCase {
    std::string_view name;
    std::string_view input;
    std::string_view expected;
};

int main()
{
    constexpr DocumentationCase cases[]{
        {"empty", "", ""},
        {"unchanged text", "An ordinary description.", "An ordinary description."},
        {"positive words",
         "Values should match, must exist and may change.",
         "Values @wis_should match, @wis_must exist and @wis_may change."},
        {"negative words",
         "Values should not change, must not vanish and may not overlap.",
         "Values @wis_shouldnot  change, @wis_mustnot vanish and @wis_maynot overlap."},
        {"repeated words", "Values must match and must exist.", "Values @wis_must match and @wis_must exist."},
        {"word boundaries", "must exist; values may. They MUST remain.", "must exist; values may. They MUST remain."},
        {"inline member comment", "///< Values must exist.", "///< Values must exist."},
        {"inline brief comment", "/// @brief Values should match.", "/// @brief Values should match."},
        {"brief without body", "/**\n * @brief Values may change.\n */", "/**\n * @brief Values may change.\n */"},
        {"brief with body",
         "/**\n * @brief Values must exist.\n * \n * Values should match.\n */",
         "/**\n * @brief Values must exist.\n * \n * Values @wis_should match.\n */"},
        {"brief fallback delimiter",
         "@brief Values must exist.\n * \nValues may change.",
         "@brief Values must exist.\n * \nValues @wis_may change."},
        {"body without brief", "/**\n * Values must exist.\n */", "/**\n * Values @wis_must exist.\n */"},
        {"already finalized", "Values @wis_must exist.", "Values @wis_must exist."},
    };

    bool passed = true;
    for (const auto& test : cases) {
        const auto actual = Generator::FinalizeNormativeDocumentation(std::string(test.input));
        if (actual != test.expected) {
            std::cerr << "Failed: " << test.name << "\nExpected:\n" << test.expected << "\nActual:\n" << actual
                      << '\n';
            passed = false;
        }
    }
    return passed ? 0 : 1;
}