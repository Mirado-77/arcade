/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** Snake
*/

#ifndef SNAKE_HPP
    #define SNAKE_HPP

    #include <deque>
    #include <random>
    #include <utility>
    #include "IGameModule.hpp"

namespace arc {
    class Snake : public IGameModule {
        public:
            Snake();
            ~Snake() override = default;

            void simulate(const Event event) noexcept override;
            Assets getAssets() noexcept override;
            std::pair<Entities, Sounds> getElements() noexcept override;
            bool isOpen() noexcept override;
            std::vector<Command> loadCommand() noexcept override;

        private:
            enum class Dir { Up, Down, Left, Right };

            static constexpr int WIDTH = 40;
            static constexpr int HEIGHT = 20;
            static constexpr int TICK_RATE = 8;

            std::deque<std::pair<int, int>> _body;
            std::pair<int, int> _food;
            Dir _dir;
            Dir _nextDir;
            int _tick;
            int _score;
            bool _open;
            std::mt19937 _rng;

            void updateDirection(Action action) noexcept;
            void moveSnake();
            void placeFood();
            bool checkCollision(std::pair<int, int> head) const noexcept;
            bool isOnBody(std::pair<int, int> pos) const noexcept;

            static Dir dirBetween(
                std::pair<int, int> from,
                std::pair<int, int> to) noexcept;
            static float headRotation(Dir dir) noexcept;
            static float tailRotation(Dir dirToBody) noexcept;
            static float bodyRotation(Dir prevDir) noexcept;
            static bool isTurn(Dir dirToPrev, Dir dirToNext) noexcept;
            static float turnRotation(
                Dir dirToPrev, Dir dirToNext) noexcept;

            void addHead(arc::Entities &entities) const noexcept;
            void addBody(arc::Entities &entities) const noexcept;
            void addTail(arc::Entities &entities) const noexcept;
    };
}

#endif
