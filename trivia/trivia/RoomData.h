#pragma once
#include <string>
enum class RoomStatus {CLOSED, STARTED, WAITING};
typedef struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	RoomStatus status;
} RoomData;