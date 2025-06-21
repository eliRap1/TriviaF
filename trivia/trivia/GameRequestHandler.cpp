#include "GameRequestHandler.h"
using json = nlohmann::json;

GameRequestHandler::GameRequestHandler(const LoggedUser& user,Game& game, RequestHandlerFactory& factory)
	: m_user(user), m_game(game), m_handlerFactory(factory)
{
	std::cout << "GameRequestHandler:: Game ID: " << m_game.getGameId() << "" << std::endl;
	m_gameManager = &m_handlerFactory.getGameManager();
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& request)
{
	return request.id == GET_QUESTION_CODE ||
		request.id == SUBMIT_ANSWER_CODE ||
		request.id == GET_GAME_RESULTS_CODE ||
		request.id == LEAVE_ROOM_CODE;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo& request)
{
	switch (request.id)
	{
	case GET_QUESTION_CODE:
		return getQuestion(request);
	case SUBMIT_ANSWER_CODE:
		return submitAnswer(request);
	case GET_GAME_RESULTS_CODE:
		return getGameResults(request);
	case LEAVE_ROOM_CODE:
		return leaveGame(request);
	default:
		return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request ID" }), nullptr };
	}
}

RequestResult GameRequestHandler::getQuestion(RequestInfo& request)
{
	GetQuestionResponse response = { 0 };
	try
	{

		std::cout << "GameRequestHandler::getQuestion" << std::endl;
		std::cout << "Getting question for user " << m_user.getUsername() << std::endl;
		Question q = m_game.getQuestionForUser(m_user);
		response.status = 1;
		response.question = q.getQuestion();
		auto answers = q.getPossibleAnswers();
		std::random_shuffle(answers.begin(), answers.end());
		int ansID = q.getCorrectAnswerId();
		response.answers[0] = answers[0];
		response.answers[1] = answers[1];
		response.answers[2] = answers[2];
		response.answers[3] = answers[3];

	}
	catch (const std::exception& e)
	{
		return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() }), nullptr };
	}

	return { JsonResponsePacketSerializer::serializeResponse(response), this };

}

RequestResult GameRequestHandler::submitAnswer(RequestInfo& request)
{
	SumbitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSumbitAnswerRequest(request.buffer);
	int ansID = req.answerId;
	m_game.submitAnswer(m_user, ansID, m_handlerFactory.getRoomManager().getRoom(m_game.getGameId()).getRoomData().timePerQuestion);
	SubmitAnswerResponse response;
	response.status = 1;
	response.correctAnswerId = m_game.getQuestionForUser(m_user).getCorrectAnswerId();
	return { JsonResponsePacketSerializer::serializeResponse(response), this };
}

RequestResult GameRequestHandler::getGameResults(RequestInfo& request)
{
	GetGameResultsResponse response;
	response.status = 1;
	try {
		auto allPlayers = m_game.getPlayers();

		for (const auto& entry : allPlayers)
		{
			const std::string& username = entry.first.getUsername();
			const GameData& data = entry.second;

			PlayerResults res;
			res.username = username;
			res.correctAnswerCount = data.correctAnswerCount;
			res.wrongAnswerCount = data.wrongAnswerCount;
			res.averageAnswerTime = data.averageAnswerTime;

			response.results.push_back(res);
		}

		return { JsonResponsePacketSerializer::serializeResponse(response), this };

	}
	catch (const std::exception& e)
	{
		return { JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Game not found"}), nullptr};
	}
	
}

RequestResult GameRequestHandler::leaveGame(RequestInfo& request)
{
	LeaveRoomResponse response;
	response.status = 1;
	m_game.sumitGameStatsToDB(m_user);
	m_game.removePlayer(m_user);
	if (m_game.getPlayers().empty())
	{
		m_gameManager->deleteGame(m_game.getGameId());
		m_handlerFactory.getRoomManager().deleteRoom(m_game.getGameId());
	}
	return {
		JsonResponsePacketSerializer::serializeResponse(response), m_handlerFactory.createMenuRequestHandler(m_user)
	};
}
