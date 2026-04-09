/*
** EPITECH PROJECT, 2026
** Arcade-perso
** File description:
** SdlibDisplay
*/

#include "SdlibDisplay.hpp"

const std::unordered_map<int, arc::Action> arc::SdlibDisplay::KEY_MAP = {
    {SDLK_UP, Action::Up},
    {SDLK_DOWN, Action::Down},
    {SDLK_LEFT, Action::Left},
    {SDLK_RIGHT, Action::Right},
    {SDLK_ESCAPE, Action::Escape},
    {SDLK_F2, Action::F2},
    {SDLK_F3, Action::F3},
    {SDLK_F4, Action::F4},
    {SDLK_F5, Action::F5},
    {SDLK_BACKSPACE, Action::Backspace},
    {SDLK_KP_ENTER, Action::Enter},
    {SDLK_SPACE, Action::Space},
    {SDLK_a, Action::A}, {SDLK_b, Action::B}, {SDLK_c, Action::C},
    {SDLK_d, Action::D}, {SDLK_e, Action::E}, {SDLK_f, Action::F},
    {SDLK_g, Action::G}, {SDLK_h, Action::H}, {SDLK_i, Action::I},
    {SDLK_j, Action::J}, {SDLK_k, Action::K}, {SDLK_l, Action::L},
    {SDLK_m, Action::M}, {SDLK_n, Action::N}, {SDLK_o, Action::O},
    {SDLK_p, Action::P}, {SDLK_q, Action::Q}, {SDLK_r, Action::R},
    {SDLK_s, Action::S}, {SDLK_t, Action::T}, {SDLK_u, Action::U},
    {SDLK_v, Action::V}, {SDLK_w, Action::W}, {SDLK_x, Action::X},
    {SDLK_y, Action::Y}, {SDLK_z, Action::Z},
};

const std::unordered_map<int, arc::Action> arc::SdlibDisplay::MOUSE_BUTTON_MAP = {
    {SDL_BUTTON_LEFT, Action::LeftMouse},
    {SDL_BUTTON_RIGHT, Action::RightMouse},
    {SDL_BUTTON_MIDDLE, Action::MiddleMouse},
};

arc::SdlibDisplay::SdlibDisplay()
    : _window(SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN),
        SDL_DestroyWindow),
    _renderer(nullptr, SDL_DestroyRenderer),
    _font(nullptr, TTF_CloseFont),
    _hasFont(false)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    _font.reset(TTF_OpenFont("assets/menu/font/SpaceMono-Regular.ttf", 80));
    if (!_font){
        _font.reset(TTF_OpenFont("assets/fonts/font.ttf", 80));
    }
    if (_font) {
        _hasFont = true;
    }
}
    
arc::SdlibDisplay::~SdlibDisplay()
{
    SDL_Quit();
}

arc::Action arc::SdlibDisplay::mapKey(SDL_Keysym key) const noexcept
{
    auto it = KEY_MAP.find(key.sym);

    if (it != KEY_MAP.end())
        return it->second;
    return Action::None;
}

arc::Action arc::SdlibDisplay::mapMouseButton(SDL_MouseButtonEvent btn) const noexcept
{
    auto it = MOUSE_BUTTON_MAP.find(btn.button);
    if (it != MOUSE_BUTTON_MAP.end())
        return it->second;
    return Action::None;
}

arc::Event arc::SdlibDisplay::getEvent() noexcept
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return {Action::Close, {0.f, 0.f}};
        if (event.type == SDL_KEYUP)
            return {mapKey(event.key.keysym), {0.f, 0.f}};
        if (event.type == SDL_MOUSEBUTTONUP)
            return {mapMouseButton(event.button), {0.f, 0.f}};
    }
    return {Action::None, {0.f, 0.f}};    
}

int arc::SdlibDisplay::setAssets(const Assets assets) noexcept
{
    _textures.clear();
    for (const auto &path : assets.first) {
        SurfacePtr surface(nullptr, SDL_FreeSurface);
        TexturePtr texture(nullptr, SDL_DestroyTexture);
        surface.reset(IMG_Load((path + ".png").c_str()));
        if (!surface)
            surface.reset(IMG_Load((path + ".jpg").c_str()));
        texture.reset(SDL_CreateTextureFromSurface(_renderer.get(), surface.get()));
        _textures.push_back(std::move(texture));
        _surfaces.push_back(std::move(surface));
        //SDL_FreeSurface(surface.get());
    }
    return 0;
}

void arc::SdlibDisplay::drawEntitySprite(const std::unique_ptr<IEntity> &entity,
    SDL_Rect DESTr, int idx)
{
    auto texIdx = static_cast<std::size_t>(idx);
    SDL_QueryTexture(_textures[texIdx].get(), nullptr, nullptr,
        &DESTr.w, &DESTr.h);
    SDL_RenderCopyEx(_renderer.get(), _textures[texIdx].get(), nullptr, &DESTr,
        entity->getRotation(), nullptr, SDL_FLIP_NONE);
}

void arc::SdlibDisplay::drawEntityRect(const std::unique_ptr<IEntity> &entity,
        SDL_Rect Destr, int idx)
{
    auto [r, g, b, a] =  entity->getColor();
    SDL_SetRenderDrawColor(_renderer.get(), r, g, b, a);
    SDL_RenderFillRect(_renderer.get(), &Destr);
    if (!_hasFont)
        return;
    SurfacePtr surface(nullptr, SDL_FreeSurface);
    TexturePtr texture(nullptr, SDL_DestroyTexture);
    auto str = entity->getStr();
    if(str.has_value())
        surface.reset(TTF_RenderText_Blended(
            _font.get(), str.value().c_str(), _fontColor));
    else
        surface.reset(TTF_RenderText_Blended(_font.get(),
            std::string(1, static_cast<char>(idx + 33)).c_str()
            , _fontColor));
    texture.reset(SDL_CreateTextureFromSurface(_renderer.get(), surface.get()));
    SDL_RenderCopy(_renderer.get(), texture.get(), nullptr, &Destr);
}

void arc::SdlibDisplay::drawEntity(const std::unique_ptr<IEntity> &entity)
{
    SDL_Rect destR;

    auto [px, py] = entity->getPos();
    auto [sw, sh] = entity->getSize();
    float availW = static_cast<float>(WIN_W);
    float availH = static_cast<float>(WIN_H - HUD_H);
    destR.x = px * availW;
    destR.y = static_cast<float>(HUD_H) + py * availH;
    destR.h = sh * availH;
    destR.w = sw * availW;
    if (destR.x < 1.f) destR.w = 1.f;
    if (destR.y < 1.f) destR.h = 1.f;
    int idx = entity->getIdx();
    bool hasTexture = idx >= 0
        && idx < static_cast<int>(_textures.size())
        && _textures[static_cast<std::size_t>(idx)].get() != NULL;
    if (hasTexture)
        drawEntitySprite(entity, destR, idx);
    else
        drawEntityRect(entity, destR, idx);
}

void arc::SdlibDisplay::drawGame(const std::pair<Entities, Sounds> data) noexcept
{
    const auto &[entities, sounds] = data;

    SDL_RenderClear(_renderer.get());
    for (const auto &entity : entities)
        drawEntity(entity);
    SDL_RenderPresent(_renderer.get());
}


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
extern "C"
{
    std::unique_ptr<arc::IDisplayModule> makeInstance()
    {
        return std::make_unique<arc::SdlibDisplay>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Display;
    }
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
