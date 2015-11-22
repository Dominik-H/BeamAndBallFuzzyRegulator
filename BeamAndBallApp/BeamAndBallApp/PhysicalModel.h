#pragma once

#include "pch.h"
#include "FuzzyRegulator.h"

class PhysicalModel
{
public:
	PhysicalModel();
	~PhysicalModel();

	bool Init(int width, int height, float servoTimeDelay);
	void Update(int dt);
	void SetServoTimeDelay(float servoTimeDelay);
	void SetGravity(float gravityAcceleration);

private:
	bool ReInit();

	FuzzyRegulator regulator;
	float regulatorOutput;
	float servoTimeDelay;
	b2World world;
	b2Vec2 gravity;
	bool initialized;
	std::vector<b2Body*> walls;
	std::vector<b2PolygonShape> wallShapes;
	std::map<std::string, b2Body*> bAndBBodies;
	b2PolygonShape beam;
	b2PolygonShape connector;
	b2CircleShape ball;
	b2CircleShape servo;
};

