#pragma once
#include "Vector2b.h"
#include <vector>
#include <SDL.h>

class ParticleManager;

/// <summary>
/// This is used by the ParticleObj. It holds the type and vertices list of the shape and manages all the rotation and drawing of the shape.
/// </summary>
class Shape
{
public:

	/// <summary>
	/// The different types of shapes we will use.
	/// </summary>
	enum ShapeType
	{
		Triangle,
		Square,
		Pentagon,
		Star,
		Footsteps,
		RocketThruster,
		Tron,
		StarPreset,
		NULL_SHAPE
	};


	/// <summary>
	/// This will create a shape
	/// </summary>
	/// <param name="pPos">The starting position of the Shape</param>
	/// <param name="pVerts">The vector of vertices for this shape</param>
	/// <param name="pType">The type of shape we're creating</param>
	/// <param name="pRenderer">The renderer we're drawing to</param>
	/// <param name="pRotSpeed">The rotation speed of the shape</param>
	Shape(Vector2b pPos, std::vector<Vector2b> pVerts, ShapeType pType, SDL_Renderer* pRenderer, float pRotSpeed);
	~Shape();


	/// <summary>
	/// Draw the shape to the screen it was passed in by the constructor
	/// </summary>
	void Draw();

	/// <summary>
	/// Update the shape. This will rotate and move it.
	/// </summary>
	/// <param name="pVelocity">The velocity the shape will move in</param>
	/// <param name="pDT">The delta time</param>
	void Update(Vector2b pVelocity, float pDT);

	/// <summary>
	/// Rotate the shape
	/// </summary>
	/// <param name="pDT">Delta time</param>
	void Rotate(float pDT);

	/// <summary>
	/// Set the shape's colour
	/// </summary>
	/// <param name="pColour">The new colour</param>
	void SetColour(SDL_Color pColour);
	
	/// <summary>
	/// The shape's position
	/// </summary>
	Vector2b _position;

	/// <summary>
	/// the vector of vertex positions. These coordinates are in local form, so 0,0 is the object's centre.
	/// </summary>
	std::vector<Vector2b> _vertices;

	/// <summary>
	/// the angle of the shape in euler
	/// </summary>
	float _angle;

	/// <summary>
	/// the type of shape this is representing
	/// </summary>
	ShapeType _type;

	/// <summary>
	/// the address of the renderer we will draw to
	/// </summary>
	SDL_Renderer * _renderer;

	/// <summary>
	/// The current colour of the shape
	/// </summary>
	SDL_Color _colour;

	/// <summary>
	/// how fast this shape will rotate
	/// </summary>
	float _rotSpeed;
};

