#include "pch.h"
#include "Application.h"


Application::Application()
	:window(0)
{
	totalTime = 0;
}


Application::~Application()
{
	for (int i = modelObjects.size() - 1; i >= 0; --i)
	{
		delete modelObjects[i];
	}

	for (int i = layoutObjects.size() - 1; i >= 0; --i)
	{
		delete layoutObjects[i];
	}
}

bool Application::Init(sf::RenderWindow* window)
{
	this->window = window;

	// Other Init code...
	sf::CircleShape* shape = new sf::CircleShape(100.f);
	shape->setFillColor(sf::Color::Green);

	modelObjects.push_back(shape);

	return true;
}

void Application::Update(sf::Time dt)
{
	// Update code...
	totalTime += dt.asMilliseconds();

	if (totalTime >= 5000)
		modelObjects[0]->setFillColor(sf::Color::Red);
}

void Application::Draw()
{
	// Draw code ...
	window->clear();

	for (uint32 i = 0; i < modelObjects.size(); ++i)
	{
		window->draw(*(modelObjects[i]));
	}

	for (uint32 i = 0; i < layoutObjects.size(); ++i)
	{
		window->draw(*(layoutObjects[i]));
	}

	window->display();
}
