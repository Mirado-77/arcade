/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SfmlDisplay
*/

#ifndef SFMLDISPLAY_HPP
    #define SFMLDISPLAY_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <unordered_map>
    #include <vector>
    #include "IDisplayModule.hpp"

namespace arc {
    class SfmlDisplay : public IDisplayModule {
        public:
            SfmlDisplay();
            ~SfmlDisplay() override;

            Event getEvent() noexcept override;
            int setAssets(const Assets assets) noexcept override;
            void drawGame(const std::pair<Entities, Sounds> data) noexcept override;

        private:
            static constexpr float CELL = 32.f;
            static constexpr unsigned WIN_W = 1280u;
            static constexpr unsigned WIN_H = 720u;
            static constexpr unsigned HUD_H = 30u;
            static const std::unordered_map<int, Action> KEY_MAP;
            static const std::unordered_map<sf::Mouse::Button, Action> MOUSE_BUTTON_MAP;

            sf::RenderWindow _window;
            sf::Font _font;
            bool _hasFont;

            std::vector<sf::Texture> _textures;
            std::vector<sf::SoundBuffer> _soundBuffers;
            std::vector<sf::Sound> _sounds;

            Action mapKey(int key) const noexcept;
            Action mapMouseButton(sf::Mouse::Button btn) const noexcept;
            sf::Color toColor(RGBA rgba) const noexcept;
            void drawEntitySprite(const std::unique_ptr<IEntity> &entity,
                sf::Vector2f pos, sf::Vector2f size, int idx);
            void drawEntityRect(const std::unique_ptr<IEntity> &entity,
                sf::Vector2f pos, sf::Vector2f size, int idx);
            void drawEntity(const std::unique_ptr<IEntity> &entity);
            void drawHud();
            void handleSounds(const Sounds &sounds);
    };
}

#endif
