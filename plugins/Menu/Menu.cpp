/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Menu
*/

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include "Menu.hpp"
#include "MenuEntity.hpp"
#include "Core/DLLoader.hpp"

const arc::Menu::NavMap arc::Menu::LEFT_NAV = {
    {Focus::Displays, Focus::Username},
    {Focus::Username, Focus::Games},
    {Focus::Play, Focus::Username},
    {Focus::Games, Focus::Displays}
};

const arc::Menu::NavMap arc::Menu::RIGHT_NAV = {
    {Focus::Games, Focus::Username},
    {Focus::Username, Focus::Displays},
    {Focus::Play, Focus::Username},
    {Focus::Displays, Focus::Games},
};

arc::Menu::Menu()
    : _gameCursor(0)
    , _displayCursor(0)
    , _gameSelected(0)
    , _displaySelected(0)
    , _focus(Focus::Games)
    , _open(true)
{
    scanLibs();
    loadScores();
}

std::size_t arc::Menu::wrapDecrement(
    std::size_t idx, std::size_t size) noexcept
{
    return (idx == 0) ? size - 1 : idx - 1;
}

std::string arc::Menu::libFileName(
    const std::string &path)
{
    return std::filesystem::path(path)
        .filename().string();
}

void arc::Menu::categorizeLib(
    const std::string &path)
{
    try {
        DLLoader loader(path);
        auto getType =
            loader.getFunction<LibType()>(
                "getLibType");
        LibType type = getType();
        if (type == LibType::Game)
            _gameLibs.push_back(path);
        else if (type == LibType::Display)
            _displayLibs.push_back(path);
    } catch (...) {}
}

void arc::Menu::scanLibs()
{
    auto exePath = std::filesystem::weakly_canonical("/proc/self/exe")
        .parent_path();
    auto libDir = exePath / "lib";

    if (!std::filesystem::is_directory(libDir))
        return;
    for (const auto &entry :
        std::filesystem::directory_iterator(libDir))
    {
        if (entry.path().extension()
            != std::string(LIB_EXTENSION))
            continue;
        auto norm =
            std::filesystem::weakly_canonical(
                entry.path()).string();
        if (libFileName(norm) != "arcade_menu.so")
            categorizeLib(norm);
    }
    std::sort(_gameLibs.begin(), _gameLibs.end());
    std::sort(_displayLibs.begin(), _displayLibs.end());
}

void arc::Menu::moveCursorUp() noexcept
{
    switch (_focus) {
    case Focus::Play:
        _focus = Focus::Username;
        break;
    case Focus::Games:
        if (!_gameLibs.empty())
            _gameCursor = wrapDecrement(
                _gameCursor, _gameLibs.size());
        break;
    case Focus::Displays:
        if (!_displayLibs.empty())
            _displayCursor = wrapDecrement(
                _displayCursor,
                _displayLibs.size());
        break;
    default:
        break;
    }
}

void arc::Menu::moveCursorDown() noexcept
{
    switch (_focus) {
    case Focus::Username:
        _focus = Focus::Play;
        break;
    case Focus::Games:
        if (!_gameLibs.empty())
            _gameCursor =
                (_gameCursor + 1)
                % _gameLibs.size();
        break;
    case Focus::Displays:
        if (!_displayLibs.empty())
            _displayCursor =
                (_displayCursor + 1)
                % _displayLibs.size();
        break;
    default:
        break;
    }
}

void arc::Menu::handleVerticalNav(
    Action action) noexcept
{
    if (action == Action::Up)
        moveCursorUp();
    else if (action == Action::Down)
        moveCursorDown();
}

void arc::Menu::handleHorizontalNav(
    Action action) noexcept
{
    if (action != Action::Left
        && action != Action::Right)
        return;
    const auto &nav =
        (action == Action::Left)
        ? LEFT_NAV : RIGHT_NAV;
    auto it = nav.find(_focus);
    if (it != nav.end())
        _focus = it->second;
}

void arc::Menu::launchGame() noexcept
{
    if (!_username.empty())
        _pendingCommands.push_back(
            {Signal::LoadUser, {_username}});
    if (!_gameLibs.empty())
        _pendingCommands.push_back(
            {Signal::LoadGame,
                {_gameLibs[_gameSelected]}});
    if (!_displayLibs.empty())
        _pendingCommands.push_back(
            {Signal::LoadDisplay,
                {_displayLibs[_displaySelected]}});
}

void arc::Menu::handleEnter() noexcept
{
    switch (_focus) {
    case Focus::Games:
        if (!_gameLibs.empty())
            _gameSelected = _gameCursor;
        break;
    case Focus::Displays:
        if (!_displayLibs.empty())
            _displaySelected = _displayCursor;
        break;
    case Focus::Play:
        launchGame();
        break;
    default:
        break;
    }
}

void arc::Menu::handleLetter(
    Action action) noexcept
{
    if (_focus != Focus::Username)
        return;
    if (action == Action::Backspace
        && !_username.empty()) {
        _username.pop_back();
        return;
    }
    if (_username.size() >= MAX_NAME)
        return;
    if (action >= Action::A
        && action <= Action::Z) {
        auto offset = static_cast<int>(action)
            - static_cast<int>(Action::A);
        _username +=
            static_cast<char>('A' + offset);
    }
}

void arc::Menu::simulate(
    const Event event) noexcept
{
    auto action = event.first;

    if (action == Action::None)
        return;
    handleVerticalNav(action);
    handleHorizontalNav(action);
    if (action == Action::Enter)
        handleEnter();
    handleLetter(action);
}

arc::Assets arc::Menu::getAssets() noexcept
{
    return {
        {
            "assets/menu/background",
            "assets/menu/logo",
            "assets/menu/play",
        },
        {}
    };
}

arc::RGBA arc::Menu::itemColor(
    bool selected, bool cursor) const noexcept
{
    if (selected)
        return GREEN;
    if (cursor)
        return YELLOW;
    return GRAY;
}

void arc::Menu::buildColumnItem(
    Entities &entities,
    const ColumnConfig &col,
    Focus targetFocus,
    std::size_t i) const
{
    bool isCursor =
        (_focus == targetFocus && i == col.cursor);
    bool isSelected = (i == col.selected);
    auto prefix = isCursor ? "> " : "  ";
    auto label =
        prefix + libFileName(col.libs[i]);
    if (isSelected)
        label += " *";
    auto row =
        LIST_START_Y + static_cast<int>(i);
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{col.listX, row},
            EntityConfig{
                IDX_TEXT, label,
                itemColor(isSelected, isCursor),
                GRID_W, GRID_H,
                LIST_ITEM_W, 1}));
}

void arc::Menu::buildColumn(
    Entities &entities,
    const ColumnConfig &col,
    Focus targetFocus) const
{
    auto header = (_focus == targetFocus)
        ? "[ " + col.title + " ]"
        : "  " + col.title + "  ";
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{col.colX, GAMES_HEADER_Y},
            EntityConfig{IDX_TEXT, header,
                CYAN, GRID_W, GRID_H,
                HEADER_W, 1}));
    for (std::size_t i = 0;
        i < col.libs.size(); ++i)
        buildColumnItem(
            entities, col, targetFocus, i);
}

void arc::Menu::buildNameInput(
    Entities &entities) const
{
    bool focused = (_focus == Focus::Username);
    auto nameHeader = focused
        ? "[ Name ]" : "  Name  ";
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{CENTER_X, NAME_Y},
            EntityConfig{IDX_TEXT, nameHeader,
                CYAN, GRID_W, GRID_H,
                PLAY_W, 1}));
    std::string display = _username;
    while (display.size() < MAX_NAME)
        display += '_';
    auto color = focused ? YELLOW : WHITE;
    auto inputX = CENTER_X
        - static_cast<int>(MAX_NAME) / 20;
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{inputX, NAME_INPUT_Y},
            EntityConfig{IDX_TEXT, display, color,
                GRID_W, GRID_H,
                static_cast<int>(MAX_NAME), 1}));
}

void arc::Menu::buildPlayButton(
    Entities &entities) const
{
    bool canPlay =
        !_gameLibs.empty()
        && !_displayLibs.empty();
    RGBA playColor = DARK_GRAY;
    if (canPlay)
        playColor = (_focus == Focus::Play)
            ? GREEN : WHITE;
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{CENTER_X, PLAY_Y},
            EntityConfig{IDX_PLAY, "",
                playColor, GRID_W, GRID_H,
                PLAY_W, 1}));
}

void arc::Menu::loadScores()
{
    auto exePath =
        std::filesystem::weakly_canonical(
            "/proc/self/exe");
    auto scorePath = exePath.parent_path() / "scores.txt";
    std::ifstream file(scorePath);
    std::string line;

    _scores.clear();
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string game;
        std::string player;
        std::string scoreStr;
        if (!std::getline(iss, game, '|'))
            continue;
        if (!std::getline(iss, player, '|'))
            continue;
        if (!std::getline(iss, scoreStr))
            continue;
        _scores.emplace_back(
            player + " " + scoreStr
            + " (" + game + ")",
            std::stoi(scoreStr));
    }
    std::sort(_scores.begin(), _scores.end(),
        [](const auto &a, const auto &b) {
            return a.second > b.second;
        });
}

void arc::Menu::buildScores(
    Entities &entities) const
{
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{SCORES_X, SCORES_HEADER_Y},
            EntityConfig{IDX_TEXT, "Scores",
                CYAN, GRID_W, GRID_H,
                SCORES_W, 1}));
    auto count = std::min(
        static_cast<int>(_scores.size()),
        SCORES_MAX);
    for (int i = 0; i < count; ++i) {
        auto row =
            SCORES_START_Y + i;
        entities.push_back(
            std::make_unique<MenuEntity>(
                std::pair{SCORES_X, row},
                EntityConfig{IDX_TEXT,
                    _scores[i].first, GRAY,
                    GRID_W, GRID_H,
                    SCORES_W, 1}));
    }
}

std::pair<arc::Entities, arc::Sounds>
arc::Menu::getElements() noexcept
{
    Entities entities;

    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{0, 0},
            EntityConfig{IDX_BG, "", WHITE,
                GRID_W, GRID_H,
                GRID_W, GRID_H}));
    entities.push_back(
        std::make_unique<MenuEntity>(
            std::pair{LOGO_X, LOGO_Y},
            EntityConfig{IDX_LOGO, "", WHITE,
                GRID_W, GRID_H,
                LOGO_W, LOGO_H}));
    buildColumn(entities,
        {_gameLibs, "Games",
            _gameCursor, _gameSelected,
            GAMES_HEADER_X, GAMES_LIST_X},
        Focus::Games);
    buildNameInput(entities);
    buildScores(entities);
    buildPlayButton(entities);
    buildColumn(entities,
        {_displayLibs, "Displays",
            _displayCursor, _displaySelected,
            DISP_HEADER_X, DISP_LIST_X},
        Focus::Displays);
    Sounds sounds;
    return {std::move(entities), std::move(sounds)};
}

bool arc::Menu::isOpen() noexcept
{
    return _open;
}

std::vector<arc::Command>
arc::Menu::loadCommand() noexcept
{
    auto cmds = std::move(_pendingCommands);
    _pendingCommands.clear();
    return cmds;
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
extern "C"
{
    std::unique_ptr<arc::IGameModule> makeInstance()
    {
        return std::make_unique<arc::Menu>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Game;
    }
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
