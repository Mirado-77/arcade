/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** CentipedeEntity
*/

#ifndef CENTIPEDEENTITY_HPP
    #define CENTIPEDEENTITY_HPP

    #include <optional>
    #include <string>
    #include <utility>
    #include "IEntity.hpp"

namespace arc {
    class CentipedeEntity : public IEntity {
        public:
            CentipedeEntity(std::pair<int, int> pos, int idx,
                std::string str, RGBA color,
                int gridW, int gridH, float rotation = 0.0f);
            ~CentipedeEntity() override = default;

            int getIdx() noexcept override;
            Vector2f getPos() noexcept override;
            Vector2f getSize() noexcept override;
            float getRotation() noexcept override;
            RGBA getColor() noexcept override;
            std::optional<std::string> getStr() noexcept override;

        private:
            std::pair<int, int> _pos;
            int _idx;
            std::string _str;
            RGBA _color;
            int _gridW;
            int _gridH;
            float _rotation;
    };
}

#endif
