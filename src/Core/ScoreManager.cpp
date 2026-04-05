/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** ScoreManager
*/

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Core/ScoreManager.hpp"

arc::ScoreManager::ScoreManager(const std::string &filePath)
    : _filePath(filePath)
{
    load();
}

arc::ScoreManager::~ScoreManager() noexcept
{
    save();
}

void arc::ScoreManager::load()
{
    std::ifstream file(_filePath);
    if (!file.is_open())
        return;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string game;
        std::string player;
        int score = 0;
        if (!std::getline(stream, game, '|'))
            continue;
        if (!std::getline(stream, player, '|'))
            continue;
        if (!(stream >> score))
            continue;
        _scores[game].emplace_back(player, score);
    }
}

void arc::ScoreManager::save() const noexcept
{
    std::ofstream file(_filePath);
    if (!file.is_open())
        return;
    for (const auto &[game, board] : _scores)
        for (const auto &[player, score] : board)
            file << game << '|' << player << '|' << score << '\n';
}

void arc::ScoreManager::addScore(const std::string &game,
    const std::string &player, int score)
{
    auto &board = _scores[game];
    auto it = std::find_if(board.begin(), board.end(),
        [&player](const auto &entry) { return entry.first == player; });
    if (it == board.end()) {
        board.emplace_back(player, score);
        return;
    }
    if (score > it->second)
        it->second = score;
}

int arc::ScoreManager::getBestScore(const std::string &game,
    const std::string &player) const noexcept
{
    auto git = _scores.find(game);
    if (git == _scores.end())
        return 0;
    auto it = std::find_if(git->second.begin(), git->second.end(),
        [&player](const auto &entry) { return entry.first == player; });
    return it != git->second.end() ? it->second : 0;
}

std::vector<std::pair<std::string, int>>
    arc::ScoreManager::getLeaderboard(const std::string &game) const noexcept
{
    auto git = _scores.find(game);
    if (git == _scores.end())
        return {};
    auto board = git->second;
    std::sort(board.begin(), board.end(),
        [](const auto &a, const auto &b) { return a.second > b.second; });
    return board;
}
