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
	//modelObjects.find("ball")->second->setRotation(bodies->find("ball")->second->GetTransform().q.GetAngle() * 180 / b2_pi);
	//modelObjects.find("ball")->second->setPosition(bodies->find("ball")->second->GetTransform().p.x * 60, bodies->find("ball")->second->GetTransform().p.y * 60);
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
