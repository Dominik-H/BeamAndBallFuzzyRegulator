#include "pch.h"
#include "Application.h"
#define DRIFT 200


Application::Application()
	:window(0)
{
	totalTime = 0;
}


Application::~Application()
{
	for (auto i = modelObjects.begin(); i != modelObjects.end(); ++i)
	{
		delete i->second;
	}

	for (auto i = layoutObjects.begin(); i != layoutObjects.end(); ++i)
	{
		delete i->second;
	}
}

bool Application::Init(sf::RenderWindow* window)
{
	this->window = window;
	//dDraw = new SFMLDebugDraw(*window);

	physicalWorld.Init(800, 600, 2.0f);

	//physicalWorld.GetWorld()->SetDebugDraw(dDraw);
	//dDraw->SetFlags(b2Draw::e_shapeBit);
	
	// Ball
	sf::CircleShape* shape = new sf::CircleShape(30.f);
	shape->setFillColor(sf::Color::Green);
	shape->setOrigin(30, 30);
	shape->move(DRIFT + 280, 150);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("zball", shape));

	// Beam
	sf::RectangleShape* beam = new sf::RectangleShape(sf::Vector2f(300.0f, 6.0f));
	beam->setFillColor(sf::Color(139, 69, 19, 255));
	beam->setOrigin(150, 3.0f);
	beam->move(DRIFT + 150, 210);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("beam", beam));

	// Connnector
	sf::RectangleShape* connector = new sf::RectangleShape(sf::Vector2f(6.0f, 120.0f));
	connector->setFillColor(sf::Color(100, 100, 100, 255));
	connector->setOrigin(3, 60);
	connector->move(DRIFT + 450, 216);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("connector", connector));

	// Triangle
	sf::CircleShape* triangle = new sf::CircleShape(103.923f,3);
	triangle->setFillColor(sf::Color::Red);
	triangle->move(DRIFT + 46, 216);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("triangle", triangle));

	// Servo
	sf::CircleShape* servo = new sf::CircleShape(60.f);
	servo->setFillColor(sf::Color::Yellow);
	servo->setOrigin(60,60);
	servo->move(DRIFT + 330, 276);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("aservo", servo));

	// Box
	//sf::RectangleShape* box = new sf::RectangleShape(sf::Vector2f(180.0f, 190.0f));
	//box->setFillColor(sf::Color::Blue);
	//box->move(DRIFT + 60, 372);
	//modelObjects.insert(std::pair<std::string, sf::Shape*>("box", box));

	// LeftLayout
	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(200.0f, 600.0f));
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("layoutBg", rect));

	return true;
}

void Application::Update(sf::Time dt) 
{
	physicalWorld.Update(dt.asSeconds(), 0.0f);
	std::map<std::string, b2Body*>* bodies = physicalWorld.GetBodies();
	
	// Update code...
	// Ball
	modelObjects.find("zball")->second->setRotation(bodies->find("ball")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 30 - radius ... in pixels
	modelObjects.find("zball")->second->setPosition(DRIFT + bodies->find("ball")->second->GetPosition().x * 60, window->getSize().y - bodies->find("ball")->second->GetPosition().y * 60);
	
	// Beam
	modelObjects.find("beam")->second->setRotation(bodies->find("beam")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 150 - half beam width; 3 - half beam height; ... in pixels! (send to physics in meters "div 60")
	modelObjects.find("beam")->second->setPosition(DRIFT + bodies->find("beam")->second->GetPosition().x * 60, window->getSize().y - bodies->find("beam")->second->GetPosition().y * 60);

	// Connector
	modelObjects.find("connector")->second->setRotation(bodies->find("conn")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 3 - half beam width; 120 - half beam height; ... in pixels! (send to physics in meters "div 60")
	modelObjects.find("connector")->second->setPosition(DRIFT  + bodies->find("conn")->second->GetPosition().x * 60, window->getSize().y  - bodies->find("conn")->second->GetPosition().y * 60);

	// Servo
	modelObjects.find("aservo")->second->setRotation(bodies->find("servo")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 60 - radius ... in pixels
	modelObjects.find("aservo")->second->setPosition(DRIFT  + bodies->find("servo")->second->GetPosition().x * 60, window->getSize().y  - bodies->find("servo")->second->GetPosition().y * 60);
		
	totalTime += dt.asMilliseconds();
	
}

void Application::Draw()
{
	// Draw code ...
	window->clear();

	//physicalWorld.GetWorld()->DrawDebugData();

	for (auto i = modelObjects.begin(); i != modelObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	for (auto i = layoutObjects.begin(); i != layoutObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	window->display();
}
