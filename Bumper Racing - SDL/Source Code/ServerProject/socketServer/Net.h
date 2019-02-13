#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL_net.h>

namespace Network
{
	enum class MessageType : Uint8 {
		Connect,
		Disconnect,
		State,
		SessionList,
		JoinSession,
		SetHost,
		PlayerList,
		Ready,
		PickUpFlag
	};

	struct MessageData {
		MessageData() : id(-1), sessionID(-1) {};
		int id;
		int sessionID;
		MessageType GetType() const {
			return type;
		}
	protected:
		MessageType type;
	};

	struct StateData : MessageData {
		StateData() { type = MessageType::State; }
		float xPos;
		float yPos;
		float xVel;
		float yVel;
	};
	struct ConnectData : MessageData {
		ConnectData() { type = MessageType::Connect; }
	};

	struct DisconnectData : MessageData {
		DisconnectData() { type = MessageType::Disconnect; }
	};

	struct SessionListData : MessageData {
		SessionListData() { type = MessageType::SessionList; }
		int count;
		int maxPlayers;
		std::vector<int> sessionIDs;
		std::vector<int> currentPlayers;
	};

	struct JoinSessionData : MessageData {
		JoinSessionData() { type = MessageType::JoinSession; }
	};

	struct SetHostData : MessageData {
		SetHostData() { type = MessageType::SetHost; }
	};

	struct PlayerListData : MessageData {
		PlayerListData() { type = MessageType::PlayerList; }
		int count;
		std::vector<int> players;
	};

	struct ReadyData : MessageData {
		ReadyData() { type = MessageType::Ready; }
		std::vector<int> ids;
		std::vector<bool> ready;
		bool allReady;
	};

	struct PickUpFlagData : MessageData {
		PickUpFlagData() { type = MessageType::PickUpFlag; }
	};



	//id in MessageData?
	//sessionId in MessageData?

	class ReceivedData {
	public:
		ReceivedData()
			: _data(nullptr)
		{
		}
		ReceivedData(const ReceivedData& rhs)
		{
			if (!rhs._data)
			{
				return;
			}
			switch (rhs._data->GetType())
			{ //template specialization to use enum type in order to determine Type
			case MessageType::Connect:
			{
				_data = new ConnectData(rhs.GetData<ConnectData>());
				break;
			}
			case MessageType::Disconnect:
			{
				_data = new DisconnectData(rhs.GetData<DisconnectData>());
				break;
			}
			case MessageType::State:
			{
				_data = new StateData(rhs.GetData<StateData>());
				break;
			}
			case MessageType::SessionList:
			{
				_data = new SessionListData(rhs.GetData<SessionListData>());
				break;
			}
			case MessageType::JoinSession:
			{
				_data = new JoinSessionData(rhs.GetData<JoinSessionData>());
				break;
			}
			case MessageType::SetHost:
			{
				_data = new SetHostData(rhs.GetData<SetHostData>());
				break;
			}
			case MessageType::PlayerList:
			{
				_data = new PlayerListData(rhs.GetData<PlayerListData>());
				break;
			}
			case MessageType::Ready:
			{
				_data = new ReadyData(rhs.GetData<ReadyData>());
				break;
			}
			case MessageType::PickUpFlag:
			{
				_data = new PickUpFlagData(rhs.GetData<PickUpFlagData>());
				break;
			}
			}
		}

		template <typename T>
		void SetData(T data)
		{
			_data = new T(data);
			_data->id = _id;
			_data->sessionID = _sessionID;
		}

		~ReceivedData()
		{
			if (_data != nullptr)
			{
				delete _data;
				_data = nullptr;
			}
		}

		bool Empty() const
		{
			return _data == nullptr;
		}

		MessageType GetType() const
		{
			return _data->GetType();
		}

		IPaddress GetSrcAddress() const
		{
			return _srcAddress;
		}

		void SetSrcAddress(const IPaddress& srcAddress)
		{
			_srcAddress = srcAddress;
		}

		template <typename T>
		T GetData() const
		{
			return *static_cast<T*>(_data);
		}

		void SetIDs(int id, int sessionID)
		{
			_id = id;
			_sessionID = sessionID;
		}

	private:
		IPaddress	_srcAddress;
		MessageData* _data;
		int _id;
		int _sessionID;
	};

	class Net
	{
	public:
		Net(int port, int packetSize = 256);
		Net();
		void Send(MessageData* data, const char * destHost, int destPort);
		void Send(MessageData* data, IPaddress destAddr);
		ReceivedData Receive();
		bool	_testSocketCreated;
	private:
		void WriteInt(int value);
		int ReadInt(int& byteOffset);
		void WriteFloat(float valueF);
		float ReadFloat(int & byteOffset);
		void WriteString(std::string& s);
		std::string ReadString(int& byteOffset);
		std::string GetTypeAsString(MessageType type);
		void WriteBool(bool value);
		bool ReadBool(int & byteOffset);

		UDPpacket* _packet;
		UDPsocket _socket;
	};
}