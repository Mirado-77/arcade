/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SnakeEntity
*/

#include "SnakeEntity.hpp"

arc::SnakeEntity::SnakeEntity(
    std::pair<int, int> pos, int idx, std::string str, RGBA color,
    int gridW, int gridH)
    : _pos(pos)
    , _idx(idx)
    , _str(std::move(str))
    , _color(color)
    , _gridW(gridW)
    , _gridH(gridH)
{}

int arc::SnakeEntity::getIdx() noexcept
{
    return _idx;
}

arc::Vector2f arc::SnakeEntity::getPos() noexcept
{
    return {
        static_cast<float>(_pos.first) / static_cast<float>(_gridW),
        static_cast<float>(_pos.second) / static_cast<float>(_gridH)
    };
}

arc::Vector2f arc::SnakeEntity::getSize() noexcept
{
    return {
        1.0f / static_cast<float>(_gridW),
        1.0f / static_cast<float>(_gridH)
    };
}

float arc::SnakeEntity::getRotation() noexcept
{
    return 0.0f;
}

arc::RGBA arc::SnakeEntity::getColor() noexcept
{
    return _color;
}

std::optional<std::string> arc::SnakeEntity::getStr() noexcept
{
    return _str;
}
