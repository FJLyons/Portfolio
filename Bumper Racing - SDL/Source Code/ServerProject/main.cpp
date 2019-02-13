#include "Net.h"
#include "Session.h"
#include <unordered_map>
#include <time.h>
#include <chrono>

using namespace Network;

typedef std::unordered_map<int, Session> SessionMap;
typedef std::chrono::time_point<std::chrono::system_clock> Clock;


struct Spectator {
	int playerID;
	IPaddress address;
};

struct Client {
	int playerID;
	int sessionID;
};

bool exists(const std::vector<Spectator>& spectators, const IPaddress& ip)
{
	for (std::vector<Spectator>::const_iterator it = spectators.begin(); it != spectators.end(); ++it)
	{
		if (it->address.host == ip.host &&
			it->address.port == ip.port)
			return true;
	}
	return false;
}

bool exists(const std::vector<Spectator>& spectators, int id)
{
	for (std::vector<Spectator>::const_iterator it = spectators.begin(); it != spectators.end(); ++it)
	{
		if (it->playerID == id)
			return true;
	}
	return false;
}

SessionListData CreateSessionListData(const SessionMap& sessions)
{
	SessionListData data;
	data.maxPlayers = MAX_PLAYERS;
	for (SessionMap::const_iterator it = sessions.begin(); it != sessions.end(); ++it)
	{
		if (it->second.GetWaiting())
		{
			data.sessionIDs.push_back(it->first);
			data.currentPlayers.push_back(it->second.GetPlayerCount());
		}
	}
	data.count = data.sessionIDs.size();

	return data;
}

void RemoveSpectator(std::vector<Spectator>& spectators, IPaddress srcAddr)
{
	//remove from spectator list
	spectators.erase(std::remove_if(spectators.begin(), spectators.end(),
		[srcAddr](const Spectator& s)
	{
		return (s.address.host == srcAddr.host && s.address.port == srcAddr.port);
	}
	), spectators.end());
}

//custom hask function in order to use IPaddress as key
namespace std 
{
	template <>
	struct hash<IPaddress>
	{
		std::size_t operator()(const IPaddress& ip) const
		{
			using std::size_t;
			using std::hash;
			return ((hash<int>()(ip.host)
				^ (hash<int>()(ip.port) << 1)) >> 1);
		}
	};

}

bool exists(const std::unordered_map<IPaddress, Client>& clients, IPaddress address)
{
	for (std::unordered_map<IPaddress, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->first.host == address.host &&
			it->first.port == address.port)
			return true;
	}
	return false;
}

const float CHECK_CONNECTION = 2.5f;
const float TIME_OUT = 10.f; 

struct Connection
{
	float timeSinceResponse;
	IPaddress addr;
	int sessionID;
};

void Disconnect(DisconnectData data, SessionMap& sessions, Net& net, std::vector<Spectator>& spectators, IPaddress srcAddr)
{
	if (data.sessionID >= 0) //is within a session
	{
		std::cout << "Removing a player from a session." << std::endl;
		if (sessions[data.sessionID].RemovePlayer(data.id))
		{
			//removed a host
			//need to let new host know
			int newHostId = sessions[data.sessionID].GetHostID();
			std::cout << "Removed " << data.id << " as host. New host is: " << newHostId << std::endl;
			if (newHostId >= 0)
			{
				SetHostData shdata;
				net.Send(&shdata, sessions[data.sessionID].GetPlayerIP(newHostId));
			}
		}
		if (sessions[data.sessionID].GetPlayerCount() == 0)
		{
			std::cout << "Session: " << data.sessionID << " has 0 players and will be removed." << std::endl;
			if (sessions.find(data.sessionID) != sessions.end())
			{
				sessions.erase(sessions.find(data.sessionID));
			}
			else
			{
				std::cout << "Tried to erase session " << data.sessionID << " which doesnt exist." << std::endl;
			}
		}
		else
		{ //update the other players player lists
			std::cout << "Notify other players. " << std::endl;
			PlayerListData pldata;
			pldata.count = sessions[data.sessionID].GetPlayerCount();
			pldata.players = sessions[data.sessionID].GetPlayerIDs();
			for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
			{
				if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)
				{
					net.Send(&pldata, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));//send player list to everybody in the session
					net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i])); //send disconnect to everyboy in the session
				}
			}
		}
	}
	else //only a spectator
	{
		std::cout << "Removed spectator." << std::endl;
		RemoveSpectator(spectators, srcAddr);
		std::cout << "Sessions Disconnect: " << sessions.size() << std::endl;
	}
}

