/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Core
*/

#ifndef CORE_HPP
    #define CORE_HPP

#include <memory>
#include <string>
#include <string_view>
#include "Core/DLLoader.hpp"
#include "IDisplayModule.hpp"
#include "IGameModule.hpp"

namespace arc {
    class Core {
        public:
            Core(const std::string &displayPath);
            ~Core() noexcept = default;

            void run();

        private:
            static constexpr std::string_view DEFAULT_GAME =
                "./lib/arcade_dummy_game.so";

            std::unique_ptr<DLLoader> _displayLoader;
            std::unique_ptr<DLLoader> _gameLoader;
            std::shared_ptr<IDisplayModule> _display;
            std::shared_ptr<IGameModule> _game;

            void loadDisplay(const std::string &path);
            void loadGame(const std::string &path);
            bool handleCoreEvent(const Event &event) const noexcept;
    };
}

#endif
