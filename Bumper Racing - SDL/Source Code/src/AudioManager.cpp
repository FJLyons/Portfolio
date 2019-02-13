#include "AudioManager.h"

//#include "ResourceManager.h"
//#include "utilities.h"

// Initialize our static variables
AudioManager* AudioManager::_audioManagerInstance = 0;
AudioManager::AudioState AudioManager::_currentState = ERROR;

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	Mix_CloseAudio();
}

// Singleton pattern
AudioManager* AudioManager::GetInstance()
{
	if (_audioManagerInstance == 0)
	{
		_audioManagerInstance = new AudioManager;
		_audioManagerInstance->InitAudioDevice();
	}
	return _audioManagerInstance;
}

void AudioManager::InitAudioDevice()
{
	if (SDL_Init(SDL_INIT_AUDIO) != -1)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			std::cerr << "Error initializing audio device...\n";
			_currentState = ERROR;
		}
		else
		{
			_currentState = WAITING;
			Mix_AllocateChannels(16);
		}
	}
	else
	{
		std::cerr << "Error initializing SDL audio subsystem...\n";
		_currentState = ERROR;
	}
	LoadSounds();
}

void AudioManager::LoadSounds()
{
	_musicMap.insert(std::make_pair("MusicMenu", Mix_LoadMUS("Media/Audio/Music_Menu.ogg")));
	_musicMap.insert(std::make_pair("Music1", Mix_LoadMUS("Media/Audio/Music_Level_1.ogg")));
	_musicMap.insert(std::make_pair("Music2", Mix_LoadMUS("Media/Audio/Music_Level_2.ogg")));
	_musicMap.insert(std::make_pair("Music3", Mix_LoadMUS("Media/Audio/Music_Level_3.ogg")));
	_musicVolume = 64;
	Mix_VolumeMusic(_musicVolume);

	_soundMap.insert(std::make_pair("Hum", Mix_LoadWAV("Media/Audio/Hum.ogg")));
	_humVolume = 64;
	Mix_VolumeChunk(_soundMap.at("Hum"), _humVolume);

	_soundMap.insert(std::make_pair("Weapon", Mix_LoadWAV("Media/Audio/Weapon.ogg")));
	_weaponVolume = 64;
	Mix_VolumeChunk(_soundMap.at("Weapon"), _weaponVolume);

	_soundMap.insert(std::make_pair("Checkpoint", Mix_LoadWAV("Media/Audio/Checkpoint.ogg")));
	_checkpointVolume = 64;
	Mix_VolumeChunk(_soundMap.at("Checkpoint"), _checkpointVolume);

	_soundMap.insert(std::make_pair("Collision", Mix_LoadWAV("Media/Audio/Collision.ogg")));
	_collisionVolume = 64;
	Mix_VolumeChunk(_soundMap.at("Collision"), _collisionVolume);

	_soundMap.insert(std::make_pair("Enter", Mix_LoadWAV("Media/Audio/Enter.ogg")));
	_soundMap.insert(std::make_pair("Click", Mix_LoadWAV("Media/Audio/Click.ogg")));
	_uiVolume = 64;
	Mix_VolumeChunk(_soundMap.at("Enter"), _collisionVolume);
	Mix_VolumeChunk(_soundMap.at("Click"), _collisionVolume);
}

void AudioManager::PlayMusic(const std::string& fileName)
{
	if (_currentState != ERROR)
	{
		// If no music is playing, play it
		if (Mix_PlayingMusic() == 0)
		{
			if (_musicMap.at(fileName) == NULL)
			{
				printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			}
			//Play music
			Mix_PlayMusic(_musicMap.at(fileName), -1);
		}
		else
		{
			// If music is playing, pause it
			this->PauseMusic();
		}
	}
}

void AudioManager::PauseMusic()
{
	if (_currentState != ERROR)
	{
		// If music is playing, handle the pause request
		if (Mix_PlayingMusic() == 1)
		{
			if (Mix_PausedMusic() == 1)
			{
				// If we receive a pause request and the music is already paused, resume it.
				Mix_ResumeMusic();
				_currentState = PLAYING;
			}
			else
			{
				// Otherwise, pause the music
				Mix_PauseMusic();
				_currentState = PAUSED;
			}
		}
	}
}

void AudioManager::StopMusic()
{
	if (_currentState != ERROR)
	{
		Mix_HaltMusic();
		_currentState = STOPPED;
	}
}

void AudioManager::PlayFX(const std::string& fileName)
{
	if (_currentState != ERROR)
	{
		int SChannel = 0;
		if (fileName == "Hum") { SChannel = 1; }
		else if (fileName == "Checkpoint") { SChannel = 2; }
		else if (fileName == "Enter") { SChannel = 3; }
		else if (fileName == "Click") { SChannel = 4; }
		else if (fileName == "Collision") { SChannel = rand() % 6 + 5; }
		else if (fileName == "Weapon") { SChannel =  rand() % 7 + 10; }

		if (_soundMap.at(fileName) == NULL)
		{
			_currentState = ERROR;
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}

		else if(Mix_Playing(SChannel) == 0)
		{
			Mix_PlayChannel(SChannel, _soundMap.at(fileName), 0);
			_currentState = PLAYING;
		}
	}
}

void AudioManager::ClearSounds()
{
	if (_soundMap.size() > 0)
	{
		for (auto& sound : _soundMap)
		{
			Mix_FreeChunk(sound.second);
		}
		_soundMap.empty();
		_soundMap.clear();
	}
}

void AudioManager::SetMusicVolume(bool volume)
{
	if (!volume)
	{
		if (_musicVolume >= 0 + 4) { _musicVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeMusic(_musicVolume));
	}

	else if (volume)
	{
		if (_musicVolume <= 128 - 4) { _musicVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeMusic(_musicVolume));
	}
}

int AudioManager::GetMusicVolume()
{
	return _musicVolume;
}

void AudioManager::SetHumVolume(bool volume)
{
	if (!volume)
	{
		if (_humVolume >= 0 + 4) { _humVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Hum"), _humVolume));
	}

	else if (volume)
	{
		if (_humVolume <= 128 - 4) { _humVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Hum"), _humVolume));
	}
}

int AudioManager::GetHumVolume()
{
	return _humVolume;
}

void AudioManager::SetWeaponVolume(bool volume)
{
	if (!volume)
	{
		if (_weaponVolume >= 0 + 4) { _weaponVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Weapon"), _weaponVolume));
	}

	else if (volume)
	{
		if (_weaponVolume <= 128 - 4) { _weaponVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Weapon"), _weaponVolume));
	}
}

int AudioManager::GetWeaponVolume()
{
	return _weaponVolume;
}

void AudioManager::SetCheckpointVolume(bool volume)
{
	if (!volume)
	{
		if (_checkpointVolume >= 0 + 4) { _checkpointVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Checkpoint"), _checkpointVolume));
	}

	else if (volume)
	{
		if (_checkpointVolume <= 128 - 4) { _checkpointVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Checkpoint"), _checkpointVolume));
	}
}

int AudioManager::GetCheckpointVolume()
{
	return _checkpointVolume;
}

void AudioManager::SetCollisionVolume(bool volume)
{
	if (!volume)
	{
		if (_collisionVolume >= 0 + 4) { _collisionVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Collision"), _collisionVolume));
	}

	else if (volume)
	{
		if (_collisionVolume <= 128 - 4) { _collisionVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Collision"), _collisionVolume));
	}
}

int AudioManager::GetCollisionVolume()
{
	return _collisionVolume;
}

void AudioManager::SetUIVolume(bool volume)
{
	if (!volume)
	{
		if (_uiVolume >= 0 + 4) { _uiVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Enter"), _uiVolume));
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Click"), _uiVolume));
	}

	else if (volume)
	{
		if (_uiVolume <= 128 - 4) { _uiVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Enter"), _uiVolume));
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Click"), _uiVolume));
	}
}

int AudioManager::GetUIVolume()
{
	return _uiVolume;
}

bool AudioManager::IsMusicPlaying()
{
	return Mix_PlayingMusic();
}
