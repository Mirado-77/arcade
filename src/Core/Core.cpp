/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Core
*/

#include <stdexcept>
#include "Core/Core.hpp"

arc::Core::Core(const std::string &displayPath)
{
    loadDisplay(displayPath);
    loadGame(std::string(DEFAULT_GAME));
}

void arc::Core::loadDisplay(const std::string &path)
{
    _displayLoader = std::make_unique<DLLoader>(path);
    auto getType = _displayLoader->getFunction<int()>("getModuleType");

    if (getType() != 0) {
        throw std::runtime_error(path + ": not a graphical library");
    }
    _display = _displayLoader->loadPlugin<IDisplayModule>(
        "createDisplayModule", "destroyDisplayModule");
}

void arc::Core::loadGame(const std::string &path)
{
    _gameLoader = std::make_unique<DLLoader>(path);
    auto getType = _gameLoader->getFunction<int()>("getModuleType");

    if (getType() != 1) {
        throw std::runtime_error(path + ": not a game library");
    }
    _game = _gameLoader->loadPlugin<IGameModule>(
        "createGameModule", "destroyGameModule");
    _display->setAssets(_game->getAssets());
}

bool arc::Core::handleCoreEvent(const Event &event) const noexcept
{
    return event.first == Action::Escape || event.first == Action::Close;
}

void arc::Core::run()
{
    while (_game->isOpen()) {
        arc::Event event = _display->getEvent();
        if (handleCoreEvent(event)) {
            break;
        }
        _game->simulate(event);
        auto elements = _game->getElements();
        _display->drawGame(std::ref(elements));
    }
}
