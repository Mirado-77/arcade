/*
** EPITECH PROJECT, 2026
** Arcade-perso
** File description:
** SdlibDisplay
*/

#ifndef SDLIBDISPLAY_HPP_
    #define SDLIBDISPLAY_HPP_
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_render.h>
    #include <SDL2/SDL_ttf.h>
    #include <vector>
    #include <unordered_map>
    #include "IDisplayModule.hpp"

namespace arc {
    class SdlibDisplay: public IDisplayModule {
        public:
        SdlibDisplay();
        ~SdlibDisplay() override;
        
        Event getEvent() noexcept override;
        int setAssets(const Assets assets) noexcept override;
        void drawGame(const std::pair<Entities, Sounds> data) noexcept override;

        protected:
        private:
            static constexpr unsigned WIN_W = 1280u;
            static constexpr unsigned WIN_H = 720u;
            static constexpr unsigned HUD_H = 30u;
            static const std::unordered_map<int, arc::Action> KEY_MAP;
            static const std::unordered_map<int, arc::Action> MOUSE_BUTTON_MAP;

            std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window;
            std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer;
            std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> _font;
            bool _hasFont;
            static constexpr SDL_Color _fontColor = {0, 0, 0, 0};

            using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
            using TexturePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
            std::vector<SurfacePtr> _surfaces;
            std::vector<TexturePtr> _textures;

            Action mapKey(SDL_Keysym keY) const noexcept;
            Action mapMouseButton(SDL_MouseButtonEvent btn) const noexcept;
            void drawEntitySprite(const std::unique_ptr<IEntity> &entity,
                SDL_Rect DESTr, int idx);
            void drawEntityRect(const std::unique_ptr<IEntity> &entity,
                SDL_Rect DESTr, int idx);
            void drawEntity(const std::unique_ptr<IEntity> &entity);
            void drawHud();
            void handleSounds(const Sounds &sounds);
    };
}

#endif /* !SDLIBDISPLAY_HPP_ */
