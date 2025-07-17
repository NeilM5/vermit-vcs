#include <iostream>
#include <string>

#include "commands.cpp"

using str = std::string;

// Get argument with index
str getArg(int index, int argc, char* argv[])
{
    if (index >= argc) return "";

    return str(argv[index]);
}

int main(int argc, char* argv[])
{
    if (argc < 2) return 0;

    if (getArg(1, argc, argv) != "vermit")
    {
        std::cout << "commands should start with 'vermit'";
    }

    if (getArg(2, argc, argv) == "init")
    {
        cmds::init(getArg(3, argc, argv), getArg(4, argc, argv));
    }

    return 0;
}