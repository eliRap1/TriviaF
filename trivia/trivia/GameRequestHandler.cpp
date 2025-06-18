#include "GameRequestHandler.h"
using json = nlohmann::json;

GameRequestHandler::GameRequestHandler(Game& game, LoggedUser* user, GameManager* gm, RequestHandlerFactory& factory)
	: m_game(game), m_user(user), m_gameManager(gm), m_handlerFactory(factory)
{
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
	GetQuestionResponse response;
	try
	{
		Question q = m_game.getQuestionForUser(*m_user);
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
	m_game.submitAnswer(*m_user, ansID, TIME_PER_QUESTION);
	SubmitAnswerResponse response;
	response.status = 1;
	response.correctAnswerId = m_game.getQuestionForUser(*m_user).getCorrectAnswerId();

	return { JsonResponsePacketSerializer::serializeResponse(response), this };
}

RequestResult GameRequestHandler::getGameResults(RequestInfo& request)
{
	GetGameResultsResponse response;
	response.status = 1;
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

	return { JsonResponsePacketSerializer::serializeResponse(response), nullptr };

}

RequestResult GameRequestHandler::leaveGame(RequestInfo& request)
{
	LeaveRoomResponse response;
	response.status = 1;

	return {
		JsonResponsePacketSerializer::serializeResponse(response),
		static_cast<IRequestHandler*>(m_handlerFactory.createMenuRequestHandler(*m_user))
	};
}
