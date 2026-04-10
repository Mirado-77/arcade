/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Centipede
*/

#ifndef CENTIPEDE_HPP
    #define CENTIPEDE_HPP

    #include <deque>
    #include <map>
    #include <random>
    #include <utility>
    #include <vector>
    #include "IGameModule.hpp"

namespace arc {
    class Centipede : public IGameModule {
        public:
            Centipede();
            ~Centipede() override = default;

            void simulate(const Event event) noexcept override;
            Assets getAssets() noexcept override;
            std::pair<Entities, Sounds> getElements() noexcept override;
            bool isOpen() noexcept override;
            std::vector<Command> loadCommand() noexcept override;

        private:
            struct CentiBody {
                std::deque<std::pair<int, int>> segs;
                int dir;
            };

            static constexpr int WIDTH = 40;
            static constexpr int HEIGHT = 25;
            static constexpr int PLAYER_TOP = 20;
            static constexpr int TICK_RATE = 6;
            static constexpr int CENTI_LENGTH = 12;
            static constexpr int MAX_CENTIPEDES = 20;
            static constexpr int OBSTACLE_HP = 5;
            static constexpr int OBSTACLE_COUNT = 20;

            std::pair<int, int> _player;
            std::pair<int, int> _bullet;
            bool _bulletActive;
            std::map<std::pair<int, int>, int> _obstacles;
            std::vector<CentiBody> _centipedes;
            int _spawnedCount;
            int _score;
            int _tick;
            bool _open;
            std::mt19937 _rng;

            void placeObstacles();
            void spawnCentipede();
            void handleInput(Action action) noexcept;
            void shoot() noexcept;
            void stepBullet() noexcept;
            void stepCentipedes() noexcept;
            void stepOneCentipede(CentiBody &c) noexcept;
            void removeEscapedCentipedes() noexcept;
            void checkBulletCentipedeHit() noexcept;
            void damageObstacle(std::pair<int, int> pos) noexcept;
            void hitCentipede(int ci, int si) noexcept;
            static CentiBody splitTail(const CentiBody &c, int si) noexcept;
            static void trimToHead(CentiBody &c, int si) noexcept;
            void checkPlayerCollision() noexcept;
            void checkWin() noexcept;

            bool isObstacle(std::pair<int, int> pos) const noexcept;
            bool isWall(std::pair<int, int> pos) const noexcept;
            bool isBlocked(std::pair<int, int> pos) const noexcept;

            void addPlayerEntity(Entities &entities) const noexcept;
            void addBulletEntity(Entities &entities) const noexcept;
            void addObstacleEntities(Entities &entities) const noexcept;
            void addCentipedeEntities(Entities &entities) const noexcept;

            int getScore() const;
    };
}

#endif
