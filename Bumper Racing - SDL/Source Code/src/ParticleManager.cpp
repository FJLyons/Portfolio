#pragma once

#include "ParticleManager.h"
//#include "TextureLoader.h"

ParticleManager::ParticleSettings ParticleManager::_FOOTSTEPS_PRESET;
ParticleManager::ParticleSettings ParticleManager::_ROCKET_THRUSTER_PRESET;
ParticleManager::ParticleSettings ParticleManager::_TRON_PRESET;
ParticleManager::ParticleSettings ParticleManager::_STAR_PRESET;


ParticleManager::ParticleManager(ParticleSettings pSettings, SDL_Renderer * pRenderer)
	:_minTTL(pSettings._minTTL),
	_maxTTL(pSettings._maxTTL),
	_particleSize(pSettings._particleSize),
	_colourLerpingList(pSettings._coloursToLerp),
	_particleVelVariation(pSettings._velVariation),
	_startingVelocity(pSettings._startingVelocity),
	_endingVelocity(pSettings._endingVelocity),
	_shapeType(pSettings._shapeType),
	_emissionRate(pSettings._emissionRate),
	_texture(pSettings._texture),
	_shouldSystemEmit(true),
	_particleList(std::vector<ParticleObj *>()),
	_positionToParentTo(pSettings._positionToParentTo),
	_currentVelocity(0, 0),
	_timeSinceEmit(0),
	_renderer(pRenderer),
	_rotationSpeed(pSettings._rotationSpeed),
	_timeToTurnOn(-1),
	_timeSinceTurnedOn(0),
	_startingParticleDirection(pSettings._startingDirection),
	_offset(pSettings._offsetFromParent)

{
	float timeToLiveTotal = 0;

	if (_positionToParentTo == nullptr)
		_positionToParentTo = new Vector2b(300, 300);

	if (_colourLerpingList.size() == 0)
	{
		ColourLerper white = ColourLerper();
		white._colour = { 255,255,255,255 };
		white._durationOfColour = 2;

		_colourLerpingList.push_back(white);
	}

	if (_minTTL == -1 && _maxTTL == -1)
	{
		for (int i = 0; i < _colourLerpingList.size(); ++i)
		{
			timeToLiveTotal += _colourLerpingList.at(i)._durationOfColour;
		}

		_minTTL = timeToLiveTotal - pSettings._timeToLiveVariation;
		_maxTTL = timeToLiveTotal;
	}
}


void ParticleManager::update(float pDT, Vector2b parentPos)
{
	if (_timeToTurnOn != -1)
	{
		_shouldSystemEmit = true;
		_timeSinceTurnedOn += pDT;
		if (_timeSinceTurnedOn > _timeToTurnOn)
		{
			_timeToTurnOn = -1;
			_timeSinceTurnedOn = 0;
			_shouldSystemEmit = false;
		}
	}

	_positionToParentTo = &parentPos;
	Vector2b diff = _position - (*_positionToParentTo + _offset);

	if (diff.Length() > 1)
	{
		if (_velocityList.size() < 3)
			_velocityList.push_back(diff);
		else
		{
			_velocityList.insert(_velocityList.begin(), diff);
			_velocityList.pop_back();
		}
		Vector2b totalSpeed = Vector2b(0, 0);
		for (int i = 0; i < _velocityList.size(); ++i)
		{
			totalSpeed = totalSpeed + _velocityList.at(i);
		}

		_currentVelocity = totalSpeed / static_cast<float>(_velocityList.size());

		_position = *_positionToParentTo + _offset;
	}


	if (_shouldSystemEmit == true)
	{
		_timeSinceEmit += pDT;

		while (_timeSinceEmit > _emissionRate)
		{
			SpawnParticle(_currentVelocity.Normalize());
			_timeSinceEmit -= _emissionRate;
		}
	}

	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i]->update(pDT);

		if (_particleList[i]->readyToRespawn())
		{
			delete _particleList.at(i);
			_particleList.erase(_particleList.begin() + i);
		}
	}
}

