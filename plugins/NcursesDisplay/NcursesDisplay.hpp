/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** NcursesDisplay
*/

#ifndef NCURSESDISPLAY_HPP
    #define NCURSESDISPLAY_HPP

    #include <ncurses.h>
    #include <unordered_map>
    #include <string>
    #include <vector>
    #include "IDisplayModule.hpp"

namespace arc {
    class NcursesDisplay : public IDisplayModule {
        public:
            NcursesDisplay();
            ~NcursesDisplay() override;

            Event getEvent() noexcept override;
            int setAssets(const Assets assets) noexcept override;
            void drawGame(const std::pair<Entities, Sounds> data) noexcept override;

        private:
            static const std::unordered_map<int, Action> KEY_MAP;

            Assets _assets;
            std::vector<std::string> _textAssets;

            void drawEntity(const std::unique_ptr<IEntity> &entity) const noexcept;
            std::string getDisplayStr(const std::unique_ptr<IEntity> &entity) const noexcept;
            Action mapKey(int key) const noexcept;
            Action mapMouseButton(mmask_t bstate) const noexcept;
            Event getMouseEvent() noexcept;
            int selectColorPair(RGBA color) const noexcept;
    };
}

#endif
