#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL_net.h>

namespace Network
{
	enum class MessageType : Uint8 
	{
		Connect,
		Disconnect,
		State,
		MultiState,
		SessionList,
		JoinSession,
		SetHost,
		PlayerList,
		Ready,
		PickUpFlag,
		CreatePowerUp,
		Fire, 
		DroppedFlag,
		CheckConnection,
		Invis,
		Ping
	};

	struct MessageData 
	{
		MessageData() : id(-1), ts(-1.f), sessionID(-1) {};

		MessageType GetType() const 
		{
			return type;
		}

		int id;
		int sessionID;
		float ts;

	protected:
		MessageType type;
	};

	struct StateData : MessageData {
		StateData() : host(false) { type = MessageType::State; }
		float xPos;
		float yPos;
		float xVel;
		float yVel;
		bool host;
		int remoteID;
	};
	struct MultiStateData : MessageData {
		MultiStateData() { type = MessageType::MultiState; }
		int count;
		std::vector<StateData> states;
	};

	struct ConnectData : MessageData 
	{
		ConnectData() { type = MessageType::Connect; }
	};

	struct DisconnectData : MessageData 
	{
		DisconnectData() { type = MessageType::Disconnect; }
	};

	struct SessionListData : MessageData 
	{
		SessionListData() { type = MessageType::SessionList; }
		int count;
		int maxPlayers;
		std::vector<int> sessionIDs;
		std::vector<int> currentPlayers;
	};

	struct JoinSessionData : MessageData {
		JoinSessionData() : host(false) { type = MessageType::JoinSession; }
		bool host;
	};

	struct SetHostData : MessageData {
		SetHostData()  { type = MessageType::SetHost; }
	};

	struct PlayerListData : MessageData 
	{
		PlayerListData() { type = MessageType::PlayerList; }
		int count;
		std::vector<int> players;
	};

	struct ReadyData : MessageData 
	{
		ReadyData() { type = MessageType::Ready; }
		std::vector<int> ids;
		std::vector<bool> ready;
		bool allReady;
		int seed;
		float gameStartTime;
	};

	struct PickUpFlagData : MessageData
	{
		PickUpFlagData() { type = MessageType::PickUpFlag; }
		int remoteID;
	};

	struct DroppedFlagData : MessageData
	{
		DroppedFlagData() { type = MessageType::DroppedFlag; }
		int remoteID;
	};

	struct CreatePowerUpData : MessageData {
		CreatePowerUpData() { type = MessageType::CreatePowerUp; }
		int index;
		int powerType;
	};
	struct FireData : MessageData {
		FireData() { type = MessageType::Fire; }
		int remoteID;
	};

	struct CheckConnectionData : MessageData {
		CheckConnectionData()  { type = MessageType::CheckConnection; }
	};

	struct InvisData : MessageData {
		InvisData() { type = MessageType::Invis; }
		int remoteID;
	};
	struct PingData : MessageData {
		PingData() :  serverTime(0.f) { type = MessageType::Ping; }
		float serverTime;
	};



	//id in MessageData?
	//sessionId in MessageData?

	class ReceivedData 
	{
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
				_data = new ConnectData(rhs.GetData<ConnectData>());
				break;

			case MessageType::Disconnect:
				_data = new DisconnectData(rhs.GetData<DisconnectData>());
				break;

			case MessageType::State:
				_data = new StateData(rhs.GetData<StateData>());
				break;
			case MessageType::MultiState:
				_data = new MultiStateData(rhs.GetData<MultiStateData>());
				break;

			case MessageType::SessionList:
				_data = new SessionListData(rhs.GetData<SessionListData>());
				break;

			case MessageType::JoinSession:
				_data = new JoinSessionData(rhs.GetData<JoinSessionData>());
				break;

			case MessageType::SetHost:
				_data = new SetHostData(rhs.GetData<SetHostData>());
				break;

			case MessageType::PlayerList:
				_data = new PlayerListData(rhs.GetData<PlayerListData>());
				break;

			case MessageType::Ready:
				_data = new ReadyData(rhs.GetData<ReadyData>());
				break;

			case MessageType::PickUpFlag:
				_data = new PickUpFlagData(rhs.GetData<PickUpFlagData>());
				break;

			case MessageType::CreatePowerUp:
				_data = new CreatePowerUpData(rhs.GetData<CreatePowerUpData>());
				break;

			case MessageType::Fire:
				_data = new FireData(rhs.GetData<FireData>());
				break;
			case MessageType::DroppedFlag:
				_data = new DroppedFlagData(rhs.GetData<DroppedFlagData>());
				break;
			case MessageType::CheckConnection:
				_data = new CheckConnectionData(rhs.GetData<CheckConnectionData>());
				break;
			case MessageType::Invis:
				_data = new InvisData(rhs.GetData<InvisData>());
				break;
			case MessageType::Ping:
				_data = new PingData(rhs.GetData<PingData>());
				break;
			}
		}

		template <typename T>
		void SetData(T data)
		{
			_data = new T(data);
			_data->id = _id;			
			_data->sessionID = _sessionID;
			_data->ts = _timeStamp;
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
		void SetTimeStamp(float timeStamp)
		{
			_timeStamp = timeStamp;
		}

	private:
		IPaddress		_srcAddress;
		MessageData*	_data;

		int				_id;
		int				_sessionID;
		float			_timeStamp;
	};

	class Net
	{
	public:
						Net();
						Net(int port, int packetSize = 128);
						~Net();

		void			Send(MessageData* data, const char * destHost, int destPort);
		void			Send(MessageData* data, IPaddress destAddr);

		ReceivedData	Receive();

		//TEMP
		bool			_testSocketCreated;

	private:
		void			WriteInt(int value);
		void			WriteFloat(float valueF);
		void			WriteString(std::string& s);
		void			WriteBool(bool value);

		int				ReadInt(int& byteOffset);
		float			ReadFloat(int & byteOffset);
		bool			ReadBool(int & byteOffset);
		
		std::string		ReadString(int& byteOffset);
		std::string		GetTypeAsString(MessageType type);

	private:
		UDPpacket*		_packet;
		UDPsocket		_socket;
	};
}