#pragma once

#include "pch.h"
#include "FuzzyRegulator.h"

class PhysicalModel
{
public:
	PhysicalModel();
	~PhysicalModel();

	bool Init(int width, int height, float servoTimeDelay);
	void Update(float dt);
	void SetServoTimeDelay(float servoTimeDelay);
	void SetGravity(float gravityAcceleration);

	std::map<std::string, b2Body*>* GetBodies() {
		return &bAndBBodies;
	}

	b2World* GetWorld() {
		return world;
	}

private:
	bool ReInit();

	FuzzyRegulator regulator;
	float regulatorOutput;
	float servoTimeDelay;
	b2World* world;
	b2Vec2 gravity;
	bool initialized;
	std::vector<b2Body*> walls;
	std::map<std::string, b2Body*> bAndBBodies;

	// Joint pointers for cleanup...
	b2RevoluteJoint* joint1;
	b2RevoluteJoint* joint2; 
	b2RevoluteJoint* joint31;
	b2PrismaticJoint* joint32;
	b2GearJoint* joint3;
};

