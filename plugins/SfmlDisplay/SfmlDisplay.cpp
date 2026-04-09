/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SfmlDisplay
*/

#include "SfmlDisplay.hpp"
#include "ISound.hpp"
#include <algorithm>

const std::unordered_map<int, arc::Action> arc::SfmlDisplay::KEY_MAP = {
    {sf::Keyboard::A, Action::A}, {sf::Keyboard::B, Action::B},
    {sf::Keyboard::C, Action::C}, {sf::Keyboard::D, Action::D},
    {sf::Keyboard::E, Action::E}, {sf::Keyboard::F, Action::F},
    {sf::Keyboard::G, Action::G}, {sf::Keyboard::H, Action::H},
    {sf::Keyboard::I, Action::I}, {sf::Keyboard::J, Action::J},
    {sf::Keyboard::K, Action::K}, {sf::Keyboard::L, Action::L},
    {sf::Keyboard::M, Action::M}, {sf::Keyboard::N, Action::N},
    {sf::Keyboard::O, Action::O}, {sf::Keyboard::P, Action::P},
    {sf::Keyboard::Q, Action::Q}, {sf::Keyboard::R, Action::R},
    {sf::Keyboard::S, Action::S}, {sf::Keyboard::T, Action::T},
    {sf::Keyboard::U, Action::U}, {sf::Keyboard::V, Action::V},
    {sf::Keyboard::W, Action::W}, {sf::Keyboard::X, Action::X},
    {sf::Keyboard::Y, Action::Y}, {sf::Keyboard::Z, Action::Z},
    
    {sf::Keyboard::Num0, Action::Num0}, {sf::Keyboard::Num1, Action::Num1},
    {sf::Keyboard::Num2, Action::Num2}, {sf::Keyboard::Num3, Action::Num3},
    {sf::Keyboard::Num4, Action::Num4}, {sf::Keyboard::Num5, Action::Num5},
    {sf::Keyboard::Num6, Action::Num6}, {sf::Keyboard::Num7, Action::Num7},
    {sf::Keyboard::Num8, Action::Num8}, {sf::Keyboard::Num9, Action::Num9},

    {sf::Keyboard::Up, Action::Up}, {sf::Keyboard::Down, Action::Down},
    {sf::Keyboard::Left, Action::Left}, {sf::Keyboard::Right, Action::Right},

    {sf::Keyboard::Escape, Action::Escape},
    {sf::Keyboard::Enter, Action::Enter},
    {sf::Keyboard::Space, Action::Space},
    {sf::Keyboard::Backspace, Action::Backspace},
    {sf::Keyboard::Tab, Action::Tab},
    {sf::Keyboard::Delete, Action::Delete},
    {sf::Keyboard::Insert, Action::Insert},
    {sf::Keyboard::Home, Action::Home},
    {sf::Keyboard::End, Action::End},
    {sf::Keyboard::PageUp, Action::PageUp},
    {sf::Keyboard::PageDown, Action::PageDown},
    {sf::Keyboard::Pause, Action::Pause},

    {sf::Keyboard::F1, Action::F1}, {sf::Keyboard::F2, Action::F2},
    {sf::Keyboard::F3, Action::F3}, {sf::Keyboard::F4, Action::F4},
    {sf::Keyboard::F5, Action::F5}, {sf::Keyboard::F6, Action::F6},
    {sf::Keyboard::F7, Action::F7}, {sf::Keyboard::F8, Action::F8},
    {sf::Keyboard::F9, Action::F9}, {sf::Keyboard::F10, Action::F10},
    {sf::Keyboard::F11, Action::F11}, {sf::Keyboard::F12, Action::F12},
    {sf::Keyboard::F13, Action::F13}, {sf::Keyboard::F14, Action::F14},
    {sf::Keyboard::F15, Action::F15},

    {sf::Keyboard::LControl, Action::LControl},
    {sf::Keyboard::LShift, Action::LShift},
    {sf::Keyboard::LAlt, Action::LAlt},
    {sf::Keyboard::LSystem, Action::LSystem},
    {sf::Keyboard::RControl, Action::RControl},
    {sf::Keyboard::RShift, Action::RShift},
    {sf::Keyboard::RAlt, Action::RAlt},
    {sf::Keyboard::RSystem, Action::RSystem},

    {sf::Keyboard::LBracket, Action::LBracket},
    {sf::Keyboard::RBracket, Action::RBracket},
    {sf::Keyboard::Semicolon, Action::Semicolon},
    {sf::Keyboard::Comma, Action::Comma},
    {sf::Keyboard::Period, Action::Period},
    {sf::Keyboard::Quote, Action::Apostrophe},
    {sf::Keyboard::Slash, Action::Slash},
    {sf::Keyboard::Backslash, Action::Backslash},
    {sf::Keyboard::Tilde, Action::Grave},
    {sf::Keyboard::Equal, Action::Equal},
    {sf::Keyboard::Hyphen, Action::Hyphen},

    {sf::Keyboard::Numpad0, Action::Numpad0},
    {sf::Keyboard::Numpad1, Action::Numpad1},
    {sf::Keyboard::Numpad2, Action::Numpad2},
    {sf::Keyboard::Numpad3, Action::Numpad3},
    {sf::Keyboard::Numpad4, Action::Numpad4},
    {sf::Keyboard::Numpad5, Action::Numpad5},
    {sf::Keyboard::Numpad6, Action::Numpad6},
    {sf::Keyboard::Numpad7, Action::Numpad7},
    {sf::Keyboard::Numpad8, Action::Numpad8},
    {sf::Keyboard::Numpad9, Action::Numpad9},
    {sf::Keyboard::Add, Action::Add},
    {sf::Keyboard::Subtract, Action::Subtract},
    {sf::Keyboard::Multiply, Action::Multiply},
    {sf::Keyboard::Divide, Action::Divide},
};

const std::unordered_map<sf::Mouse::Button, arc::Action> arc::SfmlDisplay::MOUSE_BUTTON_MAP = {
    {sf::Mouse::Left, Action::LeftMouse},
    {sf::Mouse::Right, Action::RightMouse},
    {sf::Mouse::Middle, Action::MiddleMouse},
};

arc::SfmlDisplay::SfmlDisplay()
    : _window(sf::VideoMode(WIN_W, WIN_H), "Arcade"),
      _hasFont(false)
{
    _window.setFramerateLimit(60);
    _hasFont = _font.loadFromFile("assets/menu/font/SpaceMono-Regular.ttf")
        || _font.loadFromFile("assets/fonts/font.ttf");
}

arc::SfmlDisplay::~SfmlDisplay()
{
    if (_window.isOpen())
        _window.close();
}

arc::Action arc::SfmlDisplay::mapKey(int key) const noexcept
{
    auto it = KEY_MAP.find(key);

    if (it != KEY_MAP.end())
        return it->second;
    return Action::None;
}

arc::Action arc::SfmlDisplay::mapMouseButton(sf::Mouse::Button btn) const noexcept
{
    auto it = MOUSE_BUTTON_MAP.find(btn);
    if (it != MOUSE_BUTTON_MAP.end())
        return it->second;
    return Action::None;
}

arc::Event arc::SfmlDisplay::buildMouseEvent(const sf::Event &ev) const noexcept
{
    float mx = static_cast<float>(ev.mouseButton.x) / static_cast<float>(WIN_W);
    float my = (static_cast<float>(ev.mouseButton.y) - static_cast<float>(HUD_H))
        / static_cast<float>(WIN_H - HUD_H);
    mx = std::max(0.f, std::min(1.f, mx));
    my = std::max(0.f, std::min(1.f, my));
    return {mapMouseButton(ev.mouseButton.button), {mx, my}};
}

arc::Event arc::SfmlDisplay::getEvent() noexcept
{
    sf::Event ev;

    while (_window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed)
            return {Action::Close, {0.f, 0.f}};
        if (ev.type == sf::Event::KeyPressed)
            return {mapKey(static_cast<int>(ev.key.code)), {0.f, 0.f}};
        if (ev.type == sf::Event::MouseButtonPressed)
            return buildMouseEvent(ev);
    }
    return {Action::None, {0.f, 0.f}};
}

int arc::SfmlDisplay::setAssets(const Assets assets) noexcept
{
    _textures.clear();
    _sounds.clear();
    _soundBuffers.clear();

    for (const auto &path : assets.first) {
        sf::Texture tex;
        if (!tex.loadFromFile(path + ".png"))
            tex.loadFromFile(path + ".jpg");
        _textures.push_back(std::move(tex));
    }

    _soundBuffers.resize(assets.second.size());
    _sounds.resize(assets.second.size());
    for (std::size_t i = 0; i < assets.second.size(); ++i) {
        if (_soundBuffers[i].loadFromFile(assets.second[i]))
            _sounds[i].setBuffer(_soundBuffers[i]);
    }
    return 0;
}

sf::Color arc::SfmlDisplay::toColor(arc::RGBA rgba) const noexcept
{
    auto [r, g, b, a] = rgba;
    return sf::Color(r, g, b, a);
}

void arc::SfmlDisplay::drawEntitySprite(
    const std::unique_ptr<IEntity> &entity,
    sf::Vector2f pos, sf::Vector2f size, int idx)
{
    auto texIdx = static_cast<std::size_t>(idx);
    auto texSize = _textures[texIdx].getSize();
    sf::Sprite sprite(_textures[texIdx]);
    sprite.setScale(size.x / texSize.x, size.y / texSize.y);
    sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
    sprite.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
    sprite.setRotation(entity->getRotation());
    sprite.setColor(toColor(entity->getColor()));
    _window.draw(sprite);
}

void arc::SfmlDisplay::drawEntityRect(
    const std::unique_ptr<IEntity> &entity,
    sf::Vector2f pos, sf::Vector2f size, int idx)
{
    auto color = toColor(entity->getColor());
    auto str = entity->getStr();
    bool hasText = str.has_value() && !str.value().empty();

    if (hasText && idx < 0) {
        if (!_hasFont)
            return;
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(14u);
        text.setFillColor(color);
        text.setString(str.value());
        text.setPosition(pos.x + 2.f, pos.y + 1.f);
        _window.draw(text);
    } else {
        sf::RectangleShape rect(size);
        rect.setOrigin(size.x / 2.f, size.y / 2.f);
        rect.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
        rect.setFillColor(color);
        rect.setRotation(entity->getRotation());
        _window.draw(rect);
        if (!_hasFont)
            return;
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(14u);
        text.setFillColor(sf::Color::Black);
        if (hasText)
            text.setString(str.value());
        else if (idx >= 0)
            text.setString(std::string(1, static_cast<char>(idx + 33)));
        text.setPosition(pos.x + 2.f, pos.y + 1.f);
        _window.draw(text);
    }
}

void arc::SfmlDisplay::drawEntity(const std::unique_ptr<IEntity> &entity)
{
    auto [px, py] = entity->getPos();
    auto [sw, sh] = entity->getSize();
    float availW = static_cast<float>(WIN_W);
    float availH = static_cast<float>(WIN_H - HUD_H);
    sf::Vector2f pos(
        px * availW,
        static_cast<float>(HUD_H) + py * availH);
    sf::Vector2f size(
        sw * availW,
        sh * availH);
    if (size.x < 1.f) size.x = 1.f;
    if (size.y < 1.f) size.y = 1.f;
    int idx = entity->getIdx();
    bool hasTexture = idx >= 0
        && idx < static_cast<int>(_textures.size())
        && _textures[static_cast<std::size_t>(idx)].getSize().x > 0;
    if (hasTexture)
        drawEntitySprite(entity, pos, size, idx);
    else
        drawEntityRect(entity, pos, size, idx);
}

void arc::SfmlDisplay::drawHud()
{
    sf::Vector2f barSize(
        static_cast<float>(WIN_W), static_cast<float>(HUD_H));
    sf::RectangleShape bar(barSize);
    bar.setFillColor(sf::Color(50, 50, 50));
    _window.draw(bar);
    if (!_hasFont)
        return;
    sf::Text text;
    text.setFont(_font);
    text.setCharacterSize(12u);
    text.setFillColor(sf::Color::White);
    text.setString(
        "[SFML] F2 = SwapDisplay F3 = SwapGame F4 = Restart ESC = Quit");
    text.setPosition(5.f, 7.f);
    _window.draw(text);
}

void arc::SfmlDisplay::handleSounds(const Sounds &sounds)
{
    std::vector<bool> requested(_sounds.size(), false);
    for (const auto &s : sounds) {
        int idx = s->getIdx();
        if (idx < 0 || idx >= static_cast<int>(_sounds.size()))
            continue;
        auto &snd = _sounds[static_cast<std::size_t>(idx)];
        requested[static_cast<std::size_t>(idx)] = true;
        if (snd.getStatus() != sf::Sound::Playing) {
            snd.setLoop(s->isLoop());
            snd.play();
        }
    }
    for (std::size_t i = 0; i < _sounds.size(); ++i) {
        if (!requested[i] && _sounds[i].getLoop()
            && _sounds[i].getStatus() == sf::Sound::Playing)
            _sounds[i].stop();
    }
}

void arc::SfmlDisplay::drawGame(
    const std::pair<Entities, Sounds> data) noexcept
{
    const auto &[entities, sounds] = data;
    _window.clear(sf::Color::Black);
    drawHud();
    for (const auto &entity : entities)
        drawEntity(entity);
    _window.display();
    handleSounds(sounds);
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
extern "C"
{
    std::unique_ptr<arc::IDisplayModule> makeInstance()
    {
        return std::make_unique<arc::SfmlDisplay>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Display;
    }
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