#undef main
int main(int argc, char** argv)
{

	SDLNet_Init();
	float checkTimer = 0.f;
	//std::unordered_map<IPaddress, Client> clients; //player and session id to ip address mapping
	std::vector<Spectator> spectators;
	SessionMap sessions;
	std::unordered_map<int, Connection> connections; //all connected players
	Net net(5228);
	Clock currentTime;
	Clock lastTime = std::chrono::system_clock::now();
	int playerCount = 0;
	int sessionCount = 0;
	float serverTime = 0.f;
	while (true)
	{
		currentTime = std::chrono::system_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count() / 1000000000.f;

		checkTimer += dt;
		serverTime += dt;
		//std::cout << serverTime << std::endl;
		for (std::unordered_map<int, Connection>::iterator it = connections.begin(); it != connections.end();)
		{
			it->second.timeSinceResponse += dt;
			if (it->second.timeSinceResponse > TIME_OUT)
			{
				std::cout << "Disconnect " << it->first << " due to time out." << std::endl;
				DisconnectData data;
				data.id = it->first;
				data.sessionID = it->second.sessionID;
				Disconnect(data, sessions, net, spectators, it->second.addr);
				it = connections.erase(it);
			}
			else
			{
				++it;
			}
		}
		if (checkTimer > CHECK_CONNECTION)
		{
			checkTimer = 0.f;
			for (std::unordered_map<int, Connection>::iterator it = connections.begin(); it != connections.end(); ++it)
			{
				CheckConnectionData data;
				net.Send(&data, it->second.addr);
			}
		}

		ReceivedData receiveData = net.Receive();
		if (receiveData.Empty() == false)
		{
			IPaddress srcAddr = receiveData.GetSrcAddress();
			
			switch (receiveData.GetType())
			{
			case MessageType::CheckConnection:
			{
				CheckConnectionData data = receiveData.GetData<CheckConnectionData>();
				connections[data.id].timeSinceResponse = 0.f;
				break;
			}
			case MessageType::Connect:
			{
				ConnectData data = receiveData.GetData<ConnectData>();
				std::cout << "spectators " << spectators.size() << std::endl;
				if (exists(spectators, srcAddr) == false)//if the server isnt restarted and the same player trys to join again this isnt true
				{
					std::cout << "Player connected to server. ID: " << data.id << " and SessionID: " << data.sessionID << std::endl;
					if (data.id < 0)
					{
						data.id = playerCount++;
						std::cout << "Spectator added. ID assigned: " << data.id << std::endl;
						Spectator s;
						s.playerID = data.id;
						s.address = srcAddr;
						spectators.push_back(s);

						Connection c;
						c.timeSinceResponse = 0.f;
						c.addr = srcAddr;
						c.sessionID = data.sessionID;
						connections.insert(std::pair<int, Connection>(data.id, c));

						net.Send(&data, srcAddr);
					}
				}
				SessionListData sessionData = CreateSessionListData(sessions);
				std::cout << "Sending session list of size " << sessionData.count << " to player: " << data.id << std::endl;
				net.Send(&sessionData, srcAddr);
				break;
			}
			case MessageType::JoinSession:
			{
				JoinSessionData data = receiveData.GetData<JoinSessionData>();
				if (exists(spectators, data.id) == false)//first check that the player isn't a spectator
				{
					break;
				}

				if (data.sessionID >= 0) //session exists
				{
					if (sessions[data.sessionID].Joinable()) //has space to join
					{
						std::cout << "Player " << data.id << " joining session " << data.sessionID << std::endl;
						sessions[data.sessionID].AddPlayer(data.id, srcAddr);
						connections[data.id].sessionID = data.sessionID;
					}
					else
					{
						//client will ensure to check session list before trying to join a session in case it is full
						std::cout << "Can't join session as it is full..." << std::endl;
					}
				}
				else //need to make a new session
				{
					int sessionID = sessionCount++;
					std::cout << "Creating session " << sessionID << " with player " << data.id << " as host." << std::endl;
					//error check if player already in session.
					Session s;
					s.id = sessionID;
					s.AddPlayer(data.id, srcAddr);
					sessions.insert(std::pair<int, Session>(sessionID, s));

					//now tell the player what session they have just joined and the player list?. player list is in different scene so that scene could send a seperate message
					data.sessionID = sessionID;
					data.host = true; //if made a new session then we are the host
					connections[data.id].sessionID = data.sessionID;
				}
				RemoveSpectator(spectators, srcAddr);
				net.Send(&data, srcAddr); //send joinsession

				SessionListData sessionListData = CreateSessionListData(sessions);
				for (int i = 0; i < spectators.size(); i++)
				{
					net.Send(&sessionListData, spectators[i].address); //send session list to all spectators
				}

				PlayerListData pldata;
				pldata.count = sessions[data.sessionID].GetPlayerCount();
				pldata.players = sessions[data.sessionID].GetPlayerIDs();
				//TODO simplify this (also used in ready event)
				for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
				{
					net.Send(&pldata, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));//send player list to everybody in the session
				}

				break;
			}
			case MessageType::Disconnect:
			{
				DisconnectData data = receiveData.GetData<DisconnectData>();
				std::cout << "Disconnecting player: " << data.id << " from session: " << data.sessionID << std::endl;
				if (data.id >= 0) //is a valid id
				{
					Disconnect(data, sessions, net, spectators, srcAddr);
					if (connections.find(data.id) != connections.end())
					{
						connections.erase(connections.find(data.id));
					}
					else
					{
						std::cout << "Tried to erase " << data.id << " which doesnt exist." << std::endl;
					}
				}
				break;
			}
			case MessageType::Ready:
			{
				ReadyData data = receiveData.GetData<ReadyData>();
				data.allReady = false;
				std::cout << "Ready message from: " << data.id << " who is in session: " << data.sessionID << std::endl;
				if (data.sessionID != -1 && sessions[data.sessionID].GetWaiting())
				{
					std::cout << "Session " << data.sessionID << " has " << sessions[data.sessionID].GetPlayerCount() << " players." << std::endl;
					std::cout << "That player id is " << sessions[data.sessionID].GetPlayerIDs()[0] << std::endl;
					sessions[data.sessionID].Ready(data.id); //tell session that the player is ready
					data.ready = sessions[data.sessionID].GetReadied();
					data.ids = sessions[data.sessionID].GetPlayerIDs();
					if (sessions[data.sessionID].AllReady()) //all readied up
					{
						data.allReady = true;
						data.seed = time(NULL);
						data.gameStartTime = serverTime + 4.f;
						std::cout << "Session fully ready, start that game!" << std::endl;

						SessionListData sessionListData = CreateSessionListData(sessions);
						for (int i = 0; i < spectators.size(); i++)
						{
							net.Send(&sessionListData, spectators[i].address); //send session list to all spectators
						}
					}
					//send to all needs to have time synced up correctly whern AllReady is true
					//TODO: simply this, also used in sending player list, need a SendToAll function
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));//send player list to everybody in the session
					}
				}
				break;
			}
			case MessageType::State:
			{
				StateData data = receiveData.GetData<StateData>();
				if (data.sessionID != -1)
				{
					if (data.host) //host is sending state to everybody else
					{
						for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
						{
							if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)//dont send state data back to the original player
							{
								net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
								//std::cout << "Relay data " << data.remoteID << ", to: " << sessions[data.sessionID].GetPlayerIDs()[i] << std::endl;
							}
						}
					}
					else //this is a player sending data to host
					{//send data to host
					 //std::cout << "Relay data to host: " << sessions[data.sessionID].GetHostID() << std::endl;
						net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetHostID()));
					}
				}
				break;
			}
			case MessageType::MultiState:
			{
				MultiStateData data = receiveData.GetData<MultiStateData>();
				if (data.sessionID != -1)
				{
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)//dont send state data back to the original player
						{
							net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
						}
					}
				}
				break;
			}
			case MessageType::CreatePowerUp:
			{
				CreatePowerUpData data = receiveData.GetData<CreatePowerUpData>();
				if (data.sessionID != -1)
				{
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)//dont send data back to the original player
						{
							net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
							std::cout << "Telling " << sessions[data.sessionID].GetPlayerIDs()[i] << " to create power up." << std::endl;
						}
					}
				}
				break;
			}
			case MessageType::Fire:
			{
				FireData data = receiveData.GetData<FireData>();
				if (data.sessionID != -1)
				{
					std::cout << "got fire packet " << std::endl;
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
						std::cout << "Send fire event to all players in session: " << data.sessionID << std::endl;
					}
				}
				break;
			}
			case MessageType::PickUpFlag:
			{
				PickUpFlagData data = receiveData.GetData<PickUpFlagData>();
				if (data.sessionID != -1)
				{
					std::cout << "got pick up flag packet " << std::endl;
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)//dont send data back to the original player
						{
							net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
						}
					}
				}
				break;
			}
			case MessageType::DroppedFlag:
			{
				DroppedFlagData data = receiveData.GetData<DroppedFlagData>();
				if (data.sessionID != -1)
				{
					std::cout << "got dropped flag packet " << std::endl;
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)//dont send data back to the original player
						{
							net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
						}
					}
				}
				break;
			}
			case MessageType::Invis:
			{
				InvisData data = receiveData.GetData<InvisData>();
				if (data.sessionID != -1)
				{
					std::cout << "got invis packet " << std::endl;
					for (int i = 0; i < sessions[data.sessionID].GetPlayerIDs().size(); i++)
					{
						if (sessions[data.sessionID].GetPlayerIDs()[i] != data.id)//dont send data back to the original player
						{
							net.Send(&data, sessions[data.sessionID].GetPlayerIP(sessions[data.sessionID].GetPlayerIDs()[i]));
						}
					}
				}
				break;
			}
			case MessageType::Ping:
			{
				PingData data = receiveData.GetData<PingData>();
				std::cout << "Recv ping with gametime: " << data.ts << " , serverTime: " << serverTime << std::endl;
				data.serverTime = serverTime;
				net.Send(&data, srcAddr);
				break;
			}
			}
		}
		lastTime = currentTime;
	};

	SDLNet_Quit();
	return 0;
}