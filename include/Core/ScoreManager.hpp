/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** ScoreManager
*/

#ifndef SCOREMANAGER_HPP
    #define SCOREMANAGER_HPP

#include <map>
#include <string>
#include <vector>

namespace arc {
    class ScoreManager {
        public:
            ScoreManager(const std::string &filePath);
            ~ScoreManager() noexcept;

            void addScore(const std::string &game,
                const std::string &player, int score);
            int getBestScore(const std::string &game,
                const std::string &player) const noexcept;
            std::vector<std::pair<std::string, int>>
                getLeaderboard(const std::string &game) const noexcept;

        private:
            using Leaderboard = std::vector<std::pair<std::string, int>>;

            std::string _filePath;
            std::map<std::string, Leaderboard> _scores;

            void load();
            void save() const noexcept;
    };
}

#endif
