#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const LoggedUser& user, RequestHandlerFactory& factory)
    : m_loggedUser(user), m_handlerFactory(factory) {}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& request)
{
    return request.id == CLOSE_ROOM_CODE || request.id == START_GAME_CODE || request.id == GET_ROOM_STATE_CODE;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo& request)
{
    switch (request.id)
    {
    case CLOSE_ROOM_CODE:
        return closeRoom(request);
    case START_GAME_CODE:
        return startGame(request);
    case GET_ROOM_STATE_CODE:
        return getRoomState(request);
	//case LEAVE_ROOM_CODE:
	//	return leaveRoom(request);
    default:
        ErrorResponse err{ "Invalid request for RoomAdmin" };
        return { JsonResponsePacketSerializer::serializeResponse(err), this };
    }
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo& request)
{
    int roomId = -1;
    for (const auto& room : m_handlerFactory.getRoomManager().getRooms())
    {
        auto& r = m_handlerFactory.getRoomManager().getRoom(room.id);
        auto users = r.getUsers();
        if (std::find(users.begin(), users.end(), m_loggedUser.getUsername()) != users.end())
        {
            roomId = room.id;
            break;
        }
    }

    if (roomId == -1)
    {
        ErrorResponse err{ "Room not found for user" };
        return { JsonResponsePacketSerializer::serializeResponse(err), this };
    }
	/*for (const auto& user : m_handlerFactory.getRoomManager().getRoom(roomId).getUsers())
	{
		m_handlerFactory.getRoomManager().getRoom(roomId).removeUser(user);
	}*/
    auto& room = m_handlerFactory.getRoomManager().getRoom(roomId);
    room.setState(Room::RoomState::CLOSED);

    LeaveRoomResponse response{ 1 };
    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);

    m_handlerFactory.getRoomManager().deleteRoom(roomId);

    return { buffer, m_handlerFactory.createMenuRequestHandler(m_loggedUser) };
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo& request)
{
    int roomId = -1;
    for (const auto& room : m_handlerFactory.getRoomManager().getRooms())
    {
        auto& r = m_handlerFactory.getRoomManager().getRoom(room.id);
        auto users = r.getUsers();
        if (std::find(users.begin(), users.end(), m_loggedUser.getUsername()) != users.end())
        {
            roomId = room.id;
            break;
        }
    }

    if (roomId == -1)
    {
        ErrorResponse err{ "Room not found for user" };
        return { JsonResponsePacketSerializer::serializeResponse(err), this };
    }

    auto& room = m_handlerFactory.getRoomManager().getRoom(roomId);
    room.setState(Room::RoomState::STARTED);

    StartGameResponse response{ 1 };
    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);

    return { buffer, nullptr }; 
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo& request)
{
    int roomId = -1;
    for (const auto& room : m_handlerFactory.getRoomManager().getRooms())
    {
        auto& r = m_handlerFactory.getRoomManager().getRoom(room.id);
        auto users = r.getUsers();
        if (std::find(users.begin(), users.end(), m_loggedUser.getUsername()) != users.end())
        {
            roomId = room.id;
            break;
        }
    }

    if (roomId == -1)
    {
        ErrorResponse err{ "Room not found for user" };
        return { JsonResponsePacketSerializer::serializeResponse(err), this };
    }

    auto& room = m_handlerFactory.getRoomManager().getRoom(roomId);
    RoomData data = room.getRoomData();
    auto users = room.getUsers();
    bool started = room.getState() == Room::RoomState::STARTED;

    GetRoomStateResponse response{
        static_cast<int>(room.getState()),
        started,
        users,
        data.numOfQuestionsInGame,
        data.timePerQuestion
    };

    return { JsonResponsePacketSerializer::serializeResponse(response), this };
}
