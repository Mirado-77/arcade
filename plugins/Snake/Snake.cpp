/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Snake
*/

#include <memory>
#include <random>
#include "Snake.hpp"

namespace {
    using Pos = std::pair<int, int>;

    class SnakeEntity : public arc::IEntity {
        public:
            SnakeEntity(Pos pos, int idx, std::string str, arc::RGBA color)
                : _pos(pos)
                , _idx(idx)
                , _str(std::move(str))
                , _color(color)
            {}
            ~SnakeEntity() override = default;

            int getIdx() override { return _idx; }
            arc::Vector2f getPos() override
            {
                return {static_cast<float>(_pos.first),
                        static_cast<float>(_pos.second)};
            }
            arc::Vector2f getSize() override { return {1.0f, 1.0f}; }
            float getRotation() override { return 0.0f; }
            arc::RGBA getColor() override { return _color; }
            std::optional<std::string> getStr() override { return _str; }

        private:
            Pos _pos;
            int _idx;
            std::string _str;
            arc::RGBA _color;
    };
}

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

void arc::Snake::simulate(arc::Event event)
{
    updateDirection(event.first);
    _tick += 1;
    if (_tick < TICK_RATE)
        return;
    _tick = 0;
    moveSnake();
}

arc::Assets arc::Snake::getAssets()
{
    return {{}, {}};
}

std::pair<arc::Entities, arc::Sounds> arc::Snake::getElements()
{
    arc::Entities entities;
    arc::Sounds sounds;
    constexpr arc::RGBA headColor = {0, 255, 0, 255};
    constexpr arc::RGBA bodyColor = {0, 200, 0, 255};
    constexpr arc::RGBA foodColor = {255, 50, 50, 255};

    entities.push_back(
        std::make_unique<SnakeEntity>(_body.front(), 0, "S", headColor));
    for (std::size_t i = 1; i < _body.size(); ++i) {
        entities.push_back(
            std::make_unique<SnakeEntity>(_body[i], 1, "o", bodyColor));
    }
    entities.push_back(
        std::make_unique<SnakeEntity>(_food, 2, "*", foodColor));
    return {std::move(entities), std::move(sounds)};
}

bool arc::Snake::isOpen()
{
    return _open;
}

extern "C" arc::IGameModule *createGameModule()
{
    return new arc::Snake();
}

extern "C" void destroyGameModule(arc::IGameModule *module)
{
    delete module;
}

extern "C" int getModuleType()
{
    return 1;
}
