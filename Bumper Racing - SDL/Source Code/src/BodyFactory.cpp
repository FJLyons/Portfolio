#include "BodyFactory.h"
#include "Helpers.h"


BodyFactory::BodyFactory()
{

}

BodyFactory::~BodyFactory()
{

}


void BodyFactory::Initialize(b2World* b2world)
{
	_b2world = b2world;
}

b2Body* BodyFactory::CreateBoxBody(b2BodyType type, b2Vec2 pos, b2Vec2 size, uint16 categoryBit, uint16 maskBits, bool isSensor)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.x = pixelsToMeters(pos.x);
	bodyDef.position.y = pixelsToMeters(pos.y);
	bodyDef.angle = 0;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(pixelsToMeters(size.x), pixelsToMeters(size.y));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.isSensor = isSensor;
	fixtureDef.filter.categoryBits = categoryBit;
	fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* BodyFactory::CreateBoxBodyWithSensor(b2BodyType type, b2Vec2 pos, b2Vec2 size, uint16 categoryBit, uint16 maskBits, uint16 maskBits2)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.x = pixelsToMeters(pos.x);
	bodyDef.position.y = pixelsToMeters(pos.y);
	bodyDef.angle = 0;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(pixelsToMeters(size.x), pixelsToMeters(size.y));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = categoryBit;
	fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	b2FixtureDef fixtureDefSensor;
	fixtureDefSensor.shape = &boxShape;
	fixtureDefSensor.density = 1;
	fixtureDefSensor.isSensor = false;
	fixtureDefSensor.filter.categoryBits = categoryBit;
	fixtureDefSensor.filter.maskBits = maskBits2;

	body->CreateFixture(&fixtureDefSensor);

	return body;
}

b2Body* BodyFactory::CreatePolyBody(b2BodyType type, b2Vec2 pos, b2Vec2* vertices, int count, uint16 categoryBit, uint16 maskBits, bool isSensor)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.x = pixelsToMeters(pos.x);
	bodyDef.position.y = pixelsToMeters(pos.y);
	b2Body* body = _b2world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.Set(vertices, count);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.isSensor = isSensor;
	fixtureDef.filter.categoryBits = categoryBit;
	fixtureDef.filter.maskBits = maskBits;

	body->CreateFixture(&fixtureDef);

	return body;
}