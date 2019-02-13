#ifndef BODYFACTORY_H
#define BODYFACTORY_H
#include "Box2D\Box2D.h"

class BodyFactory
{
public:
					BodyFactory();
					~BodyFactory();

	void			Initialize(b2World* b2world);
	b2Body*			CreateCircleBody(b2BodyType type, b2Vec2 pos, b2Vec2 size, uint16 categoryBit, uint16 maskBits, bool isSensor);
	b2Body*			CreateBoxBody(b2BodyType type, b2Vec2 pos, b2Vec2 size,  uint16 categoryBit, uint16 maskBits, bool isSensor);
	b2Body*			CreateBoxBodyWithSensor(b2BodyType type, b2Vec2 pos, b2Vec2 size, uint16 categoryBit, uint16 maskBits, uint16 maskBits2);
	b2Body*			CreatePolyBody(b2BodyType type, b2Vec2 pos, b2Vec2* vertices, int count, uint16 categoryBit, uint16 maskBits, bool isSensor);

private:
	b2World*		_b2world;
};
#endif