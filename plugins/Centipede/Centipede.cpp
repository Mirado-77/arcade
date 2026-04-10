/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Centipede
*/

#include <memory>
#include <random>
#include "Centipede.hpp"
#include "CentipedeEntity.hpp"

arc::Centipede::Centipede()
    : _player({WIDTH / 2, HEIGHT - 3})
    , _bullet({0, 0})
    , _bulletActive(false)
    , _spawnedCount(0)
    , _score(0)
    , _tick(0)
    , _open(true)
    , _rng(std::random_device{}())
{
    placeObstacles();
    spawnCentipede();
}

void arc::Centipede::placeObstacles()
{
    std::uniform_int_distribution<int> distX(0, WIDTH - 1);
    std::uniform_int_distribution<int> distY(1, PLAYER_TOP - 1);
    int placed = 0;

    while (placed < OBSTACLE_COUNT) {
        std::pair<int, int> pos = {distX(_rng), distY(_rng)};
        if (_obstacles.find(pos) == _obstacles.end()) {
            _obstacles[pos] = OBSTACLE_HP;
            placed += 1;
        }
    }
}

void arc::Centipede::spawnCentipede()
{
    if (_spawnedCount >= MAX_CENTIPEDES)
        return;
    CentiBody c;
    c.dir = 1;
    for (int i = 0; i < CENTI_LENGTH; i++)
        c.segs.push_back({i, 0});
    _centipedes.push_back(std::move(c));
    _spawnedCount += 1;
}

void arc::Centipede::handleInput(arc::Action action) noexcept
{
    if (action == arc::Action::Left && _player.first > 0)
        _player.first -= 1;
    if (action == arc::Action::Right && _player.first < WIDTH - 1)
        _player.first += 1;
    if (action == arc::Action::Up && _player.second > PLAYER_TOP)
        _player.second -= 1;
    if (action == arc::Action::Down && _player.second < HEIGHT - 1)
        _player.second += 1;
    if (action == arc::Action::Space)
        shoot();
}

void arc::Centipede::shoot() noexcept
{
    if (_bulletActive)
        return;
    _bullet = {_player.first, _player.second - 1};
    _bulletActive = true;
}

bool arc::Centipede::isWall(std::pair<int, int> pos) const noexcept
{
    return pos.first < 0 || pos.first >= WIDTH;
}

bool arc::Centipede::isObstacle(std::pair<int, int> pos) const noexcept
{
    return _obstacles.find(pos) != _obstacles.end();
}

bool arc::Centipede::isBlocked(std::pair<int, int> pos) const noexcept
{
    return isWall(pos) || isObstacle(pos);
}

void arc::Centipede::stepOneCentipede(CentiBody &c) noexcept
{
    auto head = c.segs.front();
    std::pair<int, int> next = {head.first + c.dir, head.second};

    if (isBlocked(next)) {
        next = {head.first, head.second + 1};
        c.dir = -c.dir;
    }
    for (int i = static_cast<int>(c.segs.size()) - 1; i > 0; i--)
        c.segs[i] = c.segs[i - 1];
    c.segs.front() = next;
}

void arc::Centipede::removeEscapedCentipedes() noexcept
{
    std::size_t i = 0;

    while (i < _centipedes.size()) {
        if (_centipedes[i].segs.front().second >= HEIGHT) {
            _score -= 10;
            _centipedes.erase(_centipedes.begin() + i);
        } else {
            i += 1;
        }
    }
}

void arc::Centipede::stepCentipedes() noexcept
{
    for (auto &c : _centipedes)
        stepOneCentipede(c);
    removeEscapedCentipedes();
}

void arc::Centipede::damageObstacle(std::pair<int, int> pos) noexcept
{
    _obstacles[pos] -= 1;
    if (_obstacles[pos] <= 0)
        _obstacles.erase(pos);
}

arc::Centipede::CentiBody arc::Centipede::splitTail(
    const CentiBody &c, int si) noexcept
{
    CentiBody tail;

    tail.dir = -c.dir;
    for (int i = si + 1; i < static_cast<int>(c.segs.size()); i++)
        tail.segs.push_back(c.segs[i]);
    return tail;
}

void arc::Centipede::trimToHead(CentiBody &c, int si) noexcept
{
    while (static_cast<int>(c.segs.size()) > si)
        c.segs.pop_back();
}

void arc::Centipede::hitCentipede(int ci, int si) noexcept
{
    _score += 10;
    _obstacles[_centipedes[ci].segs[si]] = OBSTACLE_HP;
    CentiBody tail = splitTail(_centipedes[ci], si);
    trimToHead(_centipedes[ci], si);
    if (_centipedes[ci].segs.empty())
        _centipedes.erase(_centipedes.begin() + ci);
    if (!tail.segs.empty())
        _centipedes.push_back(std::move(tail));
}

void arc::Centipede::checkBulletCentipedeHit() noexcept
{
    for (int ci = 0; ci < static_cast<int>(_centipedes.size()); ci++) {
        for (int si = 0; si < static_cast<int>(_centipedes[ci].segs.size()); si++) {
            if (_centipedes[ci].segs[si] == _bullet) {
                hitCentipede(ci, si);
                _bulletActive = false;
                return;
            }
        }
    }
}

void arc::Centipede::stepBullet() noexcept
{
    if (!_bulletActive)
        return;
    _bullet.second -= 1;
    if (_bullet.second < 0) {
        _bulletActive = false;
        return;
    }
    if (isObstacle(_bullet)) {
        damageObstacle(_bullet);
        _bulletActive = false;
        return;
    }
    checkBulletCentipedeHit();
}

void arc::Centipede::checkPlayerCollision() noexcept
{
    for (const auto &c : _centipedes) {
        for (const auto &seg : c.segs) {
            if (seg == _player) {
                _open = false;
                return;
            }
        }
    }
}

void arc::Centipede::checkWin() noexcept
{
    if (!_centipedes.empty())
        return;
    if (_spawnedCount >= MAX_CENTIPEDES) {
        _obstacles.clear();
        placeObstacles();
        _spawnedCount = 0;
    }
    spawnCentipede();
}

void arc::Centipede::simulate(const arc::Event event) noexcept
{
    handleInput(event.first);
    stepBullet();
    _tick += 1;
    if (_tick < TICK_RATE)
        return;
    _tick = 0;
    stepCentipedes();
    checkPlayerCollision();
    checkWin();
}

// Images (without extension) indexed by IEntity::getIdx()
// idx 0 = player, idx 1 = bullet, idx 2 = centipede head,
// idx 3 = centipede body, idx 4 = obstacle

arc::Assets arc::Centipede::getAssets() noexcept
{
    return {
        {
            "assets/centipede/player",
            "assets/centipede/bullet",
            "assets/centipede/head",
            "assets/centipede/body",
            "assets/centipede/obstacle",
        },
        {}
    };
}

int arc::Centipede::getScore() const
{
    return _score;
}

void arc::Centipede::addPlayerEntity(arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA playerColor = {0, 200, 255, 255};

    entities.push_back(std::make_unique<CentipedeEntity>(
        _player, 0, "P", playerColor, WIDTH, HEIGHT));
}

void arc::Centipede::addBulletEntity(arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA bulletColor = {255, 255, 0, 255};

    if (!_bulletActive)
        return;
    entities.push_back(std::make_unique<CentipedeEntity>(
        _bullet, 1, "|", bulletColor, WIDTH, HEIGHT));
}

void arc::Centipede::addObstacleEntities(
    arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA obstacleColor = {150, 100, 50, 255};

    for (const auto &entry : _obstacles) {
        entities.push_back(std::make_unique<CentipedeEntity>(
            entry.first, 4, "#", obstacleColor, WIDTH, HEIGHT));
    }
}

void arc::Centipede::addCentipedeEntities(
    arc::Entities &entities) const noexcept
{
    constexpr arc::RGBA headColor = {255, 50, 50, 255};
    constexpr arc::RGBA bodyColor = {200, 50, 50, 255};

    for (const auto &c : _centipedes) {
        if (c.segs.empty())
            continue;
        entities.push_back(std::make_unique<CentipedeEntity>(
            c.segs.front(), 2, "H", headColor, WIDTH, HEIGHT));
        for (std::size_t i = 1; i < c.segs.size(); i++) {
            entities.push_back(std::make_unique<CentipedeEntity>(
                c.segs[i], 3, "o", bodyColor, WIDTH, HEIGHT));
        }
    }
}

std::pair<arc::Entities, arc::Sounds> arc::Centipede::getElements() noexcept
{
    arc::Entities entities;
    arc::Sounds sounds;

    addPlayerEntity(entities);
    addBulletEntity(entities);
    addObstacleEntities(entities);
    addCentipedeEntities(entities);
    return {std::move(entities), std::move(sounds)};
}

bool arc::Centipede::isOpen() noexcept
{
    return _open;
}

std::vector<arc::Command> arc::Centipede::loadCommand() noexcept
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
        return std::make_unique<arc::Centipede>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Game;
    }
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
