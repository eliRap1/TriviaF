#pragma once
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "LoggedUser.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
    RoomMemberRequestHandler(const LoggedUser& user, RequestHandlerFactory& factory);
    bool isRequestRelevant(const RequestInfo& request) override;
    RequestResult handleRequest(RequestInfo& request) override;

private:
    LoggedUser m_loggedUser;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult leaveRoom(RequestInfo& request);
    RequestResult getRoomState(RequestInfo& request);
};
