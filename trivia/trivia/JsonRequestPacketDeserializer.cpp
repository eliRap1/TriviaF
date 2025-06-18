#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end()); 

	json j = json::parse(jsonStr);

	LoginRequest request;
	request.username = j["username"];
	request.password = j["password"];
	return request;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());

	json j = json::parse(jsonStr);

	SignupRequest request;
	request.username = j["username"];
	request.password = j["password"];
	request.email = j["email"];
	return request;
}

GetPlayerInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayerInRoomRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	json j = json::parse(jsonStr);

	GetPlayerInRoomRequest request;
	std::string roomId = j["roomId"];
	request.roomId = std::atoi(roomId.c_str());
	return request;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	json j = json::parse(jsonStr);

	JoinRoomRequest request;
	std::string roomId = j["roomId"];
	request.roomId = std::atoi(roomId.c_str());
	return request;
}
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
	std::string jsonStr(buffer.begin(), buffer.end());
	json j = json::parse(jsonStr);

	CreateRoomRequest request;
	request.roomName = j["roomName"];
	request.answerTimeout = j["answerTimeout"];
	request.maxUsers = j["maxUsers"];
	request.QuestionsCount = j["QuestionsCount"];
	return request;
}


SumbitAnswerRequest JsonRequestPacketDeserializer::deserializeSumbitAnswerRequest(const std::vector<unsigned char>& buffer)
{
	SumbitAnswerRequest req;
	auto j = nlohmann::json::parse(buffer.begin(), buffer.end());
	req.answerId = j["answerId"];
	return req;
}