/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** CentipedeEntity
*/

#include "CentipedeEntity.hpp"

arc::CentipedeEntity::CentipedeEntity(
    std::pair<int, int> pos, int idx, std::string str, RGBA color,
    int gridW, int gridH, float rotation)
    : _pos(pos)
    , _idx(idx)
    , _str(std::move(str))
    , _color(color)
    , _gridW(gridW)
    , _gridH(gridH)
    , _rotation(rotation)
{}

int arc::CentipedeEntity::getIdx() noexcept
{
    return _idx;
}

arc::Vector2f arc::CentipedeEntity::getPos() noexcept
{
    return {
        static_cast<float>(_pos.first) / static_cast<float>(_gridW),
        static_cast<float>(_pos.second) / static_cast<float>(_gridH)
    };
}

arc::Vector2f arc::CentipedeEntity::getSize() noexcept
{
    return {
        1.0f / static_cast<float>(_gridW),
        1.0f / static_cast<float>(_gridH)
    };
}

float arc::CentipedeEntity::getRotation() noexcept
{
    return _rotation;
}

arc::RGBA arc::CentipedeEntity::getColor() noexcept
{
    return _color;
}

std::optional<std::string> arc::CentipedeEntity::getStr() noexcept
{
    return _str;
}
