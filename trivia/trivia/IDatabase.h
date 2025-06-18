#pragma once

#include <string>
#include <vector>
#include <list>
#include "GameData.h"

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual int doesUserExist(const std::string& username) = 0;
    virtual int doesPasswordMatch(const std::string& username, const std::string& password) = 0;
    virtual int addNewUser(const std::string& username, const std::string& password, const std::string& mail) = 0;
    virtual std::list<Question> getQuestions(int amount) = 0;
    virtual float getPlayerAverageAnswerTime(const std::string& username) = 0;
    virtual int getNumOfCorrectAnswers(const std::string& username) = 0;
    virtual int getNumOfTotalAnswers(const std::string& username) = 0;
    virtual int getNumOfPlayerGames(const std::string& username) = 0;
    virtual int getPlayerScore(const std::string& username) = 0;
    virtual std::vector<std::string> getHighScores() = 0;
    virtual int submitGameStatistics(const GameData& data) = 0;
};