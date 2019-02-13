#include "Net.h"
#include <thread>

using namespace Network;

void client(Net& net, IPaddress& serverIP);

int main(int argc, char** argv)
{
	SDLNet_Init();

	IPaddress serverIP;
	SDLNet_ResolveHost(&serverIP, "localhost", 5228);

	Net net(5999);

	std::vector<std::thread> clients;
	int clientsCount = 5;
	for (int i = 0; i < clientsCount; i++)
	{
		net = Net(6000 + i);
		clients.push_back(std::thread(client, net, serverIP));
	}

	for (int i = 0; i < clientsCount; i++)
	{
		clients[i].join();
	}
	SDLNet_Quit();
	system("pause");
	return 0;
} 

void client(Net& net, IPaddress& serverIP)
{
	ConnectData data;
	net.Send(&data, serverIP);

	int id = -1;
	int sessionID = -1;

	bool waiting = true;
	while (waiting)
	{
		ReceivedData data = net.Receive();

		if (data.Empty())
			continue;

		switch (data.GetType())
		{
		case MessageType::Connect:
		{
			ConnectData cdata = data.GetData<ConnectData>();
			std::cout << "Got assigned ID: " << cdata.id << std::endl;
			id = cdata.id;
			break;
		}
		case MessageType::SessionList:
		{
			SessionListData sldata = data.GetData<SessionListData>();
			std::cout << "Sessions Available: " << sldata.count << std::endl;
			for (int i = 0; i < sldata.count; i++)
			{
				std::cout << "Session ID: " << sldata.sessionIDs[i];
				std::cout << "  Players: " << sldata.currentPlayers[i] << "/" << sldata.maxPlayers << std::endl;
			}
			waiting = false;
			break;
		}
		}
	}

	int input = 0;
	std::cout << "Press 1 to create session or 2 to refresh session list or 3 to join session 0 (if it exists)." << std::endl;
	std::cin >> input;
	if (input == 1)
	{ //create session as host
		JoinSessionData data;
		data.id = id;
		net.Send(&data, serverIP);

		waiting = true;
		while (waiting)
		{
			ReceivedData data = net.Receive();
			if (data.Empty())
				continue;

			switch (data.GetType())
			{
			case MessageType::JoinSession:
			{
				JoinSessionData jsdata = data.GetData<JoinSessionData>(); //client gets session id, switch scene to player list, get id before or after scene switch??
				std::cout << "Joined session: " << jsdata.sessionID << std::endl;
				sessionID = jsdata.sessionID;
				waiting = false;
				break;
			}
			}
		}
	}
	else if (input == 2)
	{ //refresh session list
		waiting = true;
		ConnectData data;
		data.id = id;
		net.Send(&data, serverIP);
		while (waiting)
		{
			ReceivedData data = net.Receive();

			if (data.Empty())
				continue;

			switch (data.GetType())
			{
			case MessageType::SessionList:
			{
				SessionListData sldata = data.GetData<SessionListData>();
				std::cout << "Sessions Available: " << sldata.count << std::endl;
				for (int i = 0; i < sldata.count; i++)
				{
					std::cout << "Session ID: " << sldata.sessionIDs[i];
					std::cout << "  Players: " << sldata.currentPlayers[i] << "/" << sldata.maxPlayers << std::endl;
					//TODO: destroy session? why does it have one player?
				}
				waiting = false;
				break;
			}
			}
		}
	}
	else if (input == 3)
	{ //join some other session (0)
		JoinSessionData data;
		data.id = id;
		data.sessionID = 0;
		net.Send(&data, serverIP);
		std::cout << "Trying to join session 0." << std::endl;
		waiting = true;
		while (waiting)
		{
			ReceivedData data = net.Receive();
			if (data.Empty())
				continue;

			switch (data.GetType())
			{
			case MessageType::JoinSession:
			{
				JoinSessionData jsdata = data.GetData<JoinSessionData>(); //but no point to listen for this event if you only join and dont create as you know the sessionID
				std::cout << "Joined session: " << jsdata.sessionID << std::endl;
				waiting = false;
				break;
			}
			}
		}
	}

	while (true) {};
	DisconnectData disconnect;
	disconnect.id = id;
	disconnect.sessionID = sessionID;
	net.Send(&disconnect, serverIP);
	std::cout << "Disconnecting " << id << std::endl;
}

