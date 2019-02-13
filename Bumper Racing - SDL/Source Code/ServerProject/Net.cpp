#include "Net.h"
#include <sstream>

Network::Net::Net(int port, int packetSize)
{
	_socket = SDLNet_UDP_Open(port);
	if (!_socket)
	{
		std::cout << "Failed to open socket on port " << port << std::endl;
	}
	_packet = SDLNet_AllocPacket(packetSize);
	_testSocketCreated = true;
}

Network::Net::~Net()
{
}

Network::Net::Net()
{
}


void Network::Net::Send(MessageData * data, const char * destHost, int destPort)
{
	IPaddress destAddr;
	SDLNet_ResolveHost(&destAddr, destHost, destPort);
	Send(data, destAddr);
}

void Network::Net::Send(MessageData * data, IPaddress destAddr)
{
	if (!data)
	{
		std::cout << "Send tried to send nullptr" << std::endl;
		return;
	}

	MessageType type = data->GetType();
	_packet->len = 0;
	WriteInt((Uint8)type);

	//write id and session id which always exists
	WriteInt(data->id);
	WriteInt(data->sessionID);
	WriteFloat(data->ts);

	int v = 0;
	switch (type)
	{
	case MessageType::Connect:
	{
		ConnectData* sendData = (ConnectData*)data;
		break;
	}
	case MessageType::Disconnect:
	{
		DisconnectData* sendData = (DisconnectData*)data;
		break;
	}
	case MessageType::State:
	{
		StateData* sendData = (StateData*)data;
		WriteFloat(sendData->xPos);
		WriteFloat(sendData->yPos);
		WriteFloat(sendData->xVel);
		WriteFloat(sendData->yVel);
		WriteBool(sendData->host);
		WriteInt(sendData->remoteID);
		break;
	}
	case MessageType::MultiState:
	{
		MultiStateData* sendData = (MultiStateData*)data;
		WriteInt(sendData->count);
		for (int i = 0; i < sendData->count; i++)
		{
			WriteFloat(sendData->states[i].xPos);
			WriteFloat(sendData->states[i].yPos);
			WriteFloat(sendData->states[i].xVel);
			WriteFloat(sendData->states[i].yVel);
			WriteBool(sendData->states[i].host);
			WriteInt(sendData->states[i].remoteID);
		}
		break;
	}
	case MessageType::SessionList:
	{
		SessionListData* sendData = (SessionListData*)data;
		WriteInt(sendData->count);
		WriteInt(sendData->maxPlayers);
		for (int i = 0; i < sendData->count; i++)
		{
			WriteInt(sendData->sessionIDs[i]);
			WriteInt(sendData->currentPlayers[i]);
		}
		break;
	}
	case MessageType::JoinSession:
	{
		JoinSessionData* sendData = (JoinSessionData*)data;
		WriteBool(sendData->host);
		break;
	}
	case MessageType::PlayerList:
	{
		PlayerListData* sendData = (PlayerListData*)data;
		WriteInt(sendData->count);
		for (int i = 0; i < sendData->count; i++)
		{
			WriteInt(sendData->players[i]);
		}
		break;
	}
	case MessageType::Ready:
	{
		ReadyData* sendData = (ReadyData*)data;
		WriteInt(sendData->ids.size());
		for (int i = 0; i < sendData->ids.size(); i++)
		{
			WriteInt(sendData->ids[i]);
		}
		for (int i = 0; i < sendData->ids.size(); i++)
		{
			WriteBool(sendData->ready[i]);
		}
		WriteBool(sendData->allReady);
		WriteInt(sendData->seed);
		WriteFloat(sendData->gameStartTime);
		break;
	}
	case MessageType::CreatePowerUp:
	{
		CreatePowerUpData* sendData = (CreatePowerUpData*)data;
		WriteInt(sendData->index);
		WriteInt(sendData->powerType);
		break;
	}
	case MessageType::Fire:
	{
		FireData* sendData = (FireData*)data;
		WriteInt(sendData->remoteID);
		break;
	}
	case MessageType::PickUpFlag:
	{
		PickUpFlagData* sendData = (PickUpFlagData*)data;
		WriteInt(sendData->remoteID);
		break;
	}
	case MessageType::DroppedFlag:
	{
		DroppedFlagData* sendData = (DroppedFlagData*)data;
		WriteInt(sendData->remoteID);
		break;
	}
	case MessageType::Invis:
	{
		InvisData* sendData = (InvisData*)data;
		WriteInt(sendData->remoteID);
		break;
	}
	case MessageType::Ping:
	{
		PingData* sendData = (PingData*)data;
		WriteFloat(sendData->serverTime);
		break;
	}
	}

	_packet->address.host = destAddr.host;
	_packet->address.port = destAddr.port;
	if (SDLNet_UDP_Send(_socket, -1, _packet) == 0)
		std::cout << "Failed to send packet." << std::endl;
}


Network::ReceivedData Network::Net::Receive()
{
	ReceivedData receiveData;
	if (SDLNet_UDP_Recv(_socket, _packet) > 0)
	{
		receiveData.SetSrcAddress(_packet->address);
		int byteOffset = 0;
		MessageType type = (MessageType)ReadInt(byteOffset);
		int id = ReadInt(byteOffset);
		int sessionID = ReadInt(byteOffset);
		receiveData.SetIDs(id, sessionID);
		float timeStamp = ReadFloat(byteOffset);
		receiveData.SetTimeStamp(timeStamp);

		switch (type)
		{
		case MessageType::Connect:
		{
			ConnectData data;
			receiveData.SetData(data);
			break;
		}
		case MessageType::Disconnect:
		{
			DisconnectData data;
			receiveData.SetData(data);
			break;

		}
		case MessageType::State:
		{
			StateData data;
			data.xPos = ReadFloat(byteOffset);
			data.yPos = ReadFloat(byteOffset);
			data.xVel = ReadFloat(byteOffset);
			data.yVel = ReadFloat(byteOffset);
			data.host = ReadBool(byteOffset);
			data.remoteID = ReadInt(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::MultiState:
		{
			MultiStateData data;
			data.count = ReadInt(byteOffset);
			for (int i = 0; i < data.count; i++)
			{
				StateData s;
				s.xPos = ReadFloat(byteOffset);
				s.yPos = ReadFloat(byteOffset);
				s.xVel = ReadFloat(byteOffset);
				s.yVel = ReadFloat(byteOffset);
				s.host = ReadBool(byteOffset);
				s.remoteID = ReadInt(byteOffset);
				data.states.push_back(s);
			}
			receiveData.SetData(data);
			break;
		}
		case MessageType::SessionList:
		{
			SessionListData data;
			data.count = ReadInt(byteOffset);
			data.maxPlayers = ReadInt(byteOffset);
			for (int i = 0; i < data.count; i++)
			{ 
				data.sessionIDs.push_back(ReadInt(byteOffset));
				data.currentPlayers.push_back(ReadInt(byteOffset));
			}
			receiveData.SetData(data);
			break;
		}
		case MessageType::JoinSession:
		{
			JoinSessionData data;
			data.host = ReadBool(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::SetHost:
		{
			SetHostData data;
			receiveData.SetData(data);
			break;
		}
		case MessageType::PlayerList:
		{
			PlayerListData data;
			data.count = ReadInt(byteOffset);
			for (int i = 0; i < data.count; i++)
			{
				data.players.push_back(ReadInt(byteOffset));
			}
			receiveData.SetData(data);
			break;
		}
		case MessageType::Ready:
		{
			ReadyData data;
			int count = ReadInt(byteOffset);
			for (int i = 0; i < count; i++)
			{
				data.ids.push_back(ReadInt(byteOffset));
			}
			for (int i = 0; i < count; i++)
			{
				data.ready.push_back(ReadBool(byteOffset));
			}
			data.allReady = ReadBool(byteOffset);
			data.seed = ReadInt(byteOffset);
			data.gameStartTime = ReadFloat(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::CreatePowerUp:
		{
			CreatePowerUpData data;
			data.index = ReadInt(byteOffset);
			data.powerType = ReadInt(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::Fire:
		{
			FireData data;
			data.remoteID = ReadInt(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::PickUpFlag:
		{
			PickUpFlagData data;
			data.remoteID = ReadInt(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::DroppedFlag:
		{
			DroppedFlagData data;
			data.remoteID = ReadInt(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::CheckConnection:
		{
			CheckConnectionData data;
			receiveData.SetData(data);
			break;
		}
		case MessageType::Invis:
		{
			InvisData data;
			data.remoteID = ReadInt(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::Ping:
		{
			PingData data;
			data.serverTime = ReadFloat(byteOffset);
			receiveData.SetData(data);
			break;
		}
		}
	}
	return receiveData;
}

void Network::Net::WriteInt(int value) 
{
	_packet->data[_packet->len++] = (value);		
	_packet->data[_packet->len++] = (value >> 0x08);
	_packet->data[_packet->len++] = (value >> 0x10);
	_packet->data[_packet->len++] = (value >> 0x18);
}

int Network::Net::ReadInt(int & byteOffset) 
{
	int value = _packet->data[byteOffset++];
	value |= _packet->data[byteOffset++] << 0x08;
	value |= _packet->data[byteOffset++] << 0x10;
	value |= _packet->data[byteOffset++] << 0x18;
	return value;
}

void Network::Net::WriteFloat(float valueF)
{
	//endianess issues? IEEE 754?
	Uint8* value = reinterpret_cast<Uint8*>(&valueF);
	for (int i = 0; i < sizeof(float); i++)
	{
		_packet->data[_packet->len++] = value[i];
	}
}

float Network::Net::ReadFloat(int & byteOffset)
{
	float value;
	memcpy(&value, _packet->data + byteOffset, sizeof(float));
	byteOffset += sizeof(float);
	return value;
}

void Network::Net::WriteString(std::string & s)
{
	int length = s.length() + 1;
	WriteInt(length); //this adds 4 more bytes? instead, try reading until nul is reached but depends on encoding
	memcpy(_packet->data + _packet->len, s.c_str(), length);
	_packet->len += length;
}

std::string Network::Net::ReadString(int & byteOffset)
{ 
	int length = ReadInt(byteOffset);
	std::stringstream ss;
	for (int i = 0; i < length; i++)
	{
		ss << _packet->data[byteOffset++];
	}
	return ss.str();
}

std::string Network::Net::GetTypeAsString(MessageType type)
{
	std::string s;

	switch (type)
	{
	case MessageType::Connect:
		s = "Connect";
		break;
	case MessageType::Disconnect:
		s = "Disconnect";
		break;
	case MessageType::SessionList:
		s = "SessionList";
		break;
	case MessageType::State:
		s = "State";
		break;
	case MessageType::JoinSession:
		s = "JoinSession";
		break;
	case MessageType::SetHost:
		s = "SetHost";
		break;
	case MessageType::PlayerList:
		s = "PlayerList";
		break;
	case MessageType::Ready:
		s = "Ready";
		break;
	case MessageType::PickUpFlag:
		s = "PickUpFlag";
		break;
	case MessageType::CreatePowerUp:
		s = "CreatePowerUp";
		break;
	case MessageType::Fire:
		s = "Fire";
		break; 
	default:
		s = "No type to string case found.";
		break;
	}
	return s;
}

void Network::Net::WriteBool(bool value)
{
	_packet->data[_packet->len++] = (value);
}

bool Network::Net::ReadBool(int & byteOffset)
{
	int value = _packet->data[byteOffset++];
	return value;
}

