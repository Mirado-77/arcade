/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SfmlDisplay
*/

#include "SfmlDisplay.hpp"
#include "ISound.hpp"

const std::unordered_map<int, arc::Action> arc::SfmlDisplay::KEY_MAP = {
    {sf::Keyboard::Up, Action::Up},
    {sf::Keyboard::Down, Action::Down},
    {sf::Keyboard::Left, Action::Left},
    {sf::Keyboard::Right, Action::Right},
    {sf::Keyboard::Escape, Action::Escape},
    {sf::Keyboard::F2, Action::F2},
    {sf::Keyboard::F3, Action::F3},
    {sf::Keyboard::F4, Action::F4},
    {sf::Keyboard::F5, Action::F5},
    {sf::Keyboard::Enter, Action::Enter},
    {sf::Keyboard::Space, Action::Space},
    {sf::Keyboard::Backspace, Action::Backspace},
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
};

arc::SfmlDisplay::SfmlDisplay()
    : _window(sf::VideoMode(WIN_W, WIN_H), "Arcade"),
      _hasFont(false)
{
    _window.setFramerateLimit(60);
}
// _hasFont = _font.loadFromFile(""); ill add it later, i just want to test the display for now

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

arc::Event arc::SfmlDisplay::getEvent()
{
    sf::Event ev;

    while (_window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed)
            return {Action::Close, {0.f, 0.f}};
        if (ev.type == sf::Event::KeyPressed)
            return {mapKey(static_cast<int>(ev.key.code)), {0.f, 0.f}};
    }
    return {Action::None, {0.f, 0.f}};
}

int arc::SfmlDisplay::setAssets(Assets)
{
    return 0;
}

sf::Color arc::SfmlDisplay::toColor(arc::RGBA rgba) const noexcept
{
    auto [r, g, b, a] = rgba;
    return sf::Color(r, g, b, a);
}

void arc::SfmlDisplay::drawEntity(const std::unique_ptr<IEntity> &entity)
{
    auto [x, y] = entity->getPos();
    sf::RectangleShape rect({CELL, CELL});

    rect.setPosition(x * CELL, static_cast<float>(HUD_H) + y * CELL);
    rect.setFillColor(toColor(entity->getColor()));
    _window.draw(rect);
    if (!_hasFont)
        return;
    sf::Text text;
    text.setFont(_font);
    text.setCharacterSize(14u);
    text.setFillColor(sf::Color::Black);
    auto str = entity->getStr();
    if (str.has_value())
        text.setString(str.value());
    else
        text.setString(std::string(1, static_cast<char>(entity->getIdx() + 33)));
    text.setPosition(x * CELL + 2.f, static_cast<float>(HUD_H) + y * CELL + 1.f);
    _window.draw(text);
}

void arc::SfmlDisplay::drawHud()
{
    sf::RectangleShape bar({static_cast<float>(WIN_W), static_cast<float>(HUD_H)});
    bar.setFillColor(sf::Color(50, 50, 50));
    _window.draw(bar);
    if (!_hasFont)
        return;
    sf::Text text;
    text.setFont(_font);
    text.setCharacterSize(12u);
    text.setFillColor(sf::Color::White);
    text.setString("[SFML] F2 = SwapDisplay F3 = SwapGame F4 = Restart ESC = Quit");
    text.setPosition(5.f, 7.f);
    _window.draw(text);
}

void arc::SfmlDisplay::drawGame(
    std::reference_wrapper<std::pair<Entities, Sounds>> data)
{
    auto &[entities, sounds] = data.get();
    _window.clear(sf::Color::Black);
    drawHud();
    for (const auto &entity : entities)
        drawEntity(entity);
    _window.display();
    (void)sounds;
}

extern "C" int getModuleType()
{
    return 0;
}

extern "C" arc::IDisplayModule *createDisplayModule()
{
    return new arc::SfmlDisplay();
}

extern "C" void destroyDisplayModule(arc::IDisplayModule *module)
{
    delete module;
}
