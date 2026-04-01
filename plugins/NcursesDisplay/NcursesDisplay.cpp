/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** NcursesDisplay
*/

#include "NcursesDisplay.hpp"
#include "ISound.hpp"
#include <algorithm>

const std::unordered_map<int, arc::Action> arc::NcursesDisplay::KEY_MAP = {
    {KEY_UP, Action::Up},
    {KEY_DOWN, Action::Down},
    {KEY_LEFT, Action::Left},
    {KEY_RIGHT, Action::Right},
    {27, Action::Escape},
    {KEY_F(2), Action::F2},
    {KEY_F(3), Action::F3},
    {KEY_F(4), Action::F4},
    {KEY_F(5), Action::F5},
    {KEY_BACKSPACE, Action::Backspace},
    {'\n', Action::Enter},
    {' ', Action::Space},
    {'a', Action::A}, {'b', Action::B}, {'c', Action::C},
    {'d', Action::D}, {'e', Action::E}, {'f', Action::F},
    {'g', Action::G}, {'h', Action::H}, {'i', Action::I},
    {'j', Action::J}, {'k', Action::K}, {'l', Action::L},
    {'m', Action::M}, {'n', Action::N}, {'o', Action::O},
    {'p', Action::P}, {'q', Action::Q}, {'r', Action::R},
    {'s', Action::S}, {'t', Action::T}, {'u', Action::U},
    {'v', Action::V}, {'w', Action::W}, {'x', Action::X},
    {'y', Action::Y}, {'z', Action::Z},
};

arc::NcursesDisplay::NcursesDisplay()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    flushinp();
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_GREEN, -1);
        init_pair(2, COLOR_RED, -1);
        init_pair(3, COLOR_YELLOW, -1);
        init_pair(4, COLOR_BLUE, -1);
        init_pair(5, COLOR_CYAN, -1);
        init_pair(6, COLOR_MAGENTA, -1);
        init_pair(7, COLOR_WHITE, -1);
    }
}

arc::NcursesDisplay::~NcursesDisplay()
{
    endwin();
}

arc::Action arc::NcursesDisplay::mapKey(int key) const noexcept
{
    auto it = KEY_MAP.find(key);
    if (it != KEY_MAP.end())
        return it->second;
    return Action::None;
}

arc::Event arc::NcursesDisplay::getEvent()
{
    int key = getch();
    if (key == ERR) {
        napms(16);
        return {Action::None, {0.f, 0.f}};
    }
    return {mapKey(key), {0.f, 0.f}};
}

int arc::NcursesDisplay::setAssets(Assets)
{
    return 0;
}

void arc::NcursesDisplay::drawEntity(
    const std::unique_ptr<IEntity> &entity)
{
    auto [x, y] = entity->getPos();
    auto str = entity->getStr();
    auto color = entity->getColor();
    int col = static_cast<int>(x);
    int row = static_cast<int>(y);
    int pair = selectColorPair(color);

    attron(COLOR_PAIR(pair));
    if (str.has_value())
        mvprintw(row, col, "%s", str.value().c_str());
    else
        mvprintw(row, col, "%c", static_cast<char>(entity->getIdx() + 33));
    attroff(COLOR_PAIR(pair));
}

int arc::NcursesDisplay::selectColorPair(arc::RGBA color) const noexcept
{
    auto [r, g, b, a] = color;
    (void)a;
    if (g > r && g > b) return 1;
    if (r > g && r > b) return 2;
    if (r > 200 && g > 200) return 3;
    if (b > r && b > g) return 4;
    if (g > 200 && b > 200) return 5;
    if (r > 200 && b > 200) return 6;
    return 7;
}

void arc::NcursesDisplay::drawGame(
    std::reference_wrapper<std::pair<Entities, Sounds>> data)
{
    auto &[entities, sounds] = data.get();
    clear();
    for (const auto &entity : entities)
        drawEntity(entity);
    mvprintw(0, 0, "[ncurses] F2 = SwapDisplay F3 = SwapGame F4 = Restart ESC = Quit");
    refresh();
    (void)sounds;
}

extern "C" int getModuleType()
{
    return 0;
}

extern "C" arc::IDisplayModule *createDisplayModule()
{
    return new arc::NcursesDisplay();
}

extern "C" void destroyDisplayModule(arc::IDisplayModule *module)
{
    delete module;
}
