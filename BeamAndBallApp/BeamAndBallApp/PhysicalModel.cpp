#include "pch.h"
#include "PhysicalModel.h"

#define DEGTORAD (b2_pi/180.0f)

PhysicalModel::PhysicalModel()
{
	beamLength = 5.0f;
	beamFromLeftSide = 2.5f;
}


PhysicalModel::~PhysicalModel()
{
	if (joint1)
	{
		// Delete Joints
		world->DestroyJoint(joint1);
		world->DestroyJoint(joint2);
		world->DestroyJoint(joint31);
		world->DestroyJoint(joint32);

		// Delete World
		delete world;
	}
}

void BeginContact(b2Contact* contact) 
{ 
	b2Body* bodyA = contact->GetFixtureA()->GetBody(); 
	b2Body* bodyB = contact->GetFixtureB()->GetBody(); 
	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody) 
	{ 
		bodyA->SetAwake(false); bodyB->SetAwake(false); 
	} 
}

bool PhysicalModel::Init(int width, int height, float servoTimeDelay)
{
	regulator.Init();

	gravity.Set(0.0f, -9.81f); 
	world = new b2World(gravity);

	// Walls around the world
		// Ground
		b2BodyDef bDef;
		bDef.position.Set(0.0f, 0.0f);
		bDef.allowSleep = false;
		b2Body* wallBody = world->CreateBody(&bDef);
		b2EdgeShape* wG = new b2EdgeShape();
		b2Vec2 v1(0.0f, 0.0f);
		b2Vec2 v2(10.0f, 0.0f);
		wG->Set(v1, v2);
		wallBody->CreateFixture(wG, 1.0f);
		walls.push_back(wallBody);

		// Left
		b2BodyDef bDef2;
		b2EdgeShape* wL = new b2EdgeShape();
		bDef2.position.Set(0.0f, 0.0f);
		bDef2.allowSleep = false;
		wallBody = world->CreateBody(&bDef2);
		v1.Set(0.0f, 0.0f);
		v2.Set(0.0f, 10.0f);
		wL->Set(v1, v2);
		wallBody->CreateFixture(wL, 1.0f);
		walls.push_back(wallBody);

		// Right
		b2BodyDef bDef3;
		b2EdgeShape* wR = new b2EdgeShape();
		bDef3.position.Set(0.0f, 0.0f);
		bDef3.allowSleep = false;
		wallBody = world->CreateBody(&bDef3);
		v1.Set(10.0f, 0.0f);
		v2.Set(10.0f, 10.0f);
		wR->Set(v1, v2);
		wallBody->CreateFixture(wR, 1.0f);
		walls.push_back(wallBody);

		// Up
		b2BodyDef bDef4;
		b2EdgeShape* wU = new b2EdgeShape();
		bDef4.position.Set(0.0f, 0.0f);
		bDef4.allowSleep = false;
		wallBody = world->CreateBody(&bDef4);
		v1.Set(0.0f, 10.0f);
		v2.Set(10.0f, 10.0f);
		wU->Set(v1, v2);
		wallBody->CreateFixture(wU, 0.0f);
		walls.push_back(wallBody);

	// Servo, Beam, Ball, Connection Beam - Default Values
		// Beam
		b2BodyDef bDef5;
		bDef5.position.Set(5.0f, 6.45f);
		bDef5.type = b2_dynamicBody;
		bDef5.allowSleep = false;
		bDef5.gravityScale = 0.0f;
		b2Body* body = world->CreateBody(&bDef5);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("beam", body));
		b2PolygonShape* beam = new b2PolygonShape();
		beam->SetAsBox(2.5f, 0.05f);
		b2FixtureDef fixDef;
		fixDef.shape = beam;
		fixDef.density = 4.0f;
		fixDef.restitution = 0.0f;
		body->CreateFixture(&fixDef);

		// Connector
		b2BodyDef bDef6;
		bDef6.position.Set(7.55f, 5.4f);
		bDef6.type = b2_dynamicBody;
		bDef6.allowSleep = false;
		bDef6.gravityScale = 0.0f;
		body = world->CreateBody(&bDef6);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("conn", body));
		b2PolygonShape* conn = new b2PolygonShape();
		conn->SetAsBox(0.05f, 1.0f);
		fixDef.shape = conn;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

		// Ball
		b2BodyDef bDef7;
		bDef7.position.Set(5.166f, 7.0f);
		bDef7.type = b2_dynamicBody;
		bDef7.allowSleep = false;
		bDef7.bullet = true;
		body = world->CreateBody(&bDef7);
		body->SetLinearDamping(2.0f);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("ball", body));
		b2CircleShape* ball = new b2CircleShape();
		ball->m_radius = 0.5f;
		fixDef.shape = ball;
		fixDef.density = 15.5f;
		fixDef.restitution = 0.0f;
		//fixDef.isSensor = true;
		body->CreateFixture(&fixDef);

		// Servo
		b2BodyDef bDef8;
		bDef8.position.Set(6.55f, 4.4f);
		bDef8.allowSleep = false;
		bDef8.type = b2_kinematicBody;
		bDef8.bullet = false;
		bDef8.angularVelocity = -1.0f;
		bDef8.gravityScale = 0.0f;
		body = world->CreateBody(&bDef8);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("servo", body));
		b2CircleShape servo;
		servo.m_radius = 1.0f;
		fixDef.shape = &servo;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

		// ServoPin
		b2BodyDef bDef10;
		bDef10.position.Set(6.7f, 4.4f);
		bDef10.type = b2_staticBody;
		body = world->CreateBody(&bDef10);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("servoPin", body));
		b2CircleShape servoPin;
		servo.m_radius = 0.0f;
		fixDef.shape = &servoPin;
		fixDef.density = 4.0f;
		body->CreateFixture(&fixDef);

		// triangle
		b2BodyDef bDef9;
		bDef9.allowSleep = false;
		bDef9.position.Set(2.5f, 6.5f);
		bDef9.type = b2_staticBody;
		bDef9.gravityScale = 0.0f;
		body = world->CreateBody(&bDef9);
		bAndBBodies.insert(std::pair<std::string, b2Body*>("triangle", body));
		b2PolygonShape triangle;
		b2Vec2 points[3];
		points[0].Set(2.5f, 6.5f);
		points[1].Set(1.0f, 3.9f);
		points[2].Set(4.0f, 3.9f);
		triangle.Set(points, 3);
		fixDef.shape = &triangle;
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
		joint2Def.Initialize(bAndBBodies.find("beam")->second, bAndBBodies.find("conn")->second, b2Vec2(7.5f, 6.4f));
		joint2Def.collideConnected = false;
		joint2 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&joint2Def));

		// Joint #3A - Connector <-> Servo
		b2RevoluteJointDef jointADef;
		jointADef.Initialize(bAndBBodies.find("servo")->second, bAndBBodies.find("conn")->second, b2Vec2(7.55f, 4.4f));
		jointADef.collideConnected = false;
		joint31 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&jointADef));
		
		// Joint #3B - Pin <-> Servo
		b2RevoluteJointDef jointBDef;
		jointBDef.Initialize(bAndBBodies.find("servoPin")->second, bAndBBodies.find("servo")->second, b2Vec2(6.55f, 4.4f));
		jointBDef.collideConnected = false;
		jointBDef.enableLimit = true;
		jointBDef.lowerAngle = -b2_pi/2;
		jointBDef.upperAngle = b2_pi / 2;
		joint32 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&jointBDef));

	initialized = true;

	this->servoTimeDelay = servoTimeDelay;
	oldDiff = 0.0f;

	return true;
}

