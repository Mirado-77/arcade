/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Core
*/

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include "Core/Core.hpp"
#include <iostream>

arc::Core::Core(const std::string &displayPath)
    : _root(execDir().parent_path()),
      _scores((_root / SCORE_FILE).string()),
      _displayIdx(0), _gameIdx(0)
{
    scanLibs();
    if (_displayLibs.empty())
        throw std::runtime_error(
            "no graphical library found in " + (_root / LIB_DIR).string());
    if (_gameLibs.empty())
        throw std::runtime_error(
            "no game library found in " + (_root / LIB_DIR).string());

    auto norm = std::filesystem::weakly_canonical(displayPath).string();
    auto it = std::find(_displayLibs.begin(), _displayLibs.end(), norm);
    if (it == _displayLibs.end())
        throw std::runtime_error(displayPath + ": not a graphical library");
    _displayIdx = static_cast<std::size_t>(
        std::distance(_displayLibs.begin(), it));
    loadDisplay(_displayLibs[_displayIdx]);
    auto menuPath = std::filesystem::weakly_canonical(
        _root / LIB_DIR / MENU_LIB).string();
    loadGame(menuPath);
}

std::filesystem::path arc::Core::execDir()
{
    return std::filesystem::weakly_canonical("/proc/self/exe");
}

void arc::Core::scanLibs()
{
    for (const auto &entry :
        std::filesystem::directory_iterator((_root / LIB_DIR).string())) {
        if (entry.path().extension() != ".so")
            continue;
        tryCategorizeLib(entry.path().string());
    }
}

void arc::Core::tryCategorizeLib(const std::string &path)
{
    try {
        DLLoader loader(path);
        auto getType = loader.getFunction<LibType()>("getLibType");
        auto norm = std::filesystem::weakly_canonical(path).string();
        LibType type = getType();
        if (type == LibType::Display)
            _displayLibs.push_back(norm);
        else if (type == LibType::Game)
            _gameLibs.push_back(norm);
    } catch (...) {}
}

void arc::Core::loadDisplay(const std::string &path)
{
    _display.reset();
    _displayLoader = std::make_unique<DLLoader>(path);
    auto getType = _displayLoader->getFunction<LibType()>("getLibType");
    if (getType() != LibType::Display)
        throw std::runtime_error(path + ": not a graphical library");
    auto makeInst = _displayLoader->getFunction<
        std::unique_ptr<IDisplayModule>()>("makeInstance");
    _display = makeInst();
    if (!_display)
        throw std::runtime_error("makeInstance: display factory returned nullptr");
}

void arc::Core::loadGame(const std::string &path)
{
    _game.reset();
    _gameLoader = std::make_unique<DLLoader>(path);
    auto getType = _gameLoader->getFunction<LibType()>("getLibType");
    if (getType() != LibType::Game)
        throw std::runtime_error(path + ": not a game library");
    auto makeInst = _gameLoader->getFunction<
        std::unique_ptr<IGameModule>()>("makeInstance");
    _game = makeInst();
    if (!_game)
        throw std::runtime_error("makeInstance: game factory returned nullptr");
    (void)_display->setAssets(_game->getAssets());
}

void arc::Core::nextDisplay()
{
    _displayIdx = (_displayIdx + 1) % _displayLibs.size();
    loadDisplay(_displayLibs[_displayIdx]);
    (void)_display->setAssets(_game->getAssets());
}

void arc::Core::nextGame()
{
    _gameIdx = (_gameIdx + 1) % _gameLibs.size();
    loadGame(_gameLibs[_gameIdx]);
}

void arc::Core::restartGame()
{
    auto makeInst = _gameLoader->getFunction<
        std::unique_ptr<IGameModule>()>("makeInstance");
    _game = makeInst();
    (void)_display->setAssets(_game->getAssets());
}

bool arc::Core::processEvent(const Event &event)
{
    if (event.first == Action::Escape || event.first == Action::Close)
        return true;
    if (event.first == Action::F2)
        nextDisplay();
    if (event.first == Action::F3)
        nextGame();
    if (event.first == Action::F4)
        restartGame();
    return false;
}

void arc::Core::handleLoadDisplay(const Command &cmd)
{
    auto it = std::find(_displayLibs.begin(), _displayLibs.end(), cmd.second[0]);
    if (it != _displayLibs.end())
        _displayIdx = static_cast<std::size_t>(it - _displayLibs.begin());
    loadDisplay(cmd.second[0]);
    (void)_display->setAssets(_game->getAssets());
}

void arc::Core::handleLoadGame(const Command &cmd)
{
    auto it = std::find(_gameLibs.begin(), _gameLibs.end(), cmd.second[0]);
    if (it != _gameLibs.end())
        _gameIdx = static_cast<std::size_t>(it - _gameLibs.begin());
    loadGame(cmd.second[0]);
}

void arc::Core::handleBackToMenu()
{
    auto menuPath = std::filesystem::weakly_canonical(
        _root / LIB_DIR / MENU_LIB).string();
    loadGame(menuPath);
}

void arc::Core::handleLoadScore(const Command &cmd)
{
    if (_currentPlayer.empty())
        return;
    auto gameName = std::filesystem::path(_gameLibs[_gameIdx]).stem().string();
    _scores.addScore(gameName, _currentPlayer, std::stoi(cmd.second[0]));
}

bool arc::Core::processCommand(const Command &cmd)
{
    if (cmd.first == Signal::Quit)
        return true;

    const bool hasArgs = !cmd.second.empty();

    if (cmd.first == Signal::LoadDisplay && hasArgs)
        handleLoadDisplay(cmd);
    if (cmd.first == Signal::LoadGame && hasArgs)
        handleLoadGame(cmd);
    if (cmd.first == Signal::RestartGame)
        restartGame();
    if (cmd.first == Signal::BackToMenu)
        handleBackToMenu();
    if (cmd.first == Signal::LoadUser && hasArgs)
        _currentPlayer = cmd.second[0];
    if (cmd.first == Signal::LoadScore && hasArgs)
        handleLoadScore(cmd);

    return false;
}

bool arc::Core::processCommands()
{
    for (const auto &cmd : _game->loadCommand()) {
        if (processCommand(cmd))
            return true;
    }
    return false;
}

arc::ScoreManager &arc::Core::getScores() noexcept
{
    return _scores;
}

void arc::Core::run()
{
    while (true) {
        if (!_game->isOpen()) {
            auto menuPath = std::filesystem::weakly_canonical(
                _root / LIB_DIR / MENU_LIB).string();
            loadGame(menuPath);
        }
        arc::Event event = _display->getEvent();
        if (processEvent(event))
            break;
        _game->simulate(event);
        if (processCommands())
            break;
        _display->drawGame(_game->getElements());
    }
}
