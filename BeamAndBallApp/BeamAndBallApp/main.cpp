#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "fl\Headers.h"

#include <stdio.h>

int main(int argc, char** argv)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Beam & Ball App", sf::Style::Default, settings);

	// Init Graphics Module that will Init all the other modules

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Update Graphics Module - this updates all the modules
		// Draw Graphics

	}

	return 0;
}