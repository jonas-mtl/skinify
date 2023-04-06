#include "interface/imGui.h"

int main(int, char**)
{
    if (!Interface::Render()) return 1;

    return 0;
}
