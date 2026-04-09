/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** NcursesDisplay
*/

#include "NcursesDisplay.hpp"
#include "ISound.hpp"
#include <algorithm>
#include <fstream>

const std::unordered_map<int, arc::Action> arc::NcursesDisplay::KEY_MAP = {
    {KEY_UP, Action::Up}, {KEY_DOWN, Action::Down},
    {KEY_LEFT, Action::Left}, {KEY_RIGHT, Action::Right},
    {27, Action::Escape},
    {KEY_BACKSPACE, Action::Backspace},
    {127, Action::Backspace},
    {'\n', Action::Enter},
    {'\r', Action::Enter},
    {' ', Action::Space},
    {'\t', Action::Tab},
    {KEY_DC, Action::Delete},
    {KEY_IC, Action::Insert},
    {KEY_HOME, Action::Home},
    {KEY_END, Action::End},
    {KEY_PPAGE, Action::PageUp},
    {KEY_NPAGE, Action::PageDown},
    {KEY_F(1), Action::F1}, {KEY_F(2), Action::F2},
    {KEY_F(3), Action::F3}, {KEY_F(4), Action::F4},
    {KEY_F(5), Action::F5}, {KEY_F(6), Action::F6},
    {KEY_F(7), Action::F7}, {KEY_F(8), Action::F8},
    {KEY_F(9), Action::F9}, {KEY_F(10), Action::F10},
    {KEY_F(11), Action::F11}, {KEY_F(12), Action::F12},
    {'a', Action::A}, {'b', Action::B}, {'c', Action::C},
    {'d', Action::D}, {'e', Action::E}, {'f', Action::F},
    {'g', Action::G}, {'h', Action::H}, {'i', Action::I},
    {'j', Action::J}, {'k', Action::K}, {'l', Action::L},
    {'m', Action::M}, {'n', Action::N}, {'o', Action::O},
    {'p', Action::P}, {'q', Action::Q}, {'r', Action::R},
    {'s', Action::S}, {'t', Action::T}, {'u', Action::U},
    {'v', Action::V}, {'w', Action::W}, {'x', Action::X},
    {'y', Action::Y}, {'z', Action::Z},
    {'A', Action::A}, {'B', Action::B}, {'C', Action::C},
    {'D', Action::D}, {'E', Action::E}, {'F', Action::F},
    {'G', Action::G}, {'H', Action::H}, {'I', Action::I},
    {'J', Action::J}, {'K', Action::K}, {'L', Action::L},
    {'M', Action::M}, {'N', Action::N}, {'O', Action::O},
    {'P', Action::P}, {'Q', Action::Q}, {'R', Action::R},
    {'S', Action::S}, {'T', Action::T}, {'U', Action::U},
    {'V', Action::V}, {'W', Action::W}, {'X', Action::X},
    {'Y', Action::Y}, {'Z', Action::Z},
    {'0', Action::Num0}, {'1', Action::Num1}, {'2', Action::Num2},
    {'3', Action::Num3}, {'4', Action::Num4}, {'5', Action::Num5},
    {'6', Action::Num6}, {'7', Action::Num7}, {'8', Action::Num8},
    {'9', Action::Num9},
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
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
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

arc::Action arc::NcursesDisplay::mapMouseButton(mmask_t bstate) const noexcept
{
    static const std::vector<std::pair<mmask_t, Action>> map = {
        {BUTTON1_PRESSED, Action::LeftMouse},
        {BUTTON2_PRESSED, Action::MiddleMouse},
        {BUTTON3_PRESSED, Action::RightMouse},
    };
    auto it = std::find_if(map.begin(), map.end(),
        [bstate](const auto &p) { return (bstate & p.first) != 0; });
    return it != map.end() ? it->second : Action::None;
}

arc::Event arc::NcursesDisplay::getMouseEvent() noexcept
{
    MEVENT ev;
    if (getmouse(&ev) != OK)
        return {Action::None, {0.f, 0.f}};
    float mx = (COLS > 0) ? static_cast<float>(ev.x) / static_cast<float>(COLS) : 0.f;
    float my = (LINES > 2) ? static_cast<float>(ev.y - 1) / static_cast<float>(LINES - 2) : 0.f;
    if (mx < 0.f) mx = 0.f;
    if (mx > 1.f) mx = 1.f;
    if (my < 0.f) my = 0.f;
    if (my > 1.f) my = 1.f;
    return {mapMouseButton(ev.bstate), {mx, my}};
}

arc::Event arc::NcursesDisplay::getEvent() noexcept
{
    int key = getch();
    if (key == ERR) {
        napms(16);
        return {Action::None, {0.f, 0.f}};
    }
    if (key == KEY_MOUSE)
        return getMouseEvent();
    return {mapKey(key), {0.f, 0.f}};
}

int arc::NcursesDisplay::setAssets(const Assets assets) noexcept
{
    _assets = assets;
    _textAssets.clear();
    for (const auto &path : assets.first) {
        std::ifstream file(path + ".txt");
        std::string line;
        if (file.is_open() && std::getline(file, line))
            _textAssets.push_back(line);
        else
            _textAssets.push_back("");
    }
    return 0;
}

std::string arc::NcursesDisplay::getDisplayStr(
    const std::unique_ptr<IEntity> &entity) const noexcept
{
    int idx = entity->getIdx();
    if (idx >= 0 && idx < static_cast<int>(_textAssets.size())
        && !_textAssets[static_cast<std::size_t>(idx)].empty())
        return _textAssets[static_cast<std::size_t>(idx)];
    auto str = entity->getStr();
    if (str.has_value())
        return str.value();
    return std::string(1, static_cast<char>(idx + 33));
}

void arc::NcursesDisplay::drawEntity(
    const std::unique_ptr<IEntity> &entity) const noexcept
{
    auto [px, py] = entity->getPos();
    auto color = entity->getColor();
    int col = static_cast<int>(px * static_cast<float>(COLS));
    int row = 1 + static_cast<int>(py * static_cast<float>(LINES - 2));
    int pair = selectColorPair(color);
    int idx = entity->getIdx();

    if (col < 0 || row < 1 || col >= COLS || row >= LINES)
        return;
    if (idx >= 0) {
        std::string s = getDisplayStr(entity);
        attron(COLOR_PAIR(pair));
        mvprintw(row, col, "%s", s.c_str());
        attroff(COLOR_PAIR(pair));
    } else {
        auto str = entity->getStr();
        if (str.has_value() && !str.value().empty()) {
            attron(COLOR_PAIR(pair));
            mvprintw(row, col, "%s", str.value().c_str());
            attroff(COLOR_PAIR(pair));
        } else {
            attron(COLOR_PAIR(pair) | A_REVERSE);
            mvprintw(row, col, " ");
            attroff(COLOR_PAIR(pair) | A_REVERSE);
        }
    }
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
    const std::pair<Entities, Sounds> data) noexcept
{
    const auto &[entities, sounds] = data;
    clear();
    for (const auto &entity : entities)
        drawEntity(entity);
    mvprintw(0, 0, "[ncurses] F2=SwapDisp F3=SwapGame F4=Restart ESC=Quit");
    refresh();
    (void)sounds;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
extern "C"
{
    std::unique_ptr<arc::IDisplayModule> makeInstance()
    {
        return std::make_unique<arc::NcursesDisplay>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Display;
    }
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
