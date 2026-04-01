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

            template<typename T>
            std::shared_ptr<T> loadPlugin(
                const std::string &createSym,
                const std::string &destroySym)
            {
                using Ptr = std::add_pointer_t<T>;
                auto create = getFunction<Ptr()>(createSym);
                auto destroy = getFunction<void(Ptr)>(destroySym);
                auto plugin = std::shared_ptr<T>(create(), destroy);
                if (!plugin) {
                    throw std::runtime_error(createSym + ": factory returned nullptr");
                }
                return plugin;
            }

            bool isLoaded() const noexcept;

        private:
            std::uintptr_t _handle;
            std::uintptr_t loadSymbol(const std::string &name);
    };
}

#endif
