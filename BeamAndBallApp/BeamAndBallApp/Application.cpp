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

	// Other Init code...
	sf::CircleShape* shape = new sf::CircleShape(100.f);
	shape->setFillColor(sf::Color::Green);
	shape->move(DRIFT, 0);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("circle", shape));

	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(200, 600));
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("layoutBg", rect));

	return true;
}

void Application::Update(sf::Time dt)
{
	// Update code...
	totalTime += dt.asMilliseconds();

	if (totalTime >= 5000)
		modelObjects.find("circle")->second->setFillColor(sf::Color::Red);
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
