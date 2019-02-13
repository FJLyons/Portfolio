#include "ParticleObj.h"
#include "Shape.h"
#include "ParticleManager.h"



ParticleObj::ParticleObj(ParticleObjSettings pSettings, ParticleManager & pParticleSys)
	:
	_parentSys(pParticleSys),
	_position(pSettings._position),
	_rect(SDL_Rect()),
	_startingVelocity(pSettings._startingVelocity),
	_currentVelocity(pSettings._startingVelocity),
	_endingVelocity(pSettings._endingVelocity),
	_timeToLive(pSettings._timeToLive),
	_timeAlive(0),
	_texture(pSettings._texture),
	_shape(pSettings._shape)
{
	_rect.x = _position.x;
	_rect.y = _position.y;
	_rect.w = pSettings._size.x;
	_rect.h = pSettings._size.y;
	_angle = 0;
	if(_shape == nullptr)
	{
		_angle = (atan2(pSettings._direction.y, pSettings._direction.x) * 180 / 3.1418f) - 90;
	}
}

ParticleObj::~ParticleObj()
{
	delete _shape;
}

void ParticleObj::update(float pDT)
{
	_timeAlive += pDT;

	if(_timeAlive > _timeToLive)
		return;

	//lerp velocity
	_currentVelocity = (_endingVelocity - _startingVelocity) * (_timeAlive / _timeToLive) + _startingVelocity;

	_position.x += _currentVelocity.x * pDT;
	_position.y += _currentVelocity.y * pDT;

	if (_shape != nullptr)
	{
		SDL_Color col = _parentSys.GetColour(_timeAlive);

		if ((col.r == 0 && col.g == 0 && col.b == 0 && col.a == 0) == false)
			_shape->SetColour(col);

		_shape->Update(_currentVelocity * pDT, pDT);
	}
}

bool ParticleObj::readyToRespawn() const
{
	return (_timeAlive > _timeToLive);
}

void ParticleObj::render(SDL_Renderer * pRenderer, Camera2D::Camera* camera)
{
	if (_shape == nullptr)
	{
		_rect.x = _position.x;
		_rect.y = _position.y;
		SDL_SetTextureAlphaMod(_texture, 255 - ((_timeAlive / _timeToLive) * 255));
		SDL_RenderCopyEx(pRenderer, _texture, NULL, &camera->worldToScreen(_rect), _angle, NULL, SDL_FLIP_NONE);
	}
	else
	{
		_shape->Draw();
	}
}
