#pragma once
#include <string>
#include "json.hpp"

#include <iostream>
using json = nlohmann::json;
typedef struct LoginRequest
{
	std::string username;
	std::string password;
} LoginRequest;

typedef struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
} SignupRequest;

typedef struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int QuestionsCount;
	unsigned int answerTimeout;
} CreateRoomRequest;

typedef struct GetPlayerInRoomRequest
{
	unsigned int roomId;
} GetPlayerInRoomRequest;

typedef struct JoinRoomRequest
{
	unsigned int roomId;
} JoinRoomRequest;
typedef struct SumbitAnswerRequest
{
	unsigned int answerId;
} SumbitAnswerRequest;
class JsonRequestPacketDeserializer
{
public:
	
	static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
	static SignupRequest deserializeSignupRequest(const std::vector<unsigned char>& buffer);
	static GetPlayerInRoomRequest deserializeGetPlayerInRoomRequest(const std::vector<unsigned char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer);
	static SumbitAnswerRequest deserializeSumbitAnswerRequest(const std::vector<unsigned char>& buffer);
};

