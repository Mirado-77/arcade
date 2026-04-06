/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** MenuEntity
*/

#include "MenuEntity.hpp"

arc::MenuEntity::MenuEntity(std::pair<int, int> pos, EntityConfig config)
    : _pos(pos), _config(std::move(config))
{}

int arc::MenuEntity::getIdx() noexcept
{
    return _config.idx;
}

arc::Vector2f arc::MenuEntity::getPos() noexcept
{
    return {
        static_cast<float>(_pos.first) / static_cast<float>(_config.gridW),
        static_cast<float>(_pos.second) / static_cast<float>(_config.gridH)
    };
}

arc::Vector2f arc::MenuEntity::getSize() noexcept
{
    return {
        static_cast<float>(_config.sizeW) / static_cast<float>(_config.gridW),
        static_cast<float>(_config.sizeH) / static_cast<float>(_config.gridH)
    };
}

float arc::MenuEntity::getRotation() noexcept
{
    return 0.0f;
}

arc::RGBA arc::MenuEntity::getColor() noexcept
{
    return _config.color;
}

std::optional<std::string> arc::MenuEntity::getStr() noexcept
{
    return _config.str;
}
