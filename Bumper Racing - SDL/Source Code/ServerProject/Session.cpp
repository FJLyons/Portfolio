#include "Session.h"

Session::Session()
	: _waiting(true)
	, _hostID(-1)
{
}

int Session::GetPlayerCount() const
{
	return _players.size();
}

void Session::AddPlayer(int playerID, IPaddress addr)
{
	if (_players.empty())
	{
		_hostID = playerID; //first to join is host
	}
	Player p;
	p.address = addr;
	p.ready = false;
	_players.insert(std::pair<int, Player>(playerID, p));
	for (std::map<int, Player>::iterator it = _players.begin(); it != _players.end(); ++it)
	{
		it->second.ready = false; //unready everybody when someone joins
	}
}

bool Session::RemovePlayer(int playerID)
{
	bool removingHost = false;
	if (playerID == GetHostID()) //host is being removed!
	{
		removingHost = true;
	}
	if (_players.find(playerID) != _players.end())
	{
		_players.erase(_players.find(playerID)); //remove player
	}
	if (_players.empty())
	{
		_hostID = -1;
	}
	else if (removingHost)
	{
		_hostID = _players.begin()->first;
	}
	return removingHost;
}

int Session::GetHostID()
{
	return _hostID;
}

IPaddress Session::GetPlayerIP(int playerID)
{
	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		if (it->first == playerID)
		{
			return it->second.address;
		}
	}
	return IPaddress();
}

bool Session::Joinable() const
{
	return GetPlayerCount() < MAX_PLAYERS;
}

bool Session::GetWaiting() const
{
	return _waiting;
}

std::vector<int> Session::GetPlayerIDs() const
{
	std::vector<int> players;

	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		players.push_back(it->first);
	}

	return players;
}

void Session::Ready(int playerID)
{
	_players[playerID].ready = true;
}

bool Session::AllReady()
{
	bool allready = true;
	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		if (!it->second.ready)
		{
			allready = false;
			break;
		}
	}
	_waiting = !allready;
	return allready;
}

std::vector<bool> Session::GetReadied() const
{
	std::vector<bool> readiedUp;
	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		readiedUp.push_back(it->second.ready);
	}
	return readiedUp;
}