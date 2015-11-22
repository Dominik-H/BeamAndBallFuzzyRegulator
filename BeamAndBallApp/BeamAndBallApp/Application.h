#pragma once

#include "pch.h"
#include "PhysicalModel.h"

class Application
{
public:
	Application();
	~Application();

	bool Init(sf::RenderWindow* window);
	void Update(sf::Time dt);
	void Draw();

private:
	PhysicalModel physicalWorld;
	std::vector<sf::Shape*> modelObjects;
	std::vector<sf::Shape*> layoutObjects;

	sf::RenderWindow* window;

	int totalTime;
};

