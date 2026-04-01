/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SfmlDisplay
*/

#ifndef SFMLDISPLAY_HPP
    #define SFMLDISPLAY_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "IDisplayModule.hpp"

namespace arc {
    class SfmlDisplay : public IDisplayModule {
        public:
            SfmlDisplay();
            ~SfmlDisplay() override;

            Event getEvent() override;
            int setAssets(Assets assets) override;
            void drawGame(std::reference_wrapper<
                std::pair<Entities, Sounds>> data) override;

        private:
            static constexpr float CELL = 20.f;
            static constexpr unsigned WIN_W = 800u;
            static constexpr unsigned WIN_H = 430u;
            static constexpr unsigned HUD_H = 30u;
            static const std::unordered_map<int, Action> KEY_MAP;

            sf::RenderWindow _window;
            sf::Font _font;
            bool _hasFont;

            Action mapKey(int key) const noexcept;
            sf::Color toColor(RGBA rgba) const noexcept;
            void drawEntity(const std::unique_ptr<IEntity> &entity);
            void drawHud();
    };
}

#endif
