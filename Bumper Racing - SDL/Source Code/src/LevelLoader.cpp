#include "LevelLoader.h"
#include "BasicTypes.h"
#include "Helpers.h"
#include "NetworkHandler.h"

void LevelLoader::LoadJson(const char* path, SystemManager& systemManager, BodyFactory* bodyFactory, Graph* waypoints, const std::vector<int>& ids)
{
	FILE* fp = NULL;
	fopen_s(&fp, path, "rb");
	Document doc;
	if (fp != NULL)
	{
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		doc.ParseStream(is);
	}
	else
	{
		std::cout << "ERROR LOADING : " << path << std::endl;
		system("PAUSE");
		exit(0);
	}
	fclose(fp);

	const Value& layerArray = doc["layers"];

	LoadTiles(layerArray[0u], systemManager,  doc["tilewidth"].GetInt(), doc["tileheight"].GetInt());
	LoadEntities(layerArray[1], systemManager, ids);
	LoadWaypoints(layerArray[2], systemManager, waypoints);
	LoadColliders(layerArray[3], systemManager, bodyFactory);

}

void LevelLoader::LoadTiles(const Value &tileLayer, SystemManager& systemManager, int tileWidth, int tileHeight)
{
	int mapWidth = tileLayer["width"].GetInt();
	int mapHeight = tileLayer["height"].GetInt();

	int index = 0;
	const Value& mapTileDataArray = tileLayer["data"];

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			std::vector<float> data = std::vector<float>();

			data.push_back(mapTileDataArray[index++].GetInt()); //id
			data.push_back(x * tileWidth); //xPosition
			data.push_back(y * tileHeight); //yPosition
			data.push_back(tileWidth); //width
			data.push_back(tileHeight); //height

			systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::Tile, data));
		}
	}
}

void LevelLoader::LoadEntities(const Value &entitiesLayer, SystemManager& systemManager, const std::vector<int>& ids)
{
	const Value& entityDataArray = entitiesLayer["objects"];

	int playerCount = 0;
	bool playerCreated = false;
	int aiID = (ids.empty()) ? -1 : 10000; //ai ids cant be too close to player ids
	for (int i = 0; i < entityDataArray.Size(); i++)
	{
		const Value& entity = entityDataArray[i];
		string entityName = entity["name"].GetString();

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();
		float w = entity["width"].GetFloat();
		float h = entity["height"].GetFloat();
		if (entityName == "PlayerSolo")
		{

			//TODO: use ids here to determine type AI/Player/RemotePlayer, then use other stuff to create them
			if (playerCount < ids.size())
			{
				if (ids[playerCount] == NetworkHandler::Instance().GetPlayerID())
				{
					std::vector<float> data = std::vector<float>();

					data.push_back(NetworkHandler::Instance().GetPlayerID()); //id
					data.push_back(x); //xPosition
					data.push_back(y); //yPosition
					data.push_back(w); //width
					data.push_back(h); //height

					systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::Player, data));
				}
				else
				{
					std::vector<float> data = std::vector<float>();

					data.push_back(ids[playerCount]); //id
					data.push_back(x); //xPosition
					data.push_back(y); //yPosition
					data.push_back(w); //width
					data.push_back(h); //height

					systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::RemotePlayer, data));
				}
				playerCount++;
			}
			else if (ids.size() == 0 && playerCreated == false) //no remote players so first is player
			{
				playerCreated = true;

				std::vector<float> data = std::vector<float>();

				data.push_back(-1);
				data.push_back(x); //xPosition
				data.push_back(y); //yPosition
				data.push_back(w); //width
				data.push_back(h); //height

				systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::Player, data));

			}
			else //fill rest with ai
			{
				std::vector<float> data = std::vector<float>();

				if (aiID == -1)
				{
					data.push_back(aiID); //id
				}
				else
				{
					data.push_back(aiID++); //id
				}
				data.push_back(x); //xPosition
				data.push_back(y); //yPosition
				data.push_back(w); //width
				data.push_back(h); //height

				systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::AI, data));
			}

		}
		else if (entityName == "Checkpoint")
		{
			std::string dataString = entity["type"].GetString();
			std::vector<float> data = std::vector<float>();

			data.push_back(atoi(dataString.c_str())); //id
			data.push_back(x); //xPosition
			data.push_back(y); //yPosition
			data.push_back(w); //width
			data.push_back(h); //height

			systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::Checkpoint, data));
		}
		else if (entityName == "Flag")
		{
			std::vector<float> data = std::vector<float>();

			data.push_back((ids.empty()) ? -1 : 20000); //id -1 if not multiplayer else number that isnt remote player or ai id
			data.push_back(x); //xPosition
			data.push_back(y); //yPosition
			data.push_back(w); //width
			data.push_back(h); //height

			systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::Flag, data));
		}
	}
}

