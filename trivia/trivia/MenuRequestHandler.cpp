#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& factory)
	: m_loggedUser(user), m_handlerFactory(factory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& request)
{
	return request.id >= LOGOUT_CODE && request.id <= PERSONAL_STATS_CODE;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo& request)
{
	std::cout << "MenuRequestHandler::handleRequest" << std::endl;
	switch (request.id)
	{
	case LOGOUT_CODE:
		return signout(request);
	case GET_ROOMS_CODE:
		return getRooms(request);
	case CREATE_ROOM_CODE:
		return createRoom(request);
	case JOIN_ROOM_CODE:
		return joinRoom(request);
	case GET_PLAYERS_IN_ROOM_CODE:
		return getPlayerInRoom(request);
	case PERSONAL_STATS_CODE:
		return getPersonalStats(request);
	case HIGH_SCORE_CODE:
		return getHighScores(request);
	//case CLOSE_ROOM_CODE:
	//	
	//case START_GAME_CODE:
	//	return 
	//case GET_ROOM_STATE_CODE:
	//	return 
	//case LEAVE_ROOM_CODE:
	//	return 
	default:
		return RequestResult{
			JsonResponsePacketSerializer::serializeResponse(
				ErrorResponse{ "Unknown menu request" }),
			nullptr
		};
	}
}

RequestResult MenuRequestHandler::signout(RequestInfo& request)
{
	auto rooms = m_handlerFactory.getRoomManager().getRooms();
	for (auto const& room : rooms)
	{
		m_handlerFactory.getRoomManager().getRoom(room.id).removeUser(m_loggedUser); // also disconnect from the rooms
	}
	m_handlerFactory.getLoginManager().logout(m_loggedUser.getUsername());
	LogoutResponse response{ LOGOUT_CODE };
	std::cout << "MenuRequestHandler::signout" << std::endl;
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		m_handlerFactory.createLoginRequestHandler()
	};
}

RequestResult MenuRequestHandler::getRooms(RequestInfo& request)
{
	// 1) fetch all RoomData
	auto rooms = m_handlerFactory.getRoomManager().getRooms();

	// 2) build a vector<nlohmann::json>
	std::vector<nlohmann::json> roomsJson;
	roomsJson.reserve(rooms.size());
	for (auto const& room : rooms)
	{
		nlohmann::json j;
		j["id"] = room.id;
		j["name"] = room.name;
		j["maxPlayers"] = room.maxPlayers;
		j["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
		j["timePerQuestion"] = room.timePerQuestion;
		roomsJson.push_back(j);
	}

	GetRoomResponse response{ 1, roomsJson };
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		this
	};
}

RequestResult MenuRequestHandler::createRoom(RequestInfo& request)
{
	auto req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(request.buffer);

	// generate a new unique ID *here*
	unsigned int newId = 1;
	auto existing = m_handlerFactory.getRoomManager().getRooms();
	for (auto const& rd : existing)
		newId = std::max(newId, rd.id + 1);

	RoomData data;
	data.id = newId;
	data.name = req.roomName;
	data.maxPlayers = req.maxUsers;
	data.numOfQuestionsInGame = req.QuestionsCount;
	data.timePerQuestion = req.answerTimeout;

	m_handlerFactory.getRoomManager().createRoom(m_loggedUser, data);

	CreateRoomResponse response{ 1 };
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		new RoomAdminRequestHandler(m_loggedUser, m_handlerFactory)
	};
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo& request)
{
	RequestResult res;
	auto req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(request.buffer);
	m_handlerFactory.getRoomManager().getRoom(req.roomId).addUser(m_loggedUser);

	JoinRoomResponse response{ 1 };
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		new RoomMemberRequestHandler(m_loggedUser, m_handlerFactory)
	};
}

RequestResult MenuRequestHandler::getPlayerInRoom(RequestInfo& request)
{
	auto req = JsonRequestPacketDeserializer::deserializeGetPlayerInRoomRequest(request.buffer);

	// getRoomState returns an unsigned int, so use getRoom(...) to fetch the Room,
	// then pull its players list. We're assuming Room has a getPlayers() method.
	Room& roomRef = m_handlerFactory.getRoomManager().getRoom(req.roomId);
	auto const users = roomRef.getUsers();

	GetPlayersInRoomResponse response{ users };
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		this
	};
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo& request)
{
	auto stats = m_handlerFactory
		.getStatisticsManager()
		.getUserStatistics(m_loggedUser.getUsername());

	getPersonalStatsResponse response{ 1, stats };
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		this
	};
}

RequestResult MenuRequestHandler::getHighScores(RequestInfo& request)
{
	auto scores = m_handlerFactory.getStatisticsManager().getHighScore();
	getHighScoresResponse response{ 1, scores };
	return RequestResult{
		JsonResponsePacketSerializer::serializeResponse(response),
		this
	};
}
