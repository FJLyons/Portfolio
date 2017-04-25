#ifndef LEVELOADER_H
#define LEVELOADER_H

//#include <fstream>
#include <memory>
#include "cocos2d.h" // For FileUtils
#include "GameData.h"
#include "json/document.h"

using namespace rapidjson;

////////////////////////////////////////////////////////////
/// \brief A class to load all the configuration data
///        required by the game. 
///
////////////////////////////////////////////////////////////

class LevelLoader {
private:	
	Document m_document;	// A container for a JSON document
	std::string m_JSONData;
	

	void loadJSONData(std::string const & filename);

	void loadGameData();

	std::string wStringToString(std::wstring const & wstr);

	std::string SerializeJsonToString(rapidjson::Value & val);

public:
	/// This constructor initialises the JSON parser and loads
	/// the menu and game level data.
	/// \param filename The path and filename of the JSON file.
	LevelLoader(std::string const & filename);

	
	/// \brief Loads the JSON data from the specified file.
	/// \param filename The path and filename of the JSON file.
	void load(std::string const & filename);

};

#endif // LEVELOADER_H