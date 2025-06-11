#pragma once
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "LoggedUser.h"

class RoomAdminRequestHandler : public IRequestHandler
{
public:
    RoomAdminRequestHandler(const LoggedUser& user, RequestHandlerFactory& factory);
    bool isRequestRelevant(const RequestInfo& request) override;
    RequestResult handleRequest(RequestInfo& request) override;

private:
    LoggedUser m_loggedUser;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult closeRoom(RequestInfo& request);
    RequestResult startGame(RequestInfo& request);
    RequestResult getRoomState(RequestInfo& request);
};