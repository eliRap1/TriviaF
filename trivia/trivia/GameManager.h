#pragma once
#include <vector>
#include "Game.h"
#include "GameData.h"
#include "Room.h"

class GameManager {
public:
    GameManager(IDatabase* db);

    Game& createGame(const Room& room);
    void deleteGame(unsigned int gameId);

private:
    IDatabase* m_database;
    std::vector<Game> m_games;
    unsigned int m_nextGameId = 1;
};