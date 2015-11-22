#include "pch.h"
#include "PhysicalModel.h"


PhysicalModel::PhysicalModel()
	:gravity(0.0f, -10.0f), world(gravity)
{
}


PhysicalModel::~PhysicalModel()
{
}

bool PhysicalModel::Init(int width, int height, float servoTimeDelay)
{
	return true;
}

void PhysicalModel::Update(int dt)
{

}

void PhysicalModel::SetServoTimeDelay(float servoTimeDelay)
{

}

void PhysicalModel::SetGravity(float gravityAcceleration)
{

}

bool PhysicalModel::ReInit()
{
	return true;
}
