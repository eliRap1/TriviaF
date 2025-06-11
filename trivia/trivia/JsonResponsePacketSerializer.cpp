#include "JsonResponsePacketSerializer.h"

using json = nlohmann::json;
/*
	made this help function to make it easier to add length
*/
static void addLengthToBuffer(std::vector<unsigned char>& buffer, int len)
{
	buffer.push_back((len >> 24) & 0xFF);
	buffer.push_back((len >> 16) & 0xFF);
	buffer.push_back((len >> 8) & 0xFF);
	buffer.push_back(len & 0xFF);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& res) {
	json j;
	j["message"] = res.message;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(ERROR_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(LOGIN_RESPONSE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(SIGNUP_RESPONSE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(LOGOUT_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomResponse& res) {
	json j;
	j["status"] = res.status;
	j["rooms"] = res.rooms;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(GET_ROOMS_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& res) {
	json j;
	j["players"] = res.players;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(GET_PLAYERS_IN_ROOM_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(JOIN_ROOM_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(CREATE_ROOM_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const getHighScoresResponse& res) {
	json j;
	j["status"] = res.status;
	j["highScores"] = res.highScores;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(HIGH_SCORE_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const getPersonalStatsResponse& res) {
	json j;
	j["status"] = res.status;
	j["statistics"] = res.statistics;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(PERSONAL_STATS_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(CLOSE_ROOM_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(START_GAME_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& res) {
	json j;
	j["status"] = res.status;
	j["hasGameBegun"] = res.hasGameBegun;
	j["players"] = res.players;
	j["questionCount"] = res.questionCount;
	j["answerTimeout"] = res.answerTimeout;

	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(GET_ROOM_STATE_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(LEAVE_ROOM_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& res) {
	json j;
	j["status"] = res.status;
	j["results"] = json::array();
	for (const PlayerResults& pr : res.results) {
		json playerJson;
		playerJson["username"] = pr.username;
		playerJson["correctAnswerCount"] = pr.correctAnswerCount;
		playerJson["wrongAnswerCount"] = pr.wrongAnswerCount;
		playerJson["averageAnswerTime"] = pr.averageAnswerTime;
		j["results"].push_back(playerJson);
	}
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(GET_GAME_RESULTS_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& res) {
	json j;
	j["status"] = res.status;
	j["correctAnswerId"] = res.correctAnswerId;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(SUBMIT_ANSWER_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& res) {
	json j;
	j["status"] = res.status;
	j["question"] = res.question;
	j["answers"] = json::object();
	for (const std::pair<const unsigned int, std::string>& pair : res.answers) {
		j["answers"][std::to_string(pair.first)] = pair.second;
	}
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(GET_QUESTION_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& res) {
	json j;
	j["status"] = res.status;
	std::string dump = j.dump();
	std::vector<unsigned char> buffer;
	buffer.push_back(LEAVE_ROOM_CODE);
	addLengthToBuffer(buffer, dump.size());
	buffer.insert(buffer.end(), dump.begin(), dump.end());
	return buffer;
}