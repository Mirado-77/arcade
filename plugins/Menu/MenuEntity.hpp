/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** MenuEntity
*/

#ifndef MENUENTITY_HPP
    #define MENUENTITY_HPP

    #include <optional>
    #include <string>
    #include <utility>
    #include "IEntity.hpp"

namespace arc {
    struct EntityConfig {
        int idx;
        std::string str;
        RGBA color;
        int gridW;
        int gridH;
        int sizeW = 1;
        int sizeH = 1;
    };

    class MenuEntity : public IEntity {
        public:
            MenuEntity(
                std::pair<int, int> pos,
                EntityConfig config);
            ~MenuEntity() override = default;

            int getIdx() noexcept override;
            Vector2f getPos() noexcept override;
            Vector2f getSize() noexcept override;
            float getRotation() noexcept override;
            RGBA getColor() noexcept override;
            std::optional<std::string> getStr() noexcept override;

        private:
            std::pair<int, int> _pos;
            EntityConfig _config;
    };
}

#endif
