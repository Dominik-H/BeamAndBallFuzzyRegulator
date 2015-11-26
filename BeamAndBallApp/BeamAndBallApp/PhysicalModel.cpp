#include "pch.h"
#include "PhysicalModel.h"


PhysicalModel::PhysicalModel()
{
}


PhysicalModel::~PhysicalModel()
{
	// Delete Joints
	world->DestroyJoint(joint1);
	world->DestroyJoint(joint2);
	world->DestroyJoint(joint3);
	world->DestroyJoint(joint31);
	world->DestroyJoint(joint32);

	// Delete World
	delete world;
}

bool PhysicalModel::Init(int width, int height, float servoTimeDelay)
{
	gravity.Set(0.0f, -10.0f); 
	world = new b2World(gravity);

	b2BodyDef bDef;

	// Walls around the world
		// Ground
		bDef.position.Set(5.0f, 0.0f); 
		b2Body* wallBody = world->CreateBody(&bDef);
		b2EdgeShape wG;
		b2Vec2 v1(0.0f, 0.0f);
		b2Vec2 v2(10.0f, 0.0f);
		wG.Set(v1, v2);
		wallBody->CreateFixture(&wG, 0.0f);
		walls.push_back(wallBody);

		// Left
		b2EdgeShape wL;
		bDef.position.Set(0.0f, 5.0f);
		wallBody = world->CreateBody(&bDef);
		v1.Set(0.0f, 0.0f);
		v2.Set(0.0f, 10.0f);
		wL.Set(v1, v2);
		wallBody->CreateFixture(&wL, 0.0f);
		walls.push_back(wallBody);

		// Right
		b2EdgeShape wR;
		bDef.position.Set(10.0f, 5.0f);
		wallBody = world->CreateBody(&bDef);
		v1.Set(10.0f, 0.0f);
		v2.Set(10.0f, 10.0f);
		wR.Set(v1, v2);
		wallBody->CreateFixture(&wR, 0.0f);
		walls.push_back(wallBody);

		// Up
		b2EdgeShape wU;
		bDef.position.Set(5.0f, 10.0f);
		wallBody = world->CreateBody(&bDef);
		v1.Set(0.0f, 10.0f);
		v2.Set(10.0f, 10.0f);
		wU.Set(v1, v2);
		wallBody->CreateFixture(&wU, 0.0f);
		walls.push_back(wallBody);

	// Servo, Beam, Ball, Connection Beam - Default Values
		// Beam
		bDef.position.Set(5.0f, 6.55f);
		bDef.type = b2_dynamicBody;
		b2Body* body = world->CreateBody(&bDef);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("beam", body));
		b2PolygonShape beam;
		beam.SetAsBox(2.5f, 0.05f);
		b2FixtureDef fixDef;
		fixDef.shape = &beam;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

		// Connector
		bDef.position.Set(7.55f, 4.5f);
		bDef.type = b2_dynamicBody;
		body = world->CreateBody(&bDef);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("conn", body));
		b2PolygonShape conn;
		conn.SetAsBox(0.05f, 2.0f);
		fixDef.shape = &conn;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

		// Ball
		bDef.position.Set(5.0f, 7.6f);
		bDef.type = b2_dynamicBody;
		bDef.bullet = true;
		body = world->CreateBody(&bDef);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("ball", body));
		b2CircleShape ball;
		ball.m_p.Set(5.0f, 7.6f);
		ball.m_radius = 1.0f;
		fixDef.shape = &ball;
		fixDef.density = 0.5f;
		body->CreateFixture(&fixDef);

		// Servo
		bDef.position.Set(6.5f, 4.5f);
		bDef.type = b2_dynamicBody;
		bDef.bullet = false;
		bDef.gravityScale = 0.0f;
		body = world->CreateBody(&bDef);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("servo", body));
		b2CircleShape servo;
		servo.m_p.Set(6.5f, 4.5f);
		servo.m_radius = 1.0f;
		fixDef.shape = &servo;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

		// triangle
		bDef.position.Set(2.5f, 6.5f);
		bDef.type = b2_staticBody;
		bDef.gravityScale = 0.0f;
		body = world->CreateBody(&bDef);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("triangle", body));
		b2PolygonShape triangle;
		b2Vec2 points[3];
		points[0].Set(2.5f, 6.5f);
		points[1].Set(1.0f, 3.9f);
		points[2].Set(4.0f, 3.9f);
		triangle.Set(points, 3);
		fixDef.shape = &servo;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

	// Joints ...
		// Joint #1 - Triangle <-> Beam
		b2RevoluteJointDef joint1Def;
		joint1Def.Initialize(bAndBBodies.find("triangle")->second, bAndBBodies.find("beam")->second, b2Vec2(2.5f, 6.5f));
		joint1Def.collideConnected = false;
		joint1 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&joint1Def));

		// Joint #2 - Beam <-> Connector
		b2RevoluteJointDef joint2Def;
		joint2Def.Initialize(bAndBBodies.find("beam")->second, bAndBBodies.find("conn")->second, b2Vec2(7.5f, 6.5f));
		joint2Def.collideConnected = false;
		joint2 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&joint2Def));

		// Joint #3 - Connector <-> Servo
		b2RevoluteJointDef jointADef;
		jointADef.Initialize(bAndBBodies.find("servo")->second, walls[0], b2Vec2(6.5f, 4.5f));
		jointADef.collideConnected = false;
		joint31 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&jointADef));

		b2PrismaticJointDef jointBDef;
		jointBDef.Initialize(bAndBBodies.find("conn")->second, walls[0], b2Vec2(7.55f, 4.5f), b2Vec2(0.0f, 1.0f));
		jointBDef.collideConnected = false;
		joint32 = reinterpret_cast<b2PrismaticJoint*>(world->CreateJoint(&jointBDef));

		b2GearJointDef joint3Def;
		joint3Def.bodyA = bAndBBodies.find("conn")->second;
		joint3Def.bodyB = bAndBBodies.find("servo")->second;
		joint3Def.joint1 = joint31;
		joint3Def.joint2 = joint32;
		joint3Def.ratio = 2.0f * b2_pi / 1.05f;
		joint3 = reinterpret_cast<b2GearJoint*>(world->CreateJoint(&joint3Def));

	initialized = true;

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