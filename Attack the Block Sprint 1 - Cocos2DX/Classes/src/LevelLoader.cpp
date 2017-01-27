#include "LevelLoader.h"

#include <string>
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"

using namespace std;

#define COCOS2D_DEBUG 1;


////////////////////////////////////////////////////////////
LevelLoader::LevelLoader(std::string const & filename) 
{
	loadJSONData(filename);
	m_document.Parse<0>(m_JSONData.c_str());	
	loadGameData();
}

////////////////////////////////////////////////////////////
void LevelLoader::loadJSONData(std::string const & filename)
{
	m_JSONData.clear();

	auto fileUtils = cocos2d::FileUtils::getInstance();
	// Loads all the data in the file into a std::string.
	m_JSONData = fileUtils->getStringFromFile(filename);
}


////////////////////////////////////////////////////////////
void LevelLoader::loadGameData()
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	Value::ConstMemberIterator it1 = m_document["settings"].MemberonBegin();
	assert(it1->value.IsString());
	ptr->m_TVGlow = it1->value.GetString(); // TV Glow
	it1++;
	ptr->m_tetrisTutorialFire = it1->value.GetString(); // Fire tutorial
	it1++;
	ptr->m_tetrisTutorialMove = it1->value.GetString(); // Move tutorial
	it1++;

	// Tetris
	assert(it1->value.IsInt());
	ptr->m_tetrisTimeLimit = it1->value.GetInt();
	it1++;

	// Breakout
	ptr->m_breakoutTimeLimit = it1->value.GetInt();
	it1++;

	assert(it1->value.IsBool());
	ptr->m_defaultTVGlow = it1->value.GetBool();
	it1++;	
	ptr->m_defaultTutorial = it1->value.GetBool();
	it1++;
	ptr->m_defaultMusic = it1->value.GetBool();
	it1++;

	assert(it1->value.IsString());
	ptr->m_paddlePlistFile = it1->value.GetString();
	it1++;
	ptr->m_enemyPlistFile = it1->value.GetString();
	it1++;
	
}

string LevelLoader::SerializeJsonToString(rapidjson::Value & val)
{
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	val.Accept(writer);
	string json = buffer.GetString();
	return json;
}



//Value myValue(kObjectType);
//myValue = m_document["settings"];

//const char* json = SerializeJsonToString(myValue).c_str();
//CCLOG(json);

//assert(it1->value.IsString());
//ptr->m_paddlePlistFile = "BreakoutScene/enemyPlist.plist";
//it1++;

//Value const & tower = it1->value;
//assert(tower.IsObject());
//// Get the tower properties.
//ptr->m_towerBaseFile = tower["tower_base_file"].GetString();
//ptr->m_towerGunFile = tower["tower_gun_file"].GetString();
//ptr->m_towerWidth = tower["tower_width"].GetInt();
//ptr->m_towerHeight = tower["tower_height"].GetInt();
//ptr->m_firingDelay = tower["firing_delay"].GetInt();
//ptr->m_towerRotationSpeed = tower["tower_rotation_speed"].GetInt();
//
//// The "tower_rotation_origin" element is an object.
//Value const & towerRotationOrigin = tower["tower_rotation_origin"];
//ptr->m_towerRotationOriginX = towerRotationOrigin["x"].GetInt();
//ptr->m_towerRotationOriginY = towerRotationOrigin["y"].GetInt();
//ptr->m_numberOfTowerBases = tower["number_of_tower_bases"].GetInt();
//
//// The "tower_bases" element is an array.
//Value const & towerBases = tower["tower_bases"];
//// Iterate through the array.
//for (int i = 0; i < towerBases.Size(); i++) {
//	ptr->m_towerBaseX[i] = towerBases[i]["x"].GetInt();
//	ptr->m_towerBaseY[i] = towerBases[i]["y"].GetInt();
//}
//
//// Move to the "projectile" element, which is another object.
//it1++;
//Value const & projectile = it1->value;
//ptr->m_projectileFile = projectile["projectile_file"].GetString();
//// The "projectile_centre" element is an object.	
//Value const & projectileCentre = projectile["projectile_centre"];
//ptr->m_projectileCentreX = projectileCentre["x"].GetInt();
//ptr->m_projectileCentreY = projectileCentre["y"].GetInt();
//// Get the remaining projectile attributes.
//ptr->m_maxNumProjectiles = projectile["max_num_projectiles"].GetInt();
//ptr->m_ttl = projectile["time_to_live"].GetInt();
//ptr->m_velocity = projectile["velocity"].GetInt();
//
//// Move to the "ai" element
//it1++;
//Value const & ai = it1->value;
//ptr->m_aiImageFile = ai["ai_image_file"].GetString();
//ptr->m_aiPlistFile = ai["ai_plist_file"].GetString();
//ptr->m_maxNumAI = ai["max_num_ai"].GetInt();
//ptr->m_spawnInterval = ai["ai_spawn_interval"].GetInt();
//ptr->m_aiScale = ai["ai_scale"].GetDouble();
//ptr->m_aiVelocity = ai["ai_velocity"].GetDouble();
//ptr->m_aiTimeDelay = ai["ai_time_delay"].GetInt();
//// The "ai_start_position" element is an object.
//Value const & aiStartPosition = ai["ai_start_position"];
//ptr->m_aiStartX = aiStartPosition["x"].GetInt();
//ptr->m_aiStartY = aiStartPosition["y"].GetInt();
//// The "ai_walking_spritesheet" element is an object.
//Value const & aiWalkingSpriteSheet = ai["ai_walking_spritesheet"];
//ptr->m_aiRows = aiWalkingSpriteSheet["rows"].GetInt();
//ptr->m_aiColumns = aiWalkingSpriteSheet["columns"].GetInt();
//ptr->m_aiWidth = aiWalkingSpriteSheet["width"].GetInt();
//ptr->m_aiHeight = aiWalkingSpriteSheet["height"].GetInt();
//// The "ai_waypoints" element is an array.
//Value const & aiWaypoints = ai["ai_waypoints"];
//// Iterate through the array.
//for (int i = 0; i < aiWaypoints.Size(); i++) {
//	ptr->m_aiWPX[i] = aiWaypoints[i]["x"].GetInt();
//	ptr->m_aiWPY[i] = aiWaypoints[i]["y"].GetInt();
//}
//ptr->m_numWaypoints = aiWaypoints.Size();
//}