void ParticleManager::LoadPresets(SDL_Renderer * pRenderer = nullptr)
{
	_FOOTSTEPS_PRESET = ParticleSettings();
	_FOOTSTEPS_PRESET._particleSize = 25;
	_FOOTSTEPS_PRESET._emissionRate = 0.424f;
	_FOOTSTEPS_PRESET._startingVelocity = 0;
	_FOOTSTEPS_PRESET._endingVelocity = 0;
	_FOOTSTEPS_PRESET._shapeType = Shape::Footsteps;
	//_FOOTSTEPS_PRESET._texture = TextureLoader::loadTexture("assets/footsteps.png", pRenderer);

	_ROCKET_THRUSTER_PRESET = ParticleSettings();
	_ROCKET_THRUSTER_PRESET._particleSize = 3;
	_ROCKET_THRUSTER_PRESET._emissionRate = 0.005f;
	_ROCKET_THRUSTER_PRESET._startingVelocity = 150;
	_ROCKET_THRUSTER_PRESET._endingVelocity = 0;
	_ROCKET_THRUSTER_PRESET._velVariation = 0.3f;
	_ROCKET_THRUSTER_PRESET._shapeType = Shape::RocketThruster;

	_ROCKET_THRUSTER_PRESET._coloursToLerp.clear();

	ColourLerper firstLerp;
	firstLerp._colour = { 255, 255, 255, 255 };
	firstLerp._durationOfColour = 0.5f;
	_ROCKET_THRUSTER_PRESET._coloursToLerp.push_back(firstLerp);

	ColourLerper secondLerp;
	secondLerp._colour = { 255, 160, 37, 255 };
	secondLerp._durationOfColour = 0.8f;
	_ROCKET_THRUSTER_PRESET._coloursToLerp.push_back(secondLerp);

	ColourLerper thirdLerp;
	thirdLerp._colour = { 100, 100, 100, 0 };
	thirdLerp._durationOfColour = 1.2f;
	_ROCKET_THRUSTER_PRESET._coloursToLerp.push_back(thirdLerp);


	_TRON_PRESET = ParticleSettings();
	_TRON_PRESET._particleSize = 6;
	_TRON_PRESET._emissionRate = 0.001f;
	_TRON_PRESET._startingVelocity = 0;
	_TRON_PRESET._endingVelocity = 0;
	_TRON_PRESET._velVariation = 0.0f;
	_TRON_PRESET._minTTL = 4;
	_TRON_PRESET._maxTTL = 4;
	_TRON_PRESET._shapeType = Shape::Tron;
	//_TRON_PRESET._texture = TextureLoader::loadTexture("assets/laser.png", pRenderer);

	_STAR_PRESET = ParticleSettings();
	_STAR_PRESET._particleSize = 2.5f;
	_STAR_PRESET._emissionRate = 0.005f;
	_STAR_PRESET._startingVelocity = 150;
	_STAR_PRESET._endingVelocity = 0;
	_STAR_PRESET._minTTL = 2;
	_STAR_PRESET._maxTTL = 4;
	_STAR_PRESET._velVariation = 1;
	_STAR_PRESET._shapeType = Shape::StarPreset;
}
void ParticleManager::LoadPresets(SDL_Texture * footstepsTex)
{
	_FOOTSTEPS_PRESET = ParticleSettings();
	_FOOTSTEPS_PRESET._particleSize = 25;
	_FOOTSTEPS_PRESET._emissionRate = 0.424f;
	_FOOTSTEPS_PRESET._startingVelocity = 0;
	_FOOTSTEPS_PRESET._endingVelocity = 0;
	_FOOTSTEPS_PRESET._shapeType = Shape::Footsteps;
	_FOOTSTEPS_PRESET._texture = footstepsTex;

	_ROCKET_THRUSTER_PRESET = ParticleSettings();
	_ROCKET_THRUSTER_PRESET._particleSize = 3;
	_ROCKET_THRUSTER_PRESET._emissionRate = 0.005f;
	_ROCKET_THRUSTER_PRESET._startingVelocity = 150;
	_ROCKET_THRUSTER_PRESET._endingVelocity = 0;
	_ROCKET_THRUSTER_PRESET._velVariation = 0.3f;
	_ROCKET_THRUSTER_PRESET._shapeType = Shape::RocketThruster;

	_ROCKET_THRUSTER_PRESET._coloursToLerp.clear();

	ColourLerper firstLerp;
	firstLerp._colour = { 255, 255, 255, 255 };
	firstLerp._durationOfColour = 0.5f;
	_ROCKET_THRUSTER_PRESET._coloursToLerp.push_back(firstLerp);

	ColourLerper secondLerp;
	secondLerp._colour = { 255, 160, 37, 255 };
	secondLerp._durationOfColour = 0.8f;
	_ROCKET_THRUSTER_PRESET._coloursToLerp.push_back(secondLerp);

	ColourLerper thirdLerp;
	thirdLerp._colour = { 100, 100, 100, 0 };
	thirdLerp._durationOfColour = 1.2f;
	_ROCKET_THRUSTER_PRESET._coloursToLerp.push_back(thirdLerp);

	_STAR_PRESET = ParticleSettings();
	_STAR_PRESET._particleSize = 2.5f;
	_STAR_PRESET._emissionRate = 0.005f;
	_STAR_PRESET._startingVelocity = 150;
	_STAR_PRESET._endingVelocity = 0;
	_STAR_PRESET._minTTL = 2;
	_STAR_PRESET._maxTTL = 4;
	_STAR_PRESET._velVariation = 1;
	_STAR_PRESET._shapeType = Shape::StarPreset;
}

SDL_Color ParticleManager::GetColour(float pAliveTime)
{
	if (_colourLerpingList.size() == 1)
		return _colourLerpingList.at(0)._colour;

	float totalTime = 0;
	int indexOfColour = 0;
	while (totalTime < pAliveTime && indexOfColour < _colourLerpingList.size())
	{
		totalTime += _colourLerpingList.at(indexOfColour)._durationOfColour;
		indexOfColour++;
	}

	if (indexOfColour == _colourLerpingList.size())
		return{ 0, 0, 0, 0 };

	indexOfColour -= 1;
	if (indexOfColour < 0)
		indexOfColour = 0;

	float percentThroughThisLerp = 1 - ((totalTime - pAliveTime) / _colourLerpingList.at(indexOfColour)._durationOfColour);

	float lerpedR = _colourLerpingList.at(indexOfColour)._colour.r + ((_colourLerpingList.at(indexOfColour + 1)._colour.r - _colourLerpingList.at(indexOfColour)._colour.r)) * percentThroughThisLerp;
	float lerpedG = _colourLerpingList.at(indexOfColour)._colour.g + ((_colourLerpingList.at(indexOfColour + 1)._colour.g - _colourLerpingList.at(indexOfColour)._colour.g)) * percentThroughThisLerp;
	float lerpedB = _colourLerpingList.at(indexOfColour)._colour.b + ((_colourLerpingList.at(indexOfColour + 1)._colour.b - _colourLerpingList.at(indexOfColour)._colour.b)) * percentThroughThisLerp;
	float lerpedA = _colourLerpingList.at(indexOfColour)._colour.a + ((_colourLerpingList.at(indexOfColour + 1)._colour.a - _colourLerpingList.at(indexOfColour)._colour.a)) * percentThroughThisLerp;

	return{ static_cast<Uint8>(lerpedR), static_cast<Uint8>(lerpedG), static_cast<Uint8>(lerpedB), static_cast<Uint8>(lerpedA) };
}


void ParticleManager::render(SDL_Renderer * pRenderer, Camera2D::Camera* camera)
{
	if (_renderer == nullptr)
		_renderer = pRenderer;

	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i]->render(pRenderer, camera);
	}
}

void ParticleManager::TimedTurnOn(float pTime)
{
	_timeToTurnOn = pTime;
}

void ParticleManager::SpawnParticle(Vector2b pDir)
{
	float randX = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation) - _particleVelVariation / 2;
	float randY = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation) - _particleVelVariation / 2;

	float xVel;
	float yVel;
	float endingXVel;
	float endingYVel;
	
	if(_startingParticleDirection == nullptr)
	{
		xVel = (pDir.x - randX) * _startingVelocity;
		yVel = (pDir.y - randY) * _startingVelocity;

		endingXVel = (pDir.x - randX) * _endingVelocity;
		endingYVel = (pDir.y - randY) * _endingVelocity;
	}
	else
	{
		xVel = (_startingParticleDirection->x - randX) * _startingVelocity;
		yVel = (_startingParticleDirection->y - randY) * _startingVelocity;

		endingXVel = (_startingParticleDirection->x - randX) * _endingVelocity;
		endingYVel = (_startingParticleDirection->y - randY) * _endingVelocity;
	}

	
	

	Vector2b startingVel = Vector2b(xVel, yVel);
	Vector2b endingVel = Vector2b(endingXVel, endingYVel);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_maxTTL - _minTTL)) + _minTTL;
	ParticleObj::ParticleObjSettings settings;

	settings._position = _position + _offset;
	settings._size = Vector2b(_particleSize, _particleSize);
	settings._startingVelocity = startingVel;
	settings._endingVelocity = endingVel;
	settings._timeToLive = timeToLive;
	settings._shape = nullptr;
	settings._texture = _texture;


	if (_shapeType != Shape::NULL_SHAPE)
	{
		std::vector<Vector2b> vertPositions;
		Shape * shape = nullptr;

		float rotSpeed = ((rand() % 1000 / 1000.0f) * _rotationSpeed * 2) - _rotationSpeed;
		float sizeModifier;
		int randomScale;
		switch (_shapeType)
		{

		case Shape::Triangle:

			sizeModifier = 2.5f;
			vertPositions.push_back(Vector2b(0 * _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 2 * _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2b(0 * _particleSize, sizeModifier * _particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Triangle, _renderer, rotSpeed);

			break;

		case Shape::Square:

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2b(-sizeModifier       * _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2b(sizeModifier        * _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2b(sizeModifier        * _particleSize, sizeModifier * _particleSize));
			vertPositions.push_back(Vector2b(-sizeModifier       * _particleSize, sizeModifier * _particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Square, _renderer, rotSpeed);
			break;

		case Shape::Pentagon:

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2b(sizeModifier * (0 - 1.6f) * _particleSize, sizeModifier * (0 - 1.6f) * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * (2 - 1.6f) * _particleSize, sizeModifier * (0 - 1.6f) * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * (2.6f - 1.6f) * _particleSize, sizeModifier * (2 - 1.6f) * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * (1 - 1.6f) * _particleSize, sizeModifier * (3.1f - 1.6f) * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * (-0.64f - 1.6f) * _particleSize, sizeModifier * (2 - 1.6f) * -_particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Pentagon, _renderer, rotSpeed);
			break;

		case Shape::Star:

			sizeModifier = 0.4f;
			vertPositions.push_back(Vector2b(sizeModifier * 6.5      *       _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 5 * _particleSize, sizeModifier * 5 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 0 * _particleSize, sizeModifier * 5.5f   *       -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 3.5f     *       _particleSize, sizeModifier * 9 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 2 * _particleSize, sizeModifier * 14 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 6.5f     *       _particleSize, sizeModifier * 11.5f *       -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 12 * _particleSize, sizeModifier * 14 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 10.5f    *       _particleSize, sizeModifier * 9 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 14 * _particleSize, sizeModifier * 5.5f  *       -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 9 * _particleSize, sizeModifier * 5 * -_particleSize));


			shape = new Shape(_position, vertPositions, Shape::ShapeType::Star, _renderer, rotSpeed);
			break;

		case Shape::NULL_SHAPE:
			delete shape;
			break;
		case Shape::Footsteps:
			delete shape;
			settings._size = Vector2b(_FOOTSTEPS_PRESET._particleSize, _FOOTSTEPS_PRESET._particleSize);
			break;

		case Shape::RocketThruster:

			settings._size = Vector2b(_ROCKET_THRUSTER_PRESET._particleSize, _ROCKET_THRUSTER_PRESET._particleSize);

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2b(sizeModifier * 0 * _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 2 * _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 2.6f     * _particleSize, sizeModifier * 2 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 1 * _particleSize, sizeModifier * 3.1f *      -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * -0.64f   * _particleSize, sizeModifier * 2 * -_particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Pentagon, _renderer, rotSpeed);


			break;

		case Shape::StarPreset:

			settings._size = Vector2b(_STAR_PRESET._particleSize, _STAR_PRESET._particleSize);
			randomScale = rand() % 10;

			sizeModifier = 0.2f + (0.05f * randomScale);
			vertPositions.push_back(Vector2b(sizeModifier * 6.5      *       _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 5 * _particleSize, sizeModifier * 5 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 0 * _particleSize, sizeModifier * 5.5f   *       -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 3.5f     *       _particleSize, sizeModifier * 9 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 2 * _particleSize, sizeModifier * 14 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 6.5f     *       _particleSize, sizeModifier * 11.5f *       -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 12 * _particleSize, sizeModifier * 14 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 10.5f    *       _particleSize, sizeModifier * 9 * -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 14 * _particleSize, sizeModifier * 5.5f  *       -_particleSize));
			vertPositions.push_back(Vector2b(sizeModifier * 9 * _particleSize, sizeModifier * 5 * -_particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Star, _renderer, rotSpeed);


			break;
		case Shape::Tron:
			delete shape;
			settings._size = Vector2b(_TRON_PRESET._particleSize, _TRON_PRESET._particleSize);
			break;

		default:

			break;
		}


		settings._shape = shape;
	}


	_particleList.push_back(new ParticleObj(settings, *this));
}