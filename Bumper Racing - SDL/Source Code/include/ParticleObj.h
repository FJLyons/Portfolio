#pragma once
#include <SDL.h>
#include "Vector2b.h"
#include "Camera2D.h"

class Shape;
class ParticleManager;

/// <summary>
/// The ParticleComponent Object. This represents each individual particle that the ParticleManager creates. It holds either a texture or a shape.
/// </summary>
class ParticleObj
{
public:

	/// <summary>
	/// The settings of this ParticleObj that will be passed into the constructor.
	/// </summary>
	struct ParticleObjSettings
	{
		/// <summary>
		/// The starting position of the particle.
		/// </summary>
		Vector2b _position;

		/// <summary>
		/// The size of the particle. Only used for creating the textures rect. Can be not initialised if we're using a shape.
		/// </summary>
		Vector2b _size;

		/// <summary>
		/// The starting velocity of the particle.
		/// </summary>
		Vector2b _startingVelocity;

		/// <summary>
		/// The ending velocity of the particle. This lerped to from the starting velocity
		/// </summary>
		Vector2b _endingVelocity;

		/// <summary>
		/// The time in ms that this particle will live for
		/// </summary>
		float _timeToLive;

		/// <summary>
		/// The texture of the particle, can be null if we're using a shape.
		/// </summary>
		SDL_Texture* _texture;

		/// <summary>
		/// The shape of the particle, can be null if we're using a texture.
		/// </summary>
		Shape* _shape;

		/// <summary>
		/// The direction of the particle, used to orientate the particle if it's velocity is 0
		/// </summary>
		Vector2b _direction;
	};


	/// <summary> Required Constructor </summary>
	/// <param name="pSettings"> This is the settings of the ParticleObject. (it's position, size, shape or texture etc.) </param>
	/// <param name="pParticleSys"> This is a reference of the parent ParticleSystem. This is required to get the colour it should be lerping to. </param>
	ParticleObj(ParticleObjSettings pSettings, ParticleManager& pParticleSys);

	/// clears the Shape pointer.
	~ParticleObj();

	/// <summary>
	/// The update function will move the particle, check it's time to live and find the particle's colour if it's a shape.
	/// </summary>
	/// <param name="pDT">The delta time. Required to be passed in as ms.</param>
	void update(float pDT);

	/// <summary>
	/// If the particle should be respawned.
	/// </summary>
	/// <returns>If the particle's time alive is greater than it's time to live</returns>
	bool readyToRespawn() const;
	
	/// <summary>
	/// This will render the ParticleObj to the screen
	/// </summary>
	/// <param name="pRenderer">The renderer to draw to</param>
	void render(SDL_Renderer* pRenderer, Camera2D::Camera* camera);

private:

	/// <summary>
	/// The address of the parent particle System.
	/// </summary>
	ParticleManager& _parentSys;

	/// <summary>
	/// The position of this ParticleObj
	/// </summary>
	Vector2b _position;

	/// <summary>
	/// If the particle object is showing a texture, this will be the rectangle on screen that it will be drawn on. 
	/// If this is using a shape, the rect can be NULL
	/// </summary>
	SDL_Rect _rect;

	/// <summary>
	/// The velocity that this particle will start moving at.
	/// </summary>
	Vector2b _startingVelocity;

	/// <summary>
	/// The current velocity this particle will be moving at. This is the lerped value, going from starting velocity to ending velocity.
	/// </summary>
	Vector2b _currentVelocity;

	/// <summary>
	/// The final velocity of the particle.
	/// </summary>
	Vector2b _endingVelocity;

	/// <summary>
	/// The total time this particle will be alive. Measured in ms.
	/// </summary>
	float _timeToLive;

	/// <summary>
	/// The time this particle has been alive. Measured in ms.
	/// </summary>
	float _timeAlive;

	/// <summary>
	/// The current angle that the particle is at. Only used if the particle is displaying a shape, if this is particle is a texture, this won't be used. Measured in Euler.
	/// </summary>
	float _angle;

	/// <summary>
	/// The texture of the particle. Will be nullptr if we're using a shape.
	/// </summary>
	SDL_Texture* _texture;

	/// <summary>
	/// The shape of the particle. Will be nullptr if the system is using textures.
	/// </summary>
	Shape* _shape;
};
