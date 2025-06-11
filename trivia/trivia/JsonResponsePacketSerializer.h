#pragma once
#include <string>
#include "json.hpp"
#include <iostream>
#include <vector>
using json = nlohmann::json;

// --- ENUMS FOR RESPONSE CODES ---
enum ResponseCode {
	LOGIN_RESPONSE = 1,
	SIGNUP_RESPONSE = 2,
	ERROR_CODE = 3,
	LOGOUT_CODE = 4,
	GET_ROOMS_CODE = 5,
	GET_PLAYERS_IN_ROOM_CODE = 6,
	JOIN_ROOM_CODE = 7,
	CREATE_ROOM_CODE = 8,
	HIGH_SCORE_CODE = 9,
	PERSONAL_STATS_CODE = 10,
	CLOSE_ROOM_CODE = 11,
	START_GAME_CODE = 12,
	GET_ROOM_STATE_CODE = 13,
	LEAVE_ROOM_CODE = 14,
	GET_GAME_RESULTS_CODE = 15,
	SUBMIT_ANSWER_CODE = 16,
	GET_QUESTION_CODE = 17,
};

// --- REQUEST STRUCTS ---

typedef struct LoginResponse {
	unsigned int status;
} LoginResponse;

typedef struct SignupResponse {
	unsigned int status;
} SignupResponse;


typedef struct LogoutResponse {
	unsigned int status;
} LogoutResponse;

typedef struct GetRoomResponse {
	unsigned int status;
	std::vector<json> rooms;
} GetRoomResponse;

typedef struct GetPlayersInRoomResponse {
	std::vector<std::string> players;
} GetPlayersInRoomResponse;

typedef struct JoinRoomResponse {
	unsigned int status;
} JoinRoomResponse;

typedef struct CreateRoomResponse {
	unsigned int status;
} CreateRoomResponse;

typedef struct getHighScoresResponse {
	unsigned int status;
	std::vector<std::string> highScores;
} getHighScoresResponse;

typedef struct getPersonalStatsResponse {
	unsigned int status;
	std::vector<std::string> statistics;
} getPersonalStatsResponse;

typedef struct ErrorResponse {
	std::string message;
} ErrorResponse;
typedef struct CloseRoomResponse {
	unsigned int status;
} CloseRoomResponse;

typedef struct StartGameResponse {
	unsigned int status;
} StartGameResponse;

typedef struct GetRoomStateResponse {
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
} GetRoomStateResponse;

typedef struct LeaveRoomResponse {
	unsigned int status;
} LeaveRoomResponse;
typedef struct PlayerResults {
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
} PlayerResults;

typedef struct GetGameResultsResponse {
	unsigned int status;
	std::vector<PlayerResults> results;
} GetGameResultsResponse;

typedef struct SubmitAnswerResponse {
	unsigned int status;
	unsigned int correctAnswerId;
} SubmitAnswerResponse;

typedef struct GetQuestionResponse {
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
} GetQuestionResponse;

typedef struct LeaveGameResponse {
	unsigned int status;
} LeaveGameResponse;

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeResponse(const ErrorResponse&);
	static std::vector<unsigned char> serializeResponse(const LoginResponse&);
	static std::vector<unsigned char> serializeResponse(const SignupResponse&);
	static std::vector<unsigned char> serializeResponse(const LogoutResponse&);
	static std::vector<unsigned char> serializeResponse(const GetRoomResponse&);
	static std::vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse&);
	static std::vector<unsigned char> serializeResponse(const JoinRoomResponse&);
	static std::vector<unsigned char> serializeResponse(const CreateRoomResponse&);
	static std::vector<unsigned char> serializeResponse(const getHighScoresResponse&);
	static std::vector<unsigned char> serializeResponse(const getPersonalStatsResponse&);
	static std::vector<unsigned char> serializeResponse(const CloseRoomResponse&);
	static std::vector<unsigned char> serializeResponse(const StartGameResponse&);
	static std::vector<unsigned char> serializeResponse(const GetRoomStateResponse&);
	static std::vector<unsigned char> serializeResponse(const LeaveRoomResponse&);
	static std::vector<unsigned char> serializeResponse(const GetGameResultsResponse&);
	static std::vector<unsigned char> serializeResponse(const SubmitAnswerResponse&);
	static std::vector<unsigned char> serializeResponse(const GetQuestionResponse&);
	static std::vector<unsigned char> serializeResponse(const LeaveGameResponse&);
};
