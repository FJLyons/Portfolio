#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>


class AudioManager
{
private:
	enum AudioState
	{
		ERROR = 0,
		WAITING,
		PAUSED,
		STOPPED,
		PLAYING
	};

public:
										AudioManager();
										~AudioManager();

	static AudioManager*				GetInstance();

	void								PlayMusic(const std::string& fileName);
	void								PauseMusic();
	void								StopMusic();
	void								PlayFX(const std::string& fileName);
	void								ClearSounds();

	void								SetMusicVolume(bool volume);
	int									GetMusicVolume();

	void								SetHumVolume(bool volume);
	int									GetHumVolume();

	void								SetWeaponVolume(bool volume);
	int									GetWeaponVolume();

	void								SetCheckpointVolume(bool volume);
	int									GetCheckpointVolume();

	void								SetCollisionVolume(bool volume);
	int									GetCollisionVolume();

	void								SetUIVolume(bool volume);
	int									GetUIVolume();

	bool								IsMusicPlaying();

private:
	void								InitAudioDevice();
	void								LoadSounds();

private:
	static AudioManager*				_audioManagerInstance;
	static AudioState					_currentState;
	std::string							_sCurrentMusicFilename = "";

	std::map<std::string, Mix_Chunk*>	_soundMap;
	std::map<std::string, Mix_Music*>	_musicMap;

	int									_musicVolume;

	int									_humVolume;
	int									_weaponVolume;
	int									_checkpointVolume;
	int									_collisionVolume;
	int									_uiVolume;
};