void PhysicalModel::Update(float dt, float desiredPos)
{
	int32 velocityIterations = 10;
	int32 positionIterations = 8;

	float odchylka = ((bAndBBodies.find("ball")->second->GetPosition().x - beamFromLeftSide) - desiredPos) / beamLength;
	float desiredAngle = regulator.getAngle(odchylka, oldDiff-odchylka) * (b2_pi/2);
	oldDiff = odchylka;

	float actualAngle = bAndBBodies.find("servo")->second->GetAngle();

	if (abs(actualAngle - desiredAngle) <= 1e-1)
	{
		bAndBBodies.find("servo")->second->SetAngularVelocity(0);
	}
	else {

		if (actualAngle > desiredAngle)
		{
			bAndBBodies.find("servo")->second->SetAngularVelocity(-servoTimeDelay);
		}

		if (actualAngle < desiredAngle)
		{
			bAndBBodies.find("servo")->second->SetAngularVelocity(servoTimeDelay);
		}
	}

	
	
	world->Step(1.0f/60.0f, velocityIterations, positionIterations);
}

bool PhysicalModel::ReInit(BandB_Data &dat)
{
	// Delete Joints
	world->DestroyJoint(joint1);
	world->DestroyJoint(joint2);
	world->DestroyJoint(joint31);
	world->DestroyJoint(joint32);

	// Delete World
	delete world;

	bAndBBodies.clear();

	gravity.Set(0.0f, -dat.gravitationalAcceleration);
	world = new b2World(gravity);

	// Walls around the world
	// Ground
	b2BodyDef bDef;
	bDef.position.Set(0.0f, 0.0f);
	bDef.allowSleep = false;
	b2Body* wallBody = world->CreateBody(&bDef);
	b2EdgeShape* wG = new b2EdgeShape();
	b2Vec2 v1(0.0f, 0.0f);
	b2Vec2 v2(10.0f, 0.0f);
	wG->Set(v1, v2);
	wallBody->CreateFixture(wG, 1.0f);
	walls.push_back(wallBody);

	// Left
	b2BodyDef bDef2;
	b2EdgeShape* wL = new b2EdgeShape();
	bDef2.position.Set(0.0f, 0.0f);
	bDef2.allowSleep = false;
	wallBody = world->CreateBody(&bDef2);
	v1.Set(0.0f, 0.0f);
	v2.Set(0.0f, 10.0f);
	wL->Set(v1, v2);
	wallBody->CreateFixture(wL, 1.0f);
	walls.push_back(wallBody);

	// Right
	b2BodyDef bDef3;
	b2EdgeShape* wR = new b2EdgeShape();
	bDef3.position.Set(0.0f, 0.0f);
	bDef3.allowSleep = false;
	wallBody = world->CreateBody(&bDef3);
	v1.Set(10.0f, 0.0f);
	v2.Set(10.0f, 10.0f);
	wR->Set(v1, v2);
	wallBody->CreateFixture(wR, 1.0f);
	walls.push_back(wallBody);

	// Up
	b2BodyDef bDef4;
	b2EdgeShape* wU = new b2EdgeShape();
	bDef4.position.Set(0.0f, 0.0f);
	bDef4.allowSleep = false;
	wallBody = world->CreateBody(&bDef4);
	v1.Set(0.0f, 10.0f);
	v2.Set(10.0f, 10.0f);
	wU->Set(v1, v2);
	wallBody->CreateFixture(wU, 0.0f);
	walls.push_back(wallBody);

	// Servo, Beam, Ball, Connection Beam - Default Values
	b2Vec2 start = b2Vec2(6.55f, 4.4f);
	// Beam
	b2BodyDef bDef5;
	bDef5.position.Set(start.x + dat.servoRadius - (dat.beamLength / 2), 6.45f);
	bDef5.type = b2_dynamicBody;
	bDef5.allowSleep = false;
	bDef5.gravityScale = 0.0f;
	b2Body* body = world->CreateBody(&bDef5);
	bAndBBodies.insert(std::pair<std::string, b2Body*>("beam", body));
	b2PolygonShape* beam = new b2PolygonShape();
	beam->SetAsBox(dat.beamLength / 2, 0.05f);
	b2FixtureDef fixDef;
	fixDef.shape = beam;
	fixDef.density = 4.0f;
	fixDef.restitution = 0.0f;
	body->CreateFixture(&fixDef);

	beamLength = dat.beamLength;
	beamFromLeftSide = start.x + dat.servoRadius - dat.beamLength;

	// Connector
	b2BodyDef bDef6;
	bDef6.position.Set(start.x + 0.05f + dat.servoRadius, 5.4f);
	bDef6.type = b2_dynamicBody;
	bDef6.allowSleep = false;
	bDef6.gravityScale = 0.0f;
	body = world->CreateBody(&bDef6);
	bAndBBodies.insert(std::pair<std::string, b2Body*>("conn", body));
	b2PolygonShape* conn = new b2PolygonShape();
	conn->SetAsBox(0.05f, 1.0f);
	fixDef.shape = conn;
	fixDef.density = 4.0f;
	body->CreateFixture(&fixDef);

	// Ball
	b2BodyDef bDef7;
	bDef7.position.Set(beamFromLeftSide + 0.5f, 6.5f + dat.ballRadius);
	bDef7.type = b2_dynamicBody;
	bDef7.allowSleep = false;
	bDef7.bullet = true;
	body = world->CreateBody(&bDef7);
	body->SetLinearDamping(2.0f);
	bAndBBodies.insert(std::pair<std::string, b2Body*>("ball", body));
	b2CircleShape* ball = new b2CircleShape();
	ball->m_radius = dat.ballRadius;
	fixDef.shape = ball;
	fixDef.density = dat.ballWeight / (b2_pi * dat.ballRadius * dat.ballRadius);
	fixDef.restitution = 0.0f;
	//fixDef.isSensor = true;
	body->CreateFixture(&fixDef);

	// Servo
	b2BodyDef bDef8;
	bDef8.position.Set(6.55f, 4.4f);
	bDef8.allowSleep = false;
	bDef8.type = b2_kinematicBody;
	bDef8.bullet = false;
	bDef8.angularVelocity = -1.0f;
	bDef8.gravityScale = 0.0f;
	body = world->CreateBody(&bDef8);
	bAndBBodies.insert(std::pair<std::string, b2Body*>("servo", body));
	b2CircleShape servo;
	servo.m_radius = dat.servoRadius;
	fixDef.shape = &servo;
	fixDef.density = 4.0f;
	body->CreateFixture(&fixDef);

	// ServoPin
	b2BodyDef bDef10;
	bDef10.position.Set(6.7f, 4.4f);
	bDef10.type = b2_staticBody;
	body = world->CreateBody(&bDef10);
	bAndBBodies.insert(std::pair<std::string, b2Body*>("servoPin", body));
	b2CircleShape servoPin;
	servo.m_radius = 0.0f;
	fixDef.shape = &servoPin;
	fixDef.density = 4.0f;
	body->CreateFixture(&fixDef);

	// triangle
	b2BodyDef bDef9;
	bDef9.allowSleep = false;
	bDef9.position.Set(beamFromLeftSide, 6.5f);
	bDef9.type = b2_staticBody;
	bDef9.gravityScale = 0.0f;
	body = world->CreateBody(&bDef9);
	bAndBBodies.insert(std::pair<std::string, b2Body*>("triangle", body));
	b2PolygonShape triangle;
	b2Vec2 points[3];
	points[0].Set(beamFromLeftSide, 6.5f);
	points[1].Set(beamFromLeftSide - 1.5f, 3.9f);
	points[2].Set(beamFromLeftSide + 1.5f, 3.9f);
	triangle.Set(points, 3);
	fixDef.shape = &triangle;
	fixDef.density = 4.0f;
	body->CreateFixture(&fixDef);

	// Joints ...
	// Joint #1 - Triangle <-> Beam
	b2RevoluteJointDef joint1Def;
	joint1Def.Initialize(bAndBBodies.find("triangle")->second, bAndBBodies.find("beam")->second, b2Vec2(beamFromLeftSide, 6.5f));
	joint1Def.collideConnected = false;
	joint1 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&joint1Def));

	// Joint #2 - Beam <-> Connector
	b2RevoluteJointDef joint2Def;
	joint2Def.Initialize(bAndBBodies.find("beam")->second, bAndBBodies.find("conn")->second, b2Vec2(beamFromLeftSide + dat.beamLength, 6.4f));
	joint2Def.collideConnected = false;
	joint2 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&joint2Def));

	// Joint #3A - Connector <-> Servo
	b2RevoluteJointDef jointADef;
	jointADef.Initialize(bAndBBodies.find("servo")->second, bAndBBodies.find("conn")->second, b2Vec2(6.55f + dat.servoRadius, 4.4f));
	jointADef.collideConnected = false;
	joint31 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&jointADef));

	// Joint #3B - Pin <-> Servo
	b2RevoluteJointDef jointBDef;
	jointBDef.Initialize(bAndBBodies.find("servoPin")->second, bAndBBodies.find("servo")->second, b2Vec2(6.55f, 4.4f));
	jointBDef.collideConnected = false;
	jointBDef.enableLimit = true;
	jointBDef.lowerAngle = -b2_pi / 2;
	jointBDef.upperAngle = b2_pi / 2;
	joint32 = reinterpret_cast<b2RevoluteJoint*>(world->CreateJoint(&jointBDef));

	initialized = true;

	this->servoTimeDelay = dat.servoMaxSpeed;
	oldDiff = 0.0f;

	return true;
}
