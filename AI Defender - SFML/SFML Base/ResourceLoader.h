#pragma once
#define SDL_MAIN_HANDLED
#ifdef __APPLE__

#elif defined(_WIN64) || defined(_WIN32)

#endif

#include "SFML\Graphics.hpp"
#include <SFML/Audio.hpp>

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();

	static ResourceLoader* instance();
	sf::Texture& gethealthTexture();
	sf::Texture& getbombTexture();
	sf::Texture& getwarpTexture();
	sf::Texture& getenemyTexture();
	sf::Texture& getplayershipTexture();
	sf::Texture& getbackgroundTexture();
	sf::Texture& getbulletTexture();
	sf::Texture& gethumanTexture();
	sf::Texture& getnestTexture();
	sf::Texture& getinterceptorTexture();
	sf::Texture& getabductorTexture();
	sf::Texture& getobstacleTexture();
	sf::Texture& getindicatorTexture();
	sf::Texture& getmutantTexture();
	sf::Texture& getenemyBulletTexture();
	sf::Texture& getinstructionsTexture();
	sf::Texture& getpowerTexture();

	sf::Sound menuMove, menuSelect, explosion, hit, shoot, levelUp, back;

	sf::Music musicMenu, musicGame;

private:
	static ResourceLoader* _instance;
	sf::Texture health, bomb, warp, enemy, playershipTexture, backgroundTexture, bulletTexture, humanTexture, nestTexture, 
		interceptorTexture, abductorTexture, obstacleTexture,indicatorTexture,mutantTexture, enemyBulletTexture, instructions, power;

	// Sounds
	sf::SoundBuffer bufferMenuMove, bufferMenuSelect, bufferExplosion, bufferShoot, bufferHit, bufferLevelUp, bufferStart, bufferBack;


};
