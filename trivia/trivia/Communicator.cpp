#include "Communicator.h"


void Communicator::bindAndListen()
{
	WSAInitializer wsaInit;
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
	{
		throw std::runtime_error("socket failed");
	}


	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	ULONG ip = INADDR_ANY;
	sa.sin_addr.s_addr = ip;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << PORT << std::endl;
	while (true)
	{
		SOCKET clientSocket = accept(this->m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Accept failed" << std::endl;
		}
		else
		{
			//IRequestHandler* newClient = new LoginRequestHandler();
			/*this->m_clients[clientSocket] = newClient;*/
			std::thread th(&Communicator::handleNewClients, this, clientSocket);
			th.detach();
			std::cout << "Client connected" << std::endl;
		}
	}
}


void Communicator::startHandleRequests()
{
	bindAndListen();
}

void Communicator::handleNewClients(const SOCKET& c)
{
	try
	{
		IRequestHandler* currentHandler = new LoginRequestHandler(this->m_handlerFactory);
		this->m_clients[c] = currentHandler;
		int clientIdleTime = 0;
		while (clientIdleTime < 500)
		{
			char checkBuf;
			int check = recv(c, &checkBuf, 1, MSG_PEEK);
			if (check == 0 || check == SOCKET_ERROR)
			{
				throw std::runtime_error("client disconnected");
			}
			// 1) recv header (1 byte id + 4 bytes size)
			unsigned char header[5];
			int bytesRead = recv(c, reinterpret_cast<char*>(header), 5, 0);
			++clientIdleTime;
			if (bytesRead > 0)// throw std::runtime_error("recv failed");
			{
				clientIdleTime = 0;
				unsigned char requestId = header[0];
				unsigned int messageSize =
					(header[1] << 24) |
					(header[2] << 16) |
					(header[3] << 8) |
					(header[4]);
				// 2) recv the JSON payload
				std::vector<unsigned char> buffer(messageSize);
				int totalReceived = 0;
				while (totalReceived < (int)messageSize)
				{
					int received = recv(c,
						reinterpret_cast<char*>(&buffer[totalReceived]),
						messageSize - totalReceived,
						0);
					if (received <= 0) throw std::runtime_error("recv failed");
					std::cout << received << std::endl;
					totalReceived += received;
				}
				if (totalReceived != (int)messageSize) throw std::runtime_error("incomplete message");

				// 3) build RequestInfo
				RequestInfo reqInfo;
				reqInfo.id = requestId;
				reqInfo.receivalTime = std::time(nullptr);
				reqInfo.buffer = std::move(buffer);
				// 4) handle relevance & dispatch
				RequestResult result;
				if (currentHandler->isRequestRelevant(reqInfo))
				{
					result = currentHandler->handleRequest(reqInfo);
				}
				else
				{
					ErrorResponse err{ "Request not relevant to this handler" };
					result.response = JsonResponsePacketSerializer::serializeResponse(err);
				}
				// 5) send back the serialized response
				send(c,
					reinterpret_cast<const char*>(result.response.data()),
					static_cast<int>(result.response.size()),
					0);

				// 6) switch to a new handler if one was returned
				if (result.newHandler != nullptr && result.newHandler != currentHandler)
				{
					delete currentHandler;
					currentHandler = result.newHandler;
				}
			}
		}

		// cleanup
		//delete currentHandler;
		closesocket(c);
		delete this->m_clients[c];
		this->m_clients.erase(c);
		std::cout << "Client disconnected" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception occurred with client socket: " << e.what() << std::endl;
		closesocket(c);
		//try
		//{
		//	delete this->m_clients[c];
		//	this->m_clients.erase(c);
		//}
		//catch (const std::exception& e) {}
		std::cout << "Client disconnected" << std::endl;
	}

}