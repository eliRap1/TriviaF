#include "Game.h"

Game::Game(unsigned int gameId, const std::vector<Question>& questions, IDatabase* db)
    : m_gameId(gameId), m_questions(questions), m_database(db) {}

Question Game::getQuestionForUser(const LoggedUser& user) {
    auto& data = m_players[user];

    if (data.correctAnswerCount + data.wrongAnswerCount < m_questions.size()) {
        data.currentQuestion = m_questions[data.correctAnswerCount + data.wrongAnswerCount];
        return data.currentQuestion;
    }

    return Question("No more questions", { "N/A", "N/A", "N/A", "N/A" }, 0);
}

void Game::submitAnswer(const LoggedUser& user, unsigned int answer, unsigned int time) {
    auto& data = m_players[user];
    if (data.currentQuestion.getCorrectAnswerId() == answer) {
        data.correctAnswerCount++;
    }
    else {
        data.wrongAnswerCount++;
    }

    unsigned int totalAnswers = data.correctAnswerCount + data.wrongAnswerCount;
    data.averageAnswerTime = ((data.averageAnswerTime * (totalAnswers - 1)) + time) / totalAnswers;
}

void Game::removePlayer(const LoggedUser& user) {
    m_players.erase(user);
}

void Game::sumitGameStatsToDB(const LoggedUser& user) {
    if (m_database && m_players.count(user)) {
        const GameData& data = m_players[user];
        m_database->submitGameStatistics(data);
    }
}

unsigned int Game::getGameId() const
{
    return m_gameId;
}

const std::map<LoggedUser, GameData>& Game::getPlayers() const {
    return m_players;
}
