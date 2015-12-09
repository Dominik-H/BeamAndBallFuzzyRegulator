#include "pch.h"
#include "Application.h"

int main(int argc, char** argv)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Beam & Ball App", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	sf::Clock clock;

	// Init Graphics Module that will Init all the other modules
	Application app;
	app.Init(&window);
	bool activeB1 = false;
	bool activeB2 = false;
	bool activeB4 = false;
	//sf::Vector2i position = sf::Mouse::getPosition(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			///BUTTON 1
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				if (((sf::Mouse::getPosition(window).y > 50) && (sf::Mouse::getPosition(window).y < 80)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					activeB1 = true;
					app.getShape("zbutton1")->setFillColor(sf::Color(188, 136, 22));
					activeB2 = false;
					activeB4 = false;
					app.getShape("zbutton2")->setFillColor(sf::Color(61, 44, 7));
					app.getShape("zbutton4")->setFillColor(sf::Color(61, 44, 7));
				}
				else {
					activeB1 = false;
					app.getShape("zbutton1")->setFillColor(sf::Color(61, 44, 7));
				}
			}

			////BUTTTON 2
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				if (((sf::Mouse::getPosition(window).y > 120) && (sf::Mouse::getPosition(window).y < 150)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					activeB2 = true;
					app.getShape("zbutton2")->setFillColor(sf::Color(188, 136, 22));
					activeB1 = false;
					activeB4 = false;
					app.getShape("zbutton1")->setFillColor(sf::Color(61, 44, 7));
					app.getShape("zbutton4")->setFillColor(sf::Color(61, 44, 7));
				}
				else {
					activeB2 = false;
					app.getShape("zbutton2")->setFillColor(sf::Color(61, 44, 7));
				}
			}
			////BUTTON 4
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				if (((sf::Mouse::getPosition(window).y > 180) && (sf::Mouse::getPosition(window).y < 210)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					activeB4 = true;
					app.getShape("zbutton4")->setFillColor(sf::Color(188, 136, 22));
					activeB1 = false;
					activeB2 = false;
					app.getShape("zbutton1")->setFillColor(sf::Color(61, 44, 7));
					app.getShape("zbutton2")->setFillColor(sf::Color(61, 44, 7));
				}
				else {
					activeB4 = false;
					app.getShape("zbutton4")->setFillColor(sf::Color(61, 44, 7));
				}
			}
			////BUTTON3 (SEND)
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				if (((sf::Mouse::getPosition(window).y > 260) && (sf::Mouse::getPosition(window).y < 290)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					activeB2 = false;
					activeB1 = false;
					activeB4 = false;
					app.getShape("zbutton3")->setFillColor(sf::Color::Cyan);
					//send();
					//app.setTextB1("");
					//app.setTextB2("");
				}
			}
			else {
				app.getShape("zbutton3")->setFillColor(sf::Color::White);
			}

			///BUTTON5 ABOUT 
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				if (((sf::Mouse::getPosition(window).y > 550) && (sf::Mouse::getPosition(window).y < 590)) && ((sf::Mouse::getPosition(window).x > 10) && (sf::Mouse::getPosition(window).x < 50))){
					activeB2 = false;
					activeB1 = false;
					activeB4 = false;
					app.getShape("zbutton5")->setFillColor(sf::Color::Cyan);
					//send();
					//app.setTextB1("");
					//app.setTextB2("");
				}
			}
			else {
				app.getShape("zbutton5")->setFillColor(sf::Color::White);
			}

			if (activeB1 == true){
				if (event.type == sf::Event::TextEntered){
					if (((event.text.unicode <= 59 && event.text.unicode >= 48) || (event.text.unicode == 46) || (event.text.unicode == 8)))
					{
						std::string nameB1 = app.getTextB1();

						if (event.text.unicode == 8)  { // backspace
							if (nameB1.size() > 0) nameB1.resize(nameB1.size() - 1);
						}
						else {
							if (nameB1.size() < 7)
								nameB1 += static_cast<char>(event.text.unicode);
						}
						app.setTextB1(nameB1);

					}
				}
			}

			if (activeB2 == true){
				if (event.type == sf::Event::TextEntered){
					if (((event.text.unicode <= 59 && event.text.unicode >= 48) || (event.text.unicode == 46) || (event.text.unicode == 8)))
					{
						std::string nameB2 = app.getTextB2();

						if (event.text.unicode == 8) { // backspace
							if (nameB2.size() > 0) nameB2.resize(nameB2.size() - 1);
						}
						else {
							if (nameB2.size() < 7)
								nameB2 += static_cast<char>(event.text.unicode);
						}
						app.setTextB2(nameB2);
					}
				}
			}
	

		if (activeB4 == true){
			if (event.type == sf::Event::TextEntered){
				if (((event.text.unicode <= 59 && event.text.unicode >= 48) || (event.text.unicode == 46) || (event.text.unicode == 8)))
				{
					std::string nameB4 = app.getTextB4();

					if (event.text.unicode == 8) { // backspace
						if (nameB4.size() > 0) nameB4.resize(nameB4.size() - 1);
					}
					else {
						if (nameB4.size() < 7)
							nameB4 += static_cast<char>(event.text.unicode);
					}
					app.setTextB4(nameB4);
				}
			}
		}
	}


		// Update Graphics Module - this updates all the modules
		app.Update(clock.restart());
		app.Draw();
	}

	return 0;
}