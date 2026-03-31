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

arc::Core::Core(const std::string &displayPath)
    : _displayIdx(0), _gameIdx(0)
{
    scanLibs();
    if (_displayLibs.empty())
        throw std::runtime_error("no graphical library found in ./lib");
    if (_gameLibs.empty())
        throw std::runtime_error("no game library found in ./lib");

    auto norm = std::filesystem::weakly_canonical(displayPath).string();
    auto it = std::find(_displayLibs.begin(), _displayLibs.end(), norm);
    if (it == _displayLibs.end())
        throw std::runtime_error(displayPath + ": not a graphical library");
    _displayIdx = static_cast<std::size_t>(
        std::distance(_displayLibs.begin(), it));
    loadDisplay(_displayLibs[_displayIdx]);
    loadGame(_gameLibs[_gameIdx]);
}

void arc::Core::scanLibs()
{
    for (const auto &entry :
        std::filesystem::directory_iterator(std::string(LIB_DIR))) {
        if (entry.path().extension() != ".so")
            continue;
        tryCategorizeLib(entry.path().string());
    }
}

void arc::Core::tryCategorizeLib(const std::string &path)
{
    try {
        DLLoader loader(path);
        auto getType = loader.getFunction<int()>("getModuleType");
        auto norm = std::filesystem::weakly_canonical(path).string();
        int type = getType();
        if (type == 0)
            _displayLibs.push_back(norm);
        else if (type == 1)
            _gameLibs.push_back(norm);
    } catch (...) {}
}

void arc::Core::loadDisplay(const std::string &path)
{
    _display.reset();
    _displayLoader = std::make_unique<DLLoader>(path);
    auto getType = _displayLoader->getFunction<int()>("getModuleType");
    if (getType() != 0)
        throw std::runtime_error(path + ": not a graphical library");
    _display = _displayLoader->loadPlugin<IDisplayModule>(
        "createDisplayModule", "destroyDisplayModule");
}

void arc::Core::loadGame(const std::string &path)
{
    _game.reset();
    _gameLoader = std::make_unique<DLLoader>(path);
    auto getType = _gameLoader->getFunction<int()>("getModuleType");
    if (getType() != 1)
        throw std::runtime_error(path + ": not a game library");
    _game = _gameLoader->loadPlugin<IGameModule>(
        "createGameModule", "destroyGameModule");
    _display->setAssets(_game->getAssets());
}

void arc::Core::nextDisplay()
{
    _displayIdx = (_displayIdx + 1) % _displayLibs.size();
    loadDisplay(_displayLibs[_displayIdx]);
    _display->setAssets(_game->getAssets());
}

void arc::Core::nextGame()
{
    _gameIdx = (_gameIdx + 1) % _gameLibs.size();
    loadGame(_gameLibs[_gameIdx]);
}

void arc::Core::restartGame()
{
    loadGame(_gameLibs[_gameIdx]);
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

void arc::Core::run()
{
    while (_game->isOpen()) {
        arc::Event event = _display->getEvent();
        if (processEvent(event))
            break;
        _game->simulate(event);
        auto elements = _game->getElements();
        _display->drawGame(std::ref(elements));
    }
}
