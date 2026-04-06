/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Core
*/

#ifndef CORE_HPP
    #define CORE_HPP

    #include <cstddef>
    #include <memory>
    #include <string>
    #include <string_view>
    #include <vector>
    #include "Core/DLLoader.hpp"
    #include "Core/ScoreManager.hpp"
    #include "IDisplayModule.hpp"
    #include "IGameModule.hpp"

namespace arc {
    class Core {
        public:
            Core(const std::string &displayPath);
            ~Core() noexcept = default;

            void run();

            ScoreManager &getScores() noexcept;

        private:
            static constexpr std::string_view LIB_DIR = "./lib";
            static constexpr std::string_view SCORE_FILE = "scores.txt";

            ScoreManager _scores;
            std::vector<std::string> _displayLibs;
            std::vector<std::string> _gameLibs;
            std::size_t _displayIdx;
            std::size_t _gameIdx;
            std::unique_ptr<DLLoader> _displayLoader;
            std::unique_ptr<DLLoader> _gameLoader;
            std::shared_ptr<IDisplayModule> _display;
            std::shared_ptr<IGameModule> _game;

            void scanLibs();
            void tryCategorizeLib(const std::string &path);
            void loadDisplay(const std::string &path);
            void loadGame(const std::string &path);
            void nextDisplay();
            void nextGame();
            void restartGame();
            bool processEvent(const Event &event);
            bool processCommand(const Command &cmd);
            bool processCommands();
    };
}

#endif
