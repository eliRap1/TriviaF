#include "GameManager.h"
#include <algorithm>

GameManager::GameManager(IDatabase* db)
    : m_database(db) {}

Game& GameManager::createGame(const Room& room) {
    std::list<Question> questions = m_database->getQuestions(room.getRoomData().numOfQuestionsInGame);
    std::vector<Question> questionVec(questions.begin(), questions.end());

    Game newGame(m_nextGameId++, questionVec, m_database);
    m_games.push_back(newGame);
    return m_games.back();
}

void GameManager::deleteGame(unsigned int gameId) {
    m_games.erase(std::remove_if(m_games.begin(), m_games.end(),
        [gameId](const Game& g) {
            return g.getGameId() == gameId;
        }), m_games.end());
}