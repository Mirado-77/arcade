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
#include "IDisplayModule.hpp"

namespace arc {
    class NcursesDisplay : public IDisplayModule {
        public:
            NcursesDisplay();
            ~NcursesDisplay() override;

            Event getEvent() override;
            int setAssets(Assets assets) override;
            void drawGame(std::reference_wrapper<
                std::pair<Entities, Sounds>> data) override;

        private:
            static const std::unordered_map<int, Action> KEY_MAP;

            void drawEntity(const std::unique_ptr<IEntity> &entity);
            Action mapKey(int key) const noexcept;
            int selectColorPair(RGBA color) const noexcept;
    };
}

#endif
