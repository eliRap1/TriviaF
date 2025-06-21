#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "StatisticsManager.h"
#include "RoomManager.h"
#include "GameManager.h"

class MenuRequestHandler;
class LoginRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* database);

	IRequestHandler* createGameRequestHandler(const LoggedUser& user, const Room& room);
	IRequestHandler* createLoginRequestHandler();
	IRequestHandler* createMenuRequestHandler(const LoggedUser& user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user);

	LoginManager& getLoginManager();
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	GameManager& getGameManager();

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_StatisticsManager;
	GameManager m_gameManager;
};