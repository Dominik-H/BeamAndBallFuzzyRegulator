#include "pch.h"
#include "Application.h"

int main(int argc, char** argv)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Beam & Ball App", sf::Style::Default, settings);

	sf::Clock clock;

	// Init Graphics Module that will Init all the other modules
	Application app;
	app.Init(&window);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Update Graphics Module - this updates all the modules
		app.Update(clock.restart());
		app.Draw();
	}

	return 0;
}