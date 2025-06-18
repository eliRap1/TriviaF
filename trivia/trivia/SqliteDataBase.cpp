#include "SqliteDataBase.h"
#include <iostream>

SqliteDataBase::SqliteDataBase(const std::string& dbFileName)
	: db(nullptr), m_dbFileName(dbFileName) {}

SqliteDataBase::~SqliteDataBase()
{
	close();
}

bool SqliteDataBase::open()
{
	if (sqlite3_open(m_dbFileName.c_str(), &db) != SQLITE_OK)
	{
		std::cerr << "Error opening DB: " << sqlite3_errmsg(db) << std::endl;
		return false;
	}
	createTables();
	return true;
}

bool SqliteDataBase::close()
{
	if (db)
	{
		sqlite3_close(db);
		db = nullptr;
	}
	return true;
}

void SqliteDataBase::createTables()
{
	const char* userTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            username TEXT PRIMARY KEY,
            password TEXT NOT NULL,
            email TEXT NOT NULL
        );
    )";

	const char* questionsTable = R"(
        CREATE TABLE IF NOT EXISTS questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            question TEXT NOT NULL,
            answer1 TEXT NOT NULL,
            answer2 TEXT NOT NULL,
            answer3 TEXT NOT NULL,
            answer4 TEXT NOT NULL,
            correct_answer INTEGER NOT NULL CHECK(correct_answer BETWEEN 1 AND 4)
        );
    )";

	const char* statisticsTable = R"(
        CREATE TABLE IF NOT EXISTS statistics (
            username TEXT NOT NULL,
            average_answer_time REAL DEFAULT 0,
            correct_answers INTEGER DEFAULT 0,
            total_answers INTEGER DEFAULT 0,
            games_played INTEGER DEFAULT 0,
            FOREIGN KEY(username) REFERENCES users(username)
        );
    )";

	char* errMsg = nullptr;

	if (sqlite3_exec(db, userTable, nullptr, nullptr, &errMsg) != SQLITE_OK)
	{
		std::cerr << "Create users table error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}

	if (sqlite3_exec(db, questionsTable, nullptr, nullptr, &errMsg) != SQLITE_OK)
	{
		std::cerr << "Create questions table error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}

	if (sqlite3_exec(db, statisticsTable, nullptr, nullptr, &errMsg) != SQLITE_OK)
	{
		std::cerr << "Create statistics table error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
}


int SqliteDataBase::doesUserExist(const std::string& username)
{
	sqlite3_stmt* stmt;
	const char* sql = "SELECT username FROM users WHERE username = ?;";
	bool exists = false;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		exists = (sqlite3_step(stmt) == SQLITE_ROW);
	}

	sqlite3_finalize(stmt);
	return exists;
}

int SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	sqlite3_stmt* stmt;
	const char* sql = "SELECT password FROM users WHERE username = ?;";
	bool match = false;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const char* dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			match = (password == dbPassword);
		}
	}

	sqlite3_finalize(stmt);
	return match;
}

int SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	sqlite3_stmt* stmt;
	const char* sql = "INSERT INTO users (username, password, email) VALUES (?, ?, ?);";

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);

		bool success = (sqlite3_step(stmt) == SQLITE_DONE);
		sqlite3_finalize(stmt);
		return success;
	}

	sqlite3_finalize(stmt);
	return false;
}

std::list<Question> SqliteDataBase::getQuestions(int amount)
{
	std::list<Question> questions;
	std::string sql = "SELECT question, answer1, answer2, answer3, answer4, correct_answer FROM questions LIMIT " + std::to_string(amount);
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::string questionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			std::vector<std::string> answers;
			for (int i = 0; i < 4; ++i)
			{
				answers.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i + 1)));
			}
			int correctAnswerIndex = sqlite3_column_int(stmt, 5) - 1;

			Question q(questionText, answers, correctAnswerIndex);
			questions.push_back(q);
		}
	}
	sqlite3_finalize(stmt);
	return questions;
}


int SqliteDataBase::getPlayerScore(const std::string& username)
{
	std::string sql = "SELECT correct_answers FROM statistics WHERE username = ?";
	sqlite3_stmt* stmt = nullptr;
	int score = 0;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW)
			score = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);
	return score;
}

std::vector<std::string> SqliteDataBase::getHighScores()
{
	std::vector<std::string> highscores;
	std::string sql = "SELECT username FROM statistics ORDER BY correct_answers DESC LIMIT 5";
	sqlite3_stmt* stmt = nullptr;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			highscores.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
		}
	}

	sqlite3_finalize(stmt);
	return highscores;
}

float SqliteDataBase::getPlayerAverageAnswerTime(const std::string& username)
{
	const char* sql = "SELECT AVG(answer_time) FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	float avg = 0;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			avg = static_cast<float>(sqlite3_column_double(stmt, 0));
		}
		sqlite3_finalize(stmt);
	}
	return avg;
}

int SqliteDataBase::getNumOfCorrectAnswers(const std::string& username)
{
	const char* sql = "SELECT COUNT(*) FROM statistics WHERE username = ? AND is_correct = 1;";
	sqlite3_stmt* stmt;
	int count = 0;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return count;
}

int SqliteDataBase::getNumOfTotalAnswers(const std::string& username)
{
	const char* sql = "SELECT COUNT(*) FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	int count = 0;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return count;
}

int SqliteDataBase::getNumOfPlayerGames(const std::string& username)
{
	const char* sql = "SELECT COUNT(DISTINCT game_id) FROM statistics WHERE username = ?;";
	sqlite3_stmt* stmt;
	int count = 0;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
	{
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return count;
}

int SqliteDataBase::submitGameStatistics(const GameData& data) {
	try {
		std::string sql = "INSERT INTO GameStatistics (correctAnswers, wrongAnswers, averageTime) VALUES (?, ?, ?);";
		sqlite3_stmt* stmt;
		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
			return 0;

		sqlite3_bind_int(stmt, 1, data.correctAnswerCount);
		sqlite3_bind_int(stmt, 2, data.wrongAnswerCount);
		sqlite3_bind_int(stmt, 3, data.averageAnswerTime);

		int rc = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return (rc == SQLITE_DONE) ? 1 : 0;
	}
	catch (...) {
		return 0;
	}
}

