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
	bool AllReady() const;
	bool IsReadytest();
	std::vector<bool> GetReadied() const;
private:
	std::map<int, IPaddress> _players;
	std::vector<bool> _readiedUp;
	bool _waiting;
};
