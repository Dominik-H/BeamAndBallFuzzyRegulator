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
	physicalWorld.Init(800, 600, 0.1f);

	this->window = window;

	// Ball
	sf::CircleShape* shape = new sf::CircleShape(30.f);
	shape->setFillColor(sf::Color::Green);
	shape->move(DRIFT + 280, 150);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("ball", shape));

	// Beam
	sf::RectangleShape* beam = new sf::RectangleShape(sf::Vector2f(300.0f, 6.0f));
	beam->setFillColor(sf::Color(139, 69, 19, 255));
	beam->move(DRIFT + 150, 210);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("beam", beam));

	// Connnector
	sf::RectangleShape* connector = new sf::RectangleShape(sf::Vector2f(6.0f, 240.0f));
	connector->setFillColor(sf::Color(100, 100, 100, 255));
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
	servo->move(DRIFT + 330, 276);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("servo", servo));

	// Box
	sf::RectangleShape* box = new sf::RectangleShape(sf::Vector2f(180.0f, 190.0f));
	box->setFillColor(sf::Color::Blue);
	box->move(DRIFT + 60, 372);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("box", box));

	// LeftLayout
	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(200.0f, 600.0f));
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("layoutBg", rect));

	return true;
}

void Application::Update(sf::Time dt) 
{
	physicalWorld.Update(dt.asMilliseconds());
	std::map<std::string, b2Body*>* bodies = physicalWorld.GetBodies();

	// Update code...
	// Ball
	modelObjects.find("ball")->second->setRotation(bodies->find("ball")->second->GetAngle() * 180 / b2_pi);
	// 30 - radius ... in pixels
	modelObjects.find("ball")->second->setPosition(DRIFT - 30 + bodies->find("ball")->second->GetPosition().x * 60, window->getSize().y - 30 - bodies->find("ball")->second->GetPosition().y * 60);
	
	// Beam
	modelObjects.find("beam")->second->setRotation(bodies->find("beam")->second->GetTransform().q.GetAngle() * 180 / b2_pi);
	// 150 - half beam width; 3 - half beam height; ... in pixels! (send to physics in meters "div 60")
	modelObjects.find("beam")->second->setPosition(DRIFT - 150 + bodies->find("beam")->second->GetPosition().x * 60, window->getSize().y - 3 - bodies->find("beam")->second->GetPosition().y * 60);

	// Connector
	modelObjects.find("connector")->second->setRotation(bodies->find("conn")->second->GetTransform().q.GetAngle() * 180 / b2_pi);
	// 3 - half beam width; 120 - half beam height; ... in pixels! (send to physics in meters "div 60")
	modelObjects.find("connector")->second->setPosition(DRIFT - 3 + bodies->find("conn")->second->GetPosition().x * 60, window->getSize().y - 120 - bodies->find("conn")->second->GetPosition().y * 60);

	// Servo
	modelObjects.find("servo")->second->setRotation(bodies->find("servo")->second->GetTransform().q.GetAngle() * 180 / b2_pi);
	// 60 - radius ... in pixels
	modelObjects.find("servo")->second->setPosition(DRIFT - 60 + bodies->find("servo")->second->GetPosition().x * 60, window->getSize().y - 60 - bodies->find("servo")->second->GetPosition().y * 60);
		
	totalTime += dt.asMilliseconds();

	if (totalTime >= 5000)
		modelObjects.find("ball")->second->setFillColor(sf::Color::Red);
}

void Application::Draw()
{
	// Draw code ...
	window->clear();

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
