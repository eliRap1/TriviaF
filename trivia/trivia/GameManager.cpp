#include "GameManager.h"
#include <algorithm>

GameManager::GameManager(IDatabase* db)
    : m_database(db) {}

Game* GameManager::createGame(const Room& room)
{
	// Try to find an existing game for this room
	for (Game& game : m_games)
	{
		if (game.getGameId() == room.getRoomData().id)
		{
			std::cout << "Found existing game for room " << room.getRoomData().id << std::endl;
			return &game;
		}
	}

	std::list<Question> questions = m_database->getQuestions(room.getRoomData().numOfQuestionsInGame);
	std::vector<Question> questionVec(questions.begin(), questions.end());
	Game newGame(room.getRoomData().id, questionVec, m_database);
	m_games.push_back(std::move(newGame));
	return &m_games.back();
}


void GameManager::deleteGame(unsigned int gameId) {
    m_games.erase(std::remove_if(m_games.begin(), m_games.end(),
        [gameId](const Game& g) {
            return g.getGameId() == gameId;
        }), m_games.end());
}