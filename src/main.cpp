#include "interface/imGui.hpp"

int main(int, char**)
{
    if (!Interface::Render()) return 1;

    return 0;
}