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
// idx 0 = head, idx 1 = body, idx 2 = food, idx 3 = tail, idx 4 = turn
// Head/tail sprite face Left at 0 deg.
// Body sprite is horizontal at 0 deg.
// Turn sprite connects Right and Down at 0 deg (corner at top-left).
// All rotations are clockwise, in degrees.

arc::Snake::Dir arc::Snake::dirBetween(
    std::pair<int, int> from, std::pair<int, int> to) noexcept
{
    if (to.first > from.first) return Dir::Right;
    if (to.first < from.first) return Dir::Left;
    if (to.second > from.second) return Dir::Down;
    return Dir::Up;
}

float arc::Snake::headRotation(arc::Snake::Dir dir) noexcept
{
    if (dir == Dir::Left) return 0.0f;
    if (dir == Dir::Down) return 270.0f;
    if (dir == Dir::Right) return 180.0f;
    return 90.0f;
}

float arc::Snake::tailRotation(arc::Snake::Dir dirToBody) noexcept
{
    if (dirToBody == Dir::Left) return 0.0f;
    if (dirToBody == Dir::Up) return 90.0f;
    if (dirToBody == Dir::Right) return 180.0f;
    return 270.0f;
}

float arc::Snake::bodyRotation(arc::Snake::Dir prevDir) noexcept
{
    if (prevDir == Dir::Left || prevDir == Dir::Right)
        return 0.0f;
    return 90.0f;
}

bool arc::Snake::isTurn(
    arc::Snake::Dir dirToPrev, arc::Snake::Dir dirToNext) noexcept
{
    bool prevHoriz = (dirToPrev == Dir::Left || dirToPrev == Dir::Right);
    bool nextHoriz = (dirToNext == Dir::Left || dirToNext == Dir::Right);
    return prevHoriz != nextHoriz;
}

float arc::Snake::turnRotation(
    arc::Snake::Dir dirToPrev, arc::Snake::Dir dirToNext) noexcept
{
    auto isSet = [&](Dir a, Dir b) noexcept {
        return (dirToPrev == a && dirToNext == b)
            || (dirToPrev == b && dirToNext == a);
    };
    if (isSet(Dir::Right, Dir::Down)) return 0.0f;
    if (isSet(Dir::Down, Dir::Left)) return 90.0f;
    if (isSet(Dir::Left, Dir::Up)) return 180.0f;
    return 270.0f;
}

arc::Assets arc::Snake::getAssets() noexcept
{
    return {
        {
            "assets/snake/head",
            "assets/snake/body",
            "assets/snake/food",
            "assets/snake/tail",
            "assets/snake/turn",
        },
        {}
    };
}

int arc::Snake::getScore() const
{
    return _score;
}

std::pair<arc::Entities, arc::Sounds> arc::Snake::getElements() noexcept
{
    arc::Entities entities;
    arc::Sounds sounds;

    addHead(entities);
    addBody(entities);
    addTail(entities);
    entities.push_back(std::make_unique<arc::SnakeEntity>(
        _food, 2, "*", arc::RGBA{255, 50, 50, 255}, WIDTH, HEIGHT));
    return {std::move(entities), std::move(sounds)};
}

void arc::Snake::addHead(arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA headColor = {0, 255, 0, 255};

    entities.push_back(std::make_unique<arc::SnakeEntity>(
        _body.front(), 0, "S", headColor, WIDTH, HEIGHT,
        headRotation(_dir)));
}

void arc::Snake::addBody(arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA bodyColor = {0, 200, 0, 255};

    for (std::size_t i = 1; i + 1 < _body.size(); ++i) {
        Dir dirToPrev = dirBetween(_body[i], _body[i - 1]);
        Dir dirToNext = dirBetween(_body[i], _body[i + 1]);
        int idx = 0;
        float rot = 0.0f;
        if (isTurn(dirToPrev, dirToNext)) {
            idx = 4;
            rot = turnRotation(dirToPrev, dirToNext);
        } else {
            idx = 1;
            rot = bodyRotation(dirToPrev);
        }
        entities.push_back(std::make_unique<arc::SnakeEntity>(
            _body[i], idx, "o", bodyColor, WIDTH, HEIGHT, rot));
    }
}

void arc::Snake::addTail(arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA bodyColor = {0, 200, 0, 255};

    if (_body.size() <= 1)
        return;
    std::size_t tailIdx = _body.size() - 1;
    Dir dirToBody = dirBetween(_body[tailIdx], _body[tailIdx - 1]);
    entities.push_back(std::make_unique<arc::SnakeEntity>(
        _body.back(), 3, "o", bodyColor, WIDTH, HEIGHT,
        tailRotation(dirToBody)));
}

bool arc::Snake::isOpen() noexcept
{
    return _open;
}

std::vector<arc::Command> arc::Snake::loadCommand() noexcept
{
    if (!_open)
        return {
            {Signal::LoadScore, {std::to_string(_score)}},
            {Signal::BackToMenu, {}}
        };
    return {};
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
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
#ifdef __clang__
#pragma clang diagnostic pop
#endif
