#include "Room.h"

Room::Room(const LoggedUser& user, const RoomData& metadata) : m_metadata(metadata) { m_users.push_back(user); }
void Room::addUser(const LoggedUser& user)
{
	auto it = std::find_if(m_users.begin(), m_users.end(), [&](const LoggedUser& u) {
		return u.getUsername() == user.getUsername();
		});

	if (it == m_users.end()) // user not in the room
	{
		m_users.push_back(user);
	}
}
void Room::removeUser(const LoggedUser& user)
{
	m_users.erase(
		std::remove_if(m_users.begin(), m_users.end(), [&](const LoggedUser& u) {
			return u.getUsername() == user.getUsername();
			}),
		m_users.end()
	);
}


const std::vector<std::string> Room::getUsers() const {
	std::vector<std::string> usernames;
	for (const auto& user : m_users) {
		usernames.push_back(user.getUsername());
	}
	return usernames;
}

const RoomData& Room::getRoomData() const { return m_metadata; }
void Room::setState(RoomState state)
{
	m_state = state;
}

Room::RoomState Room::getState() const
{
	return m_state;
}