/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** DLLoader
*/

#include <cstdint>
#include <dlfcn.h>
#include <stdexcept>
#include "Core/DLLoader.hpp"

arc::DLLoader::DLLoader(const std::string &path)
    : _handle(0)
{
    void *handle = dlopen(path.c_str(), RTLD_LAZY);

    if (!handle) {
        throw std::runtime_error(dlerror());
    }
    _handle = reinterpret_cast<std::uintptr_t>(handle);
}

arc::DLLoader::~DLLoader() noexcept
{
    if (_handle) {
        dlclose(reinterpret_cast<void *>(_handle));
    }
}

std::uintptr_t arc::DLLoader::loadSymbol(const std::string &name)
{
    dlerror();
    void *sym = dlsym(reinterpret_cast<void *>(_handle), name.c_str());
    const char *error = dlerror();

    if (error) {
        throw std::runtime_error(std::string("symbol '") + name + "': " + error);
    }
    return reinterpret_cast<std::uintptr_t>(sym);
}

bool arc::DLLoader::isLoaded() const noexcept
{
    return _handle != 0;
}
