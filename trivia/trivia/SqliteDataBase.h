#pragma once
#include "IDatabase.h"
#include "sqlite3.h"

class SqliteDataBase : public IDatabase
{
public:
    SqliteDataBase(const std::string& dbFileName);
    ~SqliteDataBase();

    bool open() override;
    bool close() override;

    int doesUserExist(const std::string& username) override;
    int doesPasswordMatch(const std::string& username, const std::string& password) override;
    int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
    std::list<Question> getQuestions(int amount) override;
    int getPlayerScore(const std::string& username) override;
    std::vector<std::string> getHighScores() override;
    float getPlayerAverageAnswerTime(const std::string& username);
    int getNumOfCorrectAnswers(const std::string& username);
    int getNumOfTotalAnswers(const std::string& username);
    int getNumOfPlayerGames(const std::string& username);
    int submitGameStatistics(const GameData& data) override;

private:
	sqlite3* db;
	std::string m_dbFileName;

	void createTables();
};