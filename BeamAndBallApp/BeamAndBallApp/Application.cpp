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

	// Other Init code...
	// Ball
	sf::CircleShape* shape = new sf::CircleShape(30.f);
	shape->setFillColor(sf::Color::Green);
	shape->move(DRIFT + 280, 240);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("ball", shape));

	// Beam
	sf::RectangleShape* beam = new sf::RectangleShape(sf::Vector2f(300.0f, 10.0f));
	beam->setFillColor(sf::Color(139, 69, 19, 255));
	beam->move(DRIFT + 150, 300);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("beam", beam));
	// Connnector

	// Triangle


	//LeftLeyout
	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(200.0f, 600.0f));
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("layoutBg", rect));

	return true;
}

void Application::Update(sf::Time dt) 
{
	// Update code...
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
