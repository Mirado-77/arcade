/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

    #include <cstdint>
    #include <functional>
    #include <memory>
    #include <stdexcept>
    #include <string>
    #include <type_traits>

namespace arc {
    class DLLoader {
        public:
            DLLoader(const std::string &path);
            ~DLLoader() noexcept;

            template<typename T>
            std::function<T> getFunction(const std::string &name)
            {
                using FuncPtr = std::add_pointer_t<T>;
                return reinterpret_cast<FuncPtr>(loadSymbol(name));
            }

            bool isLoaded() const noexcept;

        private:
            std::uintptr_t _handle;
            std::uintptr_t loadSymbol(const std::string &name);
    };
}

#endif
