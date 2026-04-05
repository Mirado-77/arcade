/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Snake
*/

#include <memory>
#include <random>
#include "Snake.hpp"
#include "SnakeEntity.hpp"

arc::Snake::Snake()
    : _dir(Dir::Right)
    , _nextDir(Dir::Right)
    , _tick(0)
    , _score(0)
    , _open(true)
    , _rng(std::random_device{}())
{
    _body.push_back({WIDTH / 2, HEIGHT / 2});
    _body.push_back({WIDTH / 2 - 1, HEIGHT / 2});
    _body.push_back({WIDTH / 2 - 2, HEIGHT / 2});
    placeFood();
}

void arc::Snake::updateDirection(arc::Action action) noexcept
{
    if (action == arc::Action::Up && _dir != Dir::Down)
        _nextDir = Dir::Up;
    if (action == arc::Action::Down && _dir != Dir::Up)
        _nextDir = Dir::Down;
    if (action == arc::Action::Left && _dir != Dir::Right)
        _nextDir = Dir::Left;
    if (action == arc::Action::Right && _dir != Dir::Left)
        _nextDir = Dir::Right;
}

void arc::Snake::moveSnake()
{
    _dir = _nextDir;
    int x = _body.front().first;
    int y = _body.front().second;
    if (_dir == Dir::Up) y -= 1;
    if (_dir == Dir::Down) y += 1;
    if (_dir == Dir::Left) x -= 1;
    if (_dir == Dir::Right) x += 1;
    std::pair<int, int> newHead = {x, y};
    if (checkCollision(newHead)) {
        _open = false;
        return;
    }
    _body.push_front(newHead);
    if (newHead == _food) {
        _score += 1;
        placeFood();
    } else {
        _body.pop_back();
    }
}

void arc::Snake::placeFood()
{
    std::uniform_int_distribution<int> distX(0, WIDTH - 1);
    std::uniform_int_distribution<int> distY(0, HEIGHT - 1);
    do {
        _food = {distX(_rng), distY(_rng)};
    } while (isOnBody(_food));
}

bool arc::Snake::checkCollision(std::pair<int, int> head) const noexcept
{
    if (head.first < 0 || head.first >= WIDTH)
        return true;
    if (head.second < 0 || head.second >= HEIGHT)
        return true;
    return isOnBody(head);
}

bool arc::Snake::isOnBody(std::pair<int, int> pos) const noexcept
{
    for (const auto &seg : _body) {
        if (seg == pos)
            return true;
    }
    return false;
}

void arc::Snake::simulate(const arc::Event event) noexcept
{
    updateDirection(event.first);
    _tick += 1;
    if (_tick < TICK_RATE)
        return;
    _tick = 0;
    moveSnake();
}

// Images (without extension) indexed by IEntity::getIdx()
// idx 0 = head, idx 1 = body, idx 2 = food
// Sounds (.wav with extension) indexed by ISound::getIdx()

arc::Assets arc::Snake::getAssets() noexcept
{
    return {
        {"assets/snake/head", "assets/snake/body", "assets/snake/food"},
        {}
    };
}

std::pair<arc::Entities, arc::Sounds> arc::Snake::getElements() noexcept
{
    arc::Entities entities;
    arc::Sounds sounds;
    constexpr arc::RGBA headColor = {0, 255, 0, 255};
    constexpr arc::RGBA bodyColor = {0, 200, 0, 255};
    constexpr arc::RGBA foodColor = {255, 50, 50, 255};

    entities.push_back(std::make_unique<arc::SnakeEntity>(
        _body.front(), 0, "S", headColor, WIDTH, HEIGHT));
    for (std::size_t i = 1; i < _body.size(); ++i) {
        entities.push_back(std::make_unique<arc::SnakeEntity>(
            _body[i], 1, "o", bodyColor, WIDTH, HEIGHT));
    }
    entities.push_back(std::make_unique<arc::SnakeEntity>(
        _food, 2, "*", foodColor, WIDTH, HEIGHT));
    return {std::move(entities), std::move(sounds)};
}

bool arc::Snake::isOpen() noexcept
{
    return _open;
}

std::vector<arc::Command> arc::Snake::loadCommand() noexcept
{
    return {};
}

extern "C"
{
    std::unique_ptr<arc::IGameModule> makeInstance()
    {
        return std::make_unique<arc::Snake>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Game;
    }
}
