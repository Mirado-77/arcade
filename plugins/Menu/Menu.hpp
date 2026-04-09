/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Menu
*/

#ifndef MENU_HPP
    #define MENU_HPP

    #include <cstddef>
    #include <string>
    #include <unordered_map>
    #include <vector>
    #include "IGameModule.hpp"

namespace arc {
    struct ColumnConfig {
        const std::vector<std::string> &libs;
        const std::string &title;
        std::size_t cursor;
        std::size_t selected;
        int colX;
        int listX;
    };

    class Menu : public IGameModule {
        public:
            Menu();
            ~Menu() override = default;

            void simulate(Event event) noexcept override;
            Assets getAssets() noexcept override;
            std::pair<Entities, Sounds>
                getElements() noexcept override;
            bool isOpen() noexcept override;
            std::vector<Command>
                loadCommand() noexcept override;

        private:
            enum class Focus {
                Games, Displays, Username, Play
            };

            // Grid
            static constexpr int GRID_W = 40;
            static constexpr int GRID_H = 20;

            // Asset indices
            static constexpr int IDX_BG = 0;
            static constexpr int IDX_LOGO = 1;
            static constexpr int IDX_PLAY = 2;
            static constexpr int IDX_TEXT = 3;

            // Logo
            static constexpr int LOGO_X = 14;
            static constexpr int LOGO_Y = 1;
            static constexpr int LOGO_W = 12;
            static constexpr int LOGO_H = 2;

            // Games column (left)
            static constexpr int GAMES_HEADER_X = 2;
            static constexpr int GAMES_LIST_X = 3;
            static constexpr int GAMES_HEADER_Y = 3;

            // Displays column (right)
            static constexpr int DISP_HEADER_X = 22;
            static constexpr int DISP_LIST_X = 23;
            static constexpr int DISP_HEADER_Y = 3;

            // List items
            static constexpr int LIST_START_Y = 5;
            static constexpr int LIST_ITEM_W = 16;
            static constexpr int HEADER_W = 16;

            // Center column
            static constexpr int CENTER_X = 14;

            // Name input
            static constexpr int NAME_Y = 10;
            static constexpr int NAME_INPUT_Y = 11;
            static constexpr std::size_t MAX_NAME = 10;

            // Play button
            static constexpr int PLAY_Y = 13;
            static constexpr int PLAY_W = 12;

            // Scores
            static constexpr int SCORES_X = 2;
            static constexpr int SCORES_HEADER_Y = 15;
            static constexpr int SCORES_START_Y = 16;
            static constexpr int SCORES_MAX = 4;
            static constexpr int SCORES_W = 18;

            // Misc
            static constexpr std::string_view
                LIB_EXTENSION = ".so";

            // Colors
            static constexpr RGBA CYAN =
                {0, 200, 255, 255};
            static constexpr RGBA YELLOW =
                {255, 255, 0, 255};
            static constexpr RGBA WHITE =
                {255, 255, 255, 255};
            static constexpr RGBA GRAY =
                {180, 180, 180, 255};
            static constexpr RGBA DARK_GRAY =
                {100, 100, 100, 255};
            static constexpr RGBA GREEN =
                {0, 255, 0, 255};

            using NavMap =
                std::unordered_map<Focus, Focus>;
            static const NavMap LEFT_NAV;
            static const NavMap RIGHT_NAV;

            std::vector<std::string> _gameLibs;
            std::vector<std::string> _displayLibs;
            std::size_t _gameCursor;
            std::size_t _displayCursor;
            std::size_t _gameSelected;
            std::size_t _displaySelected;
            std::string _username;
            Focus _focus;
            bool _open;
            std::vector<Command> _pendingCommands;
            std::vector<std::pair<std::string, int>>
                _scores;

            void scanLibs();
            void categorizeLib(
                const std::string &path);
            void handleVerticalNav(
                Action action) noexcept;
            void moveCursorUp() noexcept;
            void moveCursorDown() noexcept;
            void handleHorizontalNav(
                Action action) noexcept;
            void handleEnter() noexcept;
            void launchGame() noexcept;
            void handleLetter(
                Action action) noexcept;
            void buildColumn(
                Entities &entities,
                const ColumnConfig &col,
                Focus targetFocus) const;
            void buildColumnItem(
                Entities &entities,
                const ColumnConfig &col,
                Focus targetFocus,
                std::size_t i) const;
            void buildNameInput(
                Entities &entities) const;
            void buildPlayButton(
                Entities &entities) const;
            void buildScores(
                Entities &entities) const;
            void loadScores();
            RGBA itemColor(
                bool selected,
                bool cursor) const noexcept;
            static std::size_t wrapDecrement(
                std::size_t idx,
                std::size_t size) noexcept;
            static std::string libFileName(
                const std::string &path);
    };
}

#endif
