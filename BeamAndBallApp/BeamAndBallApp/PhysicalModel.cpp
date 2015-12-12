#include "pch.h"
#include "PhysicalModel.h"

#define DEGTORAD (b2_pi/180.0f)

PhysicalModel::PhysicalModel()
{
	beamLength = 5.0f;
	beamFromLeftSide = 2.5f;
	totalTime = 0.0f;
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
	graphTimes.push_back(0.0f);
	positionOverTime.push_back(0.0f);
	outputOverTime.push_back(0.0f);

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

	totalTime += dt;

	float odchylka = ((bAndBBodies.find("ball")->second->GetPosition().x - beamFromLeftSide) - desiredPos) / beamLength;
	float desiredAngle = regulator.getAngle(odchylka, oldDiff-odchylka) * (b2_pi/2);
	oldDiff = odchylka;

	graphTimes.push_back(totalTime);
	positionOverTime.push_back(bAndBBodies.find("ball")->second->GetPosition().x - beamFromLeftSide);
	outputOverTime.push_back(desiredAngle);

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
	totalTime = 0.0f;

	graphTimes.clear();
	positionOverTime.clear();
	outputOverTime.clear();

	graphTimes.push_back(0.0f);
	positionOverTime.push_back(0.0f);
	outputOverTime.push_back(0.0f);

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

void PhysicalModel::DrawGraphs(sf::RenderWindow* win)
{
	// Outlines
	sf::Vertex axisX1[] = {
		sf::Vertex(sf::Vector2f(50.0f, 200.0f), sf::Color::Red),
		sf::Vertex(sf::Vector2f(50.0f, 50.0f), sf::Color::Red)
	};

	sf::Vertex axisY1[] = {
		sf::Vertex(sf::Vector2f(50.0f, 200.0f), sf::Color::Red),
		sf::Vertex(sf::Vector2f(795.0f, 200.0f), sf::Color::Red)
	};

	sf::Vertex axisX2[] = {
		sf::Vertex(sf::Vector2f(50.0f, 450.0f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(50.0f, 300.0f), sf::Color::Blue)
	};

	sf::Vertex axisY2[] = {
		sf::Vertex(sf::Vector2f(50.0f, 450.0f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(795.0f, 450.0f), sf::Color::Blue)
	};

	sf::Vertex LAX1[] = {
		sf::Vertex(sf::Vector2f(50.0f, 50.0f), sf::Color::Red),
		sf::Vertex(sf::Vector2f(37.0f, 75.0f), sf::Color::Red)
	};

	sf::Vertex RAX1[] = {
		sf::Vertex(sf::Vector2f(50.0f, 50.0f), sf::Color::Red),
		sf::Vertex(sf::Vector2f(63.0f, 75.0f), sf::Color::Red)
	};

	sf::Vertex LAY1[] = {
		sf::Vertex(sf::Vector2f(795.0f, 200.0f), sf::Color::Red),
		sf::Vertex(sf::Vector2f(770.0f, 187.0f), sf::Color::Red)
	};

	sf::Vertex RAY1[] = {
		sf::Vertex(sf::Vector2f(795.0f, 200.0f), sf::Color::Red),
		sf::Vertex(sf::Vector2f(770.0f, 213.0f), sf::Color::Red)
	};

	sf::Vertex LAX2[] = {
		sf::Vertex(sf::Vector2f(50.0f, 300.0f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(37.0f, 325.0f), sf::Color::Blue)
	};

	sf::Vertex RAX2[] = {
		sf::Vertex(sf::Vector2f(50.0f, 300.0f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(63.0f, 325.0f), sf::Color::Blue)
	};

	sf::Vertex LAY2[] = {
		sf::Vertex(sf::Vector2f(795.0f, 450.0f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(770.0f, 437.0f), sf::Color::Blue)
	};

	sf::Vertex RAY2[] = {
		sf::Vertex(sf::Vector2f(795.0f, 450.0f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(770.0f, 463.0f), sf::Color::Blue)
	};

	sf::Text positionTitle;
	sf::Text positionY;
	sf::Text timeX;
	sf::Text outputTitle;
	sf::Text outputY;
	sf::Font font;

	sf::Text minPos;
	sf::Text maxPos;
	sf::Text minAngle;
	sf::Text maxAngle;

	font.loadFromFile("Assets/Fonts/RusticBlackShadow.ttf");

	minPos.setString("0");
	minPos.setColor(sf::Color::Red);
	minPos.setCharacterSize(20);
	minPos.setPosition(sf::Vector2f(5.0f, 190.0f));
	minPos.setFont(font);
	
	std::string str = std::to_string(beamLength);
	str.resize(str.find_first_of('.') + 2);
	maxPos.setString(str);
	maxPos.setColor(sf::Color::Red);
	maxPos.setCharacterSize(20);
	maxPos.setPosition(sf::Vector2f(5.0f, 25.0f));
	maxPos.setFont(font);
	
	maxAngle.setString("90");
	maxAngle.setColor(sf::Color::Blue);
	maxAngle.setCharacterSize(20);
	maxAngle.setPosition(sf::Vector2f(5.0f, 285.0f));
	maxAngle.setFont(font);
	
	minAngle.setString("n90");
	minAngle.setColor(sf::Color::Blue);
	minAngle.setCharacterSize(20);
	minAngle.setPosition(sf::Vector2f(4.0f, 430.0f));
	minAngle.setFont(font);

	positionTitle.setString("Ball Position Over Time");
	positionTitle.setColor(sf::Color::Red);
	positionTitle.setFont(font);
	positionTitle.setCharacterSize(20);
	positionTitle.setPosition(sf::Vector2f(250.0f, 5.0f));

	positionY.setString("Position");
	positionY.setRotation(-90);
	positionY.setColor(sf::Color::Red);
	positionY.setCharacterSize(20);
	positionY.setPosition(sf::Vector2f(5.0f, 170.0f));
	positionY.setFont(font);

	outputY.setString("Output");
	outputY.setRotation(-90);
	outputY.setColor(sf::Color::Blue);
	outputY.setCharacterSize(20);
	outputY.setPosition(sf::Vector2f(5.0f, 415.0f));
	outputY.setFont(font);

	outputTitle.setString("Regulator Output Over Time");
	outputTitle.setColor(sf::Color::Blue);
	outputTitle.setCharacterSize(20);
	outputTitle.setPosition(sf::Vector2f(230.0f, 255.0f));
	outputTitle.setFont(font);

	timeX.setString("Time");
	timeX.setColor(sf::Color::Red);
	timeX.setCharacterSize(20);
	timeX.setPosition(sf::Vector2f(370.0f, 205.0f));
	timeX.setFont(font);

	win->draw(positionTitle);
	win->draw(outputTitle);
	win->draw(outputY);
	win->draw(positionY);
	win->draw(timeX);
	win->draw(minPos);
	win->draw(maxPos);
	win->draw(minAngle);
	win->draw(maxAngle);

	timeX.setColor(sf::Color::Blue);
	timeX.setPosition(sf::Vector2f(370.0f, 455.0f));

	win->draw(timeX);

	win->draw(axisX1, 2, sf::Lines);
	win->draw(axisY1, 2, sf::Lines);
	win->draw(axisX2, 2, sf::Lines);
	win->draw(axisY2, 2, sf::Lines);

	win->draw(LAX1, 2, sf::Lines);
	win->draw(RAX1, 2, sf::Lines);
	win->draw(LAY1, 2, sf::Lines);
	win->draw(RAY1, 2, sf::Lines);
	win->draw(LAX2, 2, sf::Lines);
	win->draw(RAX2, 2, sf::Lines);
	win->draw(LAY2, 2, sf::Lines);
	win->draw(RAY2, 2, sf::Lines);


	// Actual Graphs
	struct HelperVert {
		sf::Vertex arr[2];
	};

	std::vector<HelperVert> positionLines;
	std::vector<HelperVert> outputLines;

	for (uint32 i = 0; i < graphTimes.size() - 1; ++i) {
		sf::Vertex pos[2] = {
			sf::Vertex(sf::Vector2f(50 + (graphTimes[i] * (745 / graphTimes[graphTimes.size() - 1])), 
									50 + (150 - (positionOverTime[i] * (150 / beamLength)))), sf::Color::Red),
			sf::Vertex(sf::Vector2f(((i + 1) == graphTimes.size()) ? 795 : (50 + (graphTimes[i+1] * (745 / graphTimes[graphTimes.size() - 1]))), 
									50 + (150 - (positionOverTime[i + 1] * (150 / beamLength)))), sf::Color::Red)
		};

		sf::Vertex out[2] = {
			sf::Vertex(sf::Vector2f(50 + (graphTimes[i] * (745 / graphTimes[graphTimes.size() - 1])), 
									300 + (75 - (((90 * outputOverTime[i])) * (150.0f / 220.0f)))), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(((i + 1) == graphTimes.size()) ? 795 : (50 + (graphTimes[i + 1] * (745 / graphTimes[graphTimes.size() - 1]))), 
									300 + (75 - (((90 * outputOverTime[i + 1])) * (150.0f / 220.0f)))), sf::Color::Blue)
		};

		HelperVert p;
		HelperVert o;
		p.arr[0] = pos[0];
		p.arr[1] = pos[1];
		o.arr[0] = out[0];
		o.arr[1] = out[1];

		positionLines.push_back(p);
		outputLines.push_back(o);
	}

	for(uint32 i = 0; i < positionLines.size(); ++i) {
		win->draw(positionLines[i].arr, 2, sf::Lines);
		win->draw(outputLines[i].arr, 2, sf::Lines);
	}
}

void PhysicalModel::resetGraphs()
{
	totalTime = 0.0f;

	graphTimes.clear();
	positionOverTime.clear();
	outputOverTime.clear();

	graphTimes.push_back(0.0f);
	positionOverTime.push_back(0.0f);
	outputOverTime.push_back(0.0f);
}
