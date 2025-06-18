#pragma once

#include <vector>
#include <map>
#include "LoggedUser.h"
#include "IDatabase.h"
class Game {
public:
    Game(unsigned int gameId, const std::vector<Question>& questions, IDatabase* db);

    Question getQuestionForUser(const LoggedUser& user);
    void submitAnswer(const LoggedUser& user, unsigned int answer, unsigned int time);
    void removePlayer(const LoggedUser& user);
    void sumitGameStatsToDB(const LoggedUser& user);
    unsigned int getGameId() const;
    const std::map<LoggedUser, GameData>& getPlayers() const;

private:
    std::vector<Question> m_questions;
    std::map<LoggedUser, GameData> m_players;
    unsigned int m_gameId;
    IDatabase* m_database;
};