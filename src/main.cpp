/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Main entry point
*/

#include <iostream>
#include <string>
#include <string_view>
#include "Core/Core.hpp"

static void printUsage(std::string_view name)
{
    std::cerr << "Usage: " << name << " <graphical_library.so>" << std::endl;
}

int main(int ac, char **av)
{
    if (ac != 2) {
        printUsage(std::string_view(av[0]));
        return 84;
    }
    try {
        arc::Core core{std::string(av[1])};
        core.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
