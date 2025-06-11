#pragma once
#include "Room.h"
#include "LoggedUser.h"
#include <map>
#include <vector>
class RoomManager
{
public:
	void createRoom(const LoggedUser& user, const RoomData& roomData);
	void deleteRoom(const int roomId);
	Room& getRoom(const int roomId);
	RoomStatus getRoomState(const int roomId);
	std::vector<RoomData> getRooms();
private:
	std::map<unsigned int, Room> m_rooms;
};

