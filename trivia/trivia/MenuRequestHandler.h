#pragma once
#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
class MenuRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_loggedUser;
	RequestHandlerFactory& m_handlerFactory;

public:
	MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& factory);
	bool isRequestRelevant(const RequestInfo& request) override;
	RequestResult handleRequest(RequestInfo& request) override;
private:
	RequestResult signout(RequestInfo& request);
	RequestResult getRooms(RequestInfo& request);
	RequestResult createRoom(RequestInfo& request);
	RequestResult joinRoom(RequestInfo& request);
	RequestResult getPlayerInRoom(RequestInfo& request);
	RequestResult getPersonalStats(RequestInfo& request);
	RequestResult getHighScores(RequestInfo& request);
};

