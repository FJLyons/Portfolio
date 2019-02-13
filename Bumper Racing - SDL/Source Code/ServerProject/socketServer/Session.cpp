#include "Session.h"

Session::Session()
	: _waiting(true)
{
}

int Session::GetPlayerCount() const
{
	return _players.size();
}

void Session::AddPlayer(int playerID, IPaddress addr)
{
	_players.insert(std::pair<int, IPaddress>(playerID, addr));
	_readiedUp.push_back(false);
}

bool Session::RemovePlayer(int playerID)
{
	bool removingHost = false;
	std::map<int, IPaddress>::const_iterator idToErase = _players.find(playerID);
	if (playerID == GetHostID()) //host is being removed!
	{
		removingHost = true;
	}
	if (idToErase != _players.end())
	{
		_players.erase(idToErase);
	}
	return removingHost;
}

int Session::GetHostID()
{
	if (_players.empty())
	{
		return -1;
	}
	return _players.begin()->first;
}

IPaddress Session::GetPlayerIP(int playerID)
{
	for (std::map<int, IPaddress>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		if (it->first == playerID)
		{
			return it->second;
		}
	}
	return IPaddress();
}

bool Session::Joinable() const
{
	return GetPlayerCount() < MAX_PLAYERS;
}

std::vector<int> Session::GetPlayerIDs() const
{
	std::vector<int> players;

	for (std::map<int, IPaddress>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		players.push_back(it->first);
	}

	return players;
}

void Session::Ready(int playerID)
{
	int readyIndex = std::distance(_players.begin(), _players.find(playerID));
	_readiedUp[readyIndex] = true;
}

bool Session::AllReady() const
{
	bool allready = true;
	for (int i = 0; i < _readiedUp.size(); i++)
	{
		if (!_readiedUp[i])
		{
			allready = false;
			break;
		}
	}
	return allready;
}

bool Session::IsReadytest()
{
	return _readiedUp[0];
}

std::vector<bool> Session::GetReadied() const
{
	return _readiedUp;
}
