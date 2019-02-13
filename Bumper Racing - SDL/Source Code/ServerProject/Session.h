#pragma once
#include <map>
#include <algorithm>
#include <SDL2/SDL_net.h>
#include <vector>

const int MAX_PLAYERS = 4;

class Session {
public:
	Session();
	int GetPlayerCount() const;
	void AddPlayer(int playerID, IPaddress addr);
	bool RemovePlayer(int playerID);
	int GetHostID();
	IPaddress GetPlayerIP(int playerID);
	bool Joinable() const;
	std::vector<int> GetPlayerIDs() const;
	void Ready(int playerID);
	bool AllReady();
	std::vector<bool> GetReadied() const;
	bool GetWaiting() const;
	int id;
private:
	struct Player
	{
		IPaddress address;
		bool ready;
	};
	std::map<int, Player> _players;
	bool _waiting;
	int _hostID;
};
