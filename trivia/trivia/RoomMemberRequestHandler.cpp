#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, RequestHandlerFactory& factory)
    : m_loggedUser(user), m_handlerFactory(factory) {}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& request)
{
    return request.id == LEAVE_ROOM_CODE || request.id == GET_ROOM_STATE_CODE;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo& request)
{
    switch (request.id)
    {
    case LEAVE_ROOM_CODE:
        return leaveRoom(request);
    case GET_ROOM_STATE_CODE:
        return getRoomState(request);
    default:
        ErrorResponse err{ "Invalid request for RoomMember" };
        return { JsonResponsePacketSerializer::serializeResponse(err), this };
    }
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo& request)
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
		auto buffer = JsonResponsePacketSerializer::serializeResponse(err);
        //return { JsonResponsePacketSerializer::serializeResponse(err), this };
		return { buffer, m_handlerFactory.createMenuRequestHandler(m_loggedUser) };
    }

    m_handlerFactory.getRoomManager().getRoom(roomId).removeUser(m_loggedUser);

    LeaveRoomResponse response{ 1 };
    auto buffer = JsonResponsePacketSerializer::serializeResponse(response);

    return { buffer, m_handlerFactory.createMenuRequestHandler(m_loggedUser) };
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo& request)
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
