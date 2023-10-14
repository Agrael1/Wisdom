#include "generator.h"

int main()
{
    Generator g;
    g.GenerateCAPI();
    g.GenerateCPPAPI();
    return 0;
}
