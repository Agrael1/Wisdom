#include "generator.h"

inline constexpr std::string_view input_file = INPUT_FILE;

int main()
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(input_file.data()) != tinyxml2::XMLError::XML_SUCCESS)
        return 1;

    Generator g(doc);
    g.GenerateCAPI();
    g.GenerateCPPAPI();
    return 0;
}
