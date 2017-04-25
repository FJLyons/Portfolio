#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <memory>
////////////////////////////////////////////////////////////
/// \brief A class that stores gameplay data
/// 
////////////////////////////////////////////////////////////


class GameData {
private:
	//Constructor
	GameData();

	static std::weak_ptr<GameData> m_instance;

	friend std::weak_ptr < GameData >;
	friend std::shared_ptr < GameData >;
public:
	//Get instance of singleton
	static std::shared_ptr<GameData> sharedGameData();

	// Options
	// Screen Options
	std::string m_TVGlow;
	std::string m_tetrisTutorialFire;
	std::string m_tetrisTutorialMove;
	bool m_defaultTVGlow;
	bool m_defaultTutorial;
	bool m_defaultMusic;
	bool isTetris = true;

	// Tetris
	int m_tetrisTimeLimit;
	int m_tetrisScore = 0; // add to JSON
	bool m_punish = false; // add to JSON

	// Breakout
	int m_breakoutTimeLimit;
	int m_breakoutScore = 0; // add to JSON
	int m_breakoutPlayerLives = 100; // add to JSON
	int m_breakoutEnemyLives = 100; // add to JSON
	std::string m_paddlePlistFile;
	std::string m_enemyPlistFile;
	int m_damageLives = 1; // add to JSON

	// HighScore
	int m_tetrisHighScore = 0;
	int m_breakoutHighScore = 0;
};

#endif // GAMEDATA_H

	//std::string m_backgroundTextureFile;
	//std::string m_textureAtlasImageFile;
	//std::string m_textureAtlasPlistFile;
	//// Tower base and tower related fields.
	//std::string m_towerBaseFile;
	//std::string m_towerGunFile;
	//int m_towerWidth;
	//int m_towerHeight;
	//int m_firingDelay;
	//int m_towerRotationSpeed;
	//float m_towerRotationOriginX;
	//float m_towerRotationOriginY;
	//int m_numberOfTowerBases;

	//float m_towerBaseX[3];
	//float m_towerBaseY[3];

	//// Projectile related fields.
	//std::string m_projectileFile;
	//float m_projectileCentreX;
	//float m_projectileCentreY;
	//int m_maxNumProjectiles;
	//int m_ttl;
	//float m_velocity;

	//// AI related fields.
	//std::string m_aiImageFile;
	//std::string m_aiPlistFile;
	//int m_maxNumAI;
	//int m_spawnInterval;
	//float m_aiScale;
	//float m_aiVelocity;
	//int m_aiTimeDelay;
	//int m_aiStartX;
	//int m_aiStartY;
	//// AI sprite sheet.
	//int m_aiRows;
	//int m_aiColumns;
	//int m_aiWidth;
	//int m_aiHeight;
	//// AI Waypoints.
	//int m_numWaypoints;
	//float m_aiWPX[12];
	//float m_aiWPY[12];
