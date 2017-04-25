#include "GameData.h"

std::weak_ptr<GameData> GameData::m_instance;

GameData::GameData()
{

}

std::shared_ptr<GameData> GameData::sharedGameData()
{
	// http://pseudorandombits.com/posts/theres-more-than-one-way-to-skin-a-singleton/
	
	std::shared_ptr<GameData> ptr = m_instance.lock();
	if (!ptr)
	{
		ptr = std::make_shared<GameData>(GameData());
		m_instance = std::weak_ptr<GameData>(ptr);
	}
	return ptr;
}