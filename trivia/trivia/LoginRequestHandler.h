#pragma once
#include "IRequestHandler.h"
//#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#define LOGIN_CODE 101
#define SIGNUP_CODE 102
#define ERROR_CODE 999
class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	//LoginRequestHandler(IDatabase* database);
	RequestResult handleRequest(RequestInfo& request) override;
	bool isRequestRelevant(const RequestInfo& request) override;
private:
	RequestHandlerFactory& m_handlerFactory;
	RequestResult login(const RequestInfo& request);
	RequestResult signup(const RequestInfo& request);
};