void LevelLoader::LoadWaypoints(const Value &waypointLayer, SystemManager& systemManager, Graph* waypoints)
{
	//node
	const Value& wayPointArray = waypointLayer["objects"];
	int wayPointSize = wayPointArray.Size();

	for (int i = 0; i < wayPointSize; i++)
	{
		const Value& entity = wayPointArray[i];

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();
		float w = entity["width"].GetFloat();
		float h = entity["height"].GetFloat();

		helper::Vector2 position = helper::Vector2(x + w*.5f, y + h*.5f);

		const Value& properties = entity["properties"];

		std::string dataString = entity["type"].GetString();
		pair<GraphNode::EntityData, int> nodeData;
		if (dataString.empty())
		{
			nodeData = make_pair(GraphNode::EntityData::Null, 0);
		}
		else
		{
			nodeData = make_pair(GraphNode::EntityData::Checkpoint, atoi(dataString.c_str()));
		}
		waypoints->addNode(nodeData, position);
	}

	//arc
	for (int i = 0; i < wayPointSize; i++)
	{
		const Value& entity = wayPointArray[i];
		const Value& properties = entity["properties"];
		const Value& neighbour = properties["neighbour"];

		int fromNode = properties["node"].GetInt();

		for (int j = 0; j< neighbour.Size(); j++)
		{
			int toNode = neighbour[j].GetInt();
			int lenght = (waypoints->getNodes()[fromNode]->getPosition() - waypoints->getNodes()[toNode]->getPosition()).length();
			waypoints->addArc(fromNode, toNode, lenght, false);
		}
	}
	//flag node
	waypoints->addNode(make_pair(GraphNode::EntityData::Flag, 0), helper::Vector2(0, 0));
}

void LevelLoader::LoadColliders(const Value &colliderLayer, SystemManager& systemManager, BodyFactory* bodyFactory)
{
	const Value& colliderDataArray = colliderLayer["objects"];

	for (int i = 0; i < colliderDataArray.Size(); i++)
	{
		const Value& entity = colliderDataArray[i];
		string entityName = entity["name"].GetString();

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();

		if (entityName == "Box")
		{
			float w = entity["width"].GetFloat();
			float h = entity["height"].GetFloat();

			b2Body* body = bodyFactory->CreateBoxBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x + w*.5f, y + h*.5f)
				, b2Vec2(w*.5f, h*.5f)
				, (uint16)EntityType::Obstacle
				, OBSTACLE_MASK
				, false);//create body

			body->SetUserData("Obstacle");
		}
		else if (entityName == "Poly")
		{
			const Value& entityPolyArray = entity["polygon"]; //loop json entity
			int count = entityPolyArray.Size();

			b2Vec2* vertices = new b2Vec2[count];
			for (int i = 0; i < count; i++)
			{
				const Value& entityPoly = entityPolyArray[i];
				vertices[i].Set(pixelsToMeters(entityPoly["x"].GetFloat()), pixelsToMeters(entityPoly["y"].GetFloat()));
			}

			b2Body* body = bodyFactory->CreatePolyBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x, y)
				, vertices
				, count
				, (uint16)EntityType::Obstacle
				, OBSTACLE_MASK
				, false);

			body->SetUserData("Obstacle");
		}
	}
}