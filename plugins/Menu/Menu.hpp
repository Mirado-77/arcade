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
            std::pair<Entities, Sounds> getElements() noexcept override;
            bool isOpen() noexcept override;
            std::vector<Command> loadCommand() noexcept override;

        private:
            enum class Focus {
                Games,
                Displays,
                Username,
                Play
            };

            static constexpr int GRID_W = 40;
            static constexpr int GRID_H = 20;
            static constexpr std::size_t MAX_NAME = 10;
            static constexpr int IDX_BG = 0;
            static constexpr int IDX_LOGO = 1;
            static constexpr int IDX_PLAY = 2;
            static constexpr int IDX_TEXT = 3;
            static constexpr int LIST_START_ROW = 6;
            static constexpr std::string_view LIB_EXTENTION = ".so";


            static constexpr RGBA cyan = {0, 200, 255, 255};    
            static constexpr RGBA yellow = {255, 255, 0, 255};
            static constexpr RGBA white = {255, 255, 255, 255};

            using NavMap = std::unordered_map<Focus, Focus>;
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

            void scanLibs();
            void categorizeLib(const std::string &path);
            void handleVerticalNav(Action action) noexcept;
            void moveCursorUp() noexcept;
            void moveCursorDown() noexcept;
            void handleHorizontalNav(Action action) noexcept;
            void handleEnter() noexcept;
            void launchGame() noexcept;
            void handleLetter(Action action) noexcept;
            void buildColumn(Entities &entities, const ColumnConfig &col,
                Focus targetFocus) const;
            void buildColumnItem(Entities &entities, const ColumnConfig &col,
                Focus targetFocus, std::size_t i) const;
            void buildNameInput(Entities &entities) const;
            void buildPlayButton(Entities &entities) const;
            RGBA itemColor(bool selected, bool cursor) const noexcept;
            static std::size_t wrapDecrement(std::size_t idx,
                std::size_t size) noexcept;
            static std::string libFileName(const std::string &path);
    };
}

#endif
