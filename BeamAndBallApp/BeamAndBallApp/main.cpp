#include "pch.h"
#include "Application.h"

void DrawAbout(sf::RenderWindow* win);

int main(int argc, char** argv)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Beam & Ball App", sf::Style::Default, settings);
	sf::RenderWindow* aboutWindow = nullptr;
	bool aboutWin = false;
	bool graphsWin = false;
	sf::RenderWindow* graphsWindow = nullptr;
	window.setFramerateLimit(60);

	BandB_Data dat;

	sf::Clock clock;

	// Init Graphics Module that will Init all the other modules
	Application app;
	app.Init(&window);

	while (window.isOpen())
	{
		sf::Event event;
		if (aboutWin) {
			aboutWindow->pollEvent(event);
			if (event.type == sf::Event::Closed) {
				aboutWindow->close();
				aboutWin = false;
			}
		}

		if (graphsWin) {
			graphsWindow->pollEvent(event);
			if (event.type == sf::Event::Closed) {
				graphsWindow->close();
				graphsWin = false;
			}
		}

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				// Position Field
				if (((sf::Mouse::getPosition(window).y > 50) && (sf::Mouse::getPosition(window).y < 80)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("des_pos");
				}
				else {
					app.resetField("des_pos");
				}

				// Beam Length Field
				if (((sf::Mouse::getPosition(window).y > 120) && (sf::Mouse::getPosition(window).y < 150)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("beam_len");
				}
				else {
					app.resetField("beam_len");
				}

				// Ball Radius Field
				if (((sf::Mouse::getPosition(window).y > 180) && (sf::Mouse::getPosition(window).y < 210)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("ball_rad");
				}
				else {
					app.resetField("ball_rad");
				}

				// Send Button
				if (((sf::Mouse::getPosition(window).y > 260) && (sf::Mouse::getPosition(window).y < 290)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setButtonActive("send");
					//send();
					//app.setTextB1("");
					//app.setTextB2("");
				}

				// About Button 
				if (((sf::Mouse::getPosition(window).y > 550) && (sf::Mouse::getPosition(window).y < 590)) && ((sf::Mouse::getPosition(window).x > 10) && (sf::Mouse::getPosition(window).x < 50))){
					app.resetFields();
					app.setButtonActive("about");
					aboutWin = true;
					aboutWindow = new sf::RenderWindow(sf::VideoMode(400, 400), "Beam & Ball App - About", sf::Style::Default);
					DrawAbout(aboutWindow);
				}
			}
			else {
				app.resetButtons();
			}

			if (event.type == sf::Event::TextEntered){
				if (app.getFieldStatus("des_pos")){
					if (((event.text.unicode <= 59 && event.text.unicode >= 48) || (event.text.unicode == 46) || (event.text.unicode == 8)))
					{
						std::string nameB1 = app.getTextString("des_pos");

						if (event.text.unicode == 8)  { // backspace
							if (nameB1.size() > 0) nameB1.resize(nameB1.size() - 1);
						}
						else {
							if (nameB1.size() < 7)
								nameB1 += static_cast<char>(event.text.unicode);
						}
						app.setTextString("des_pos", nameB1);

					}
				}			

				if (app.getFieldStatus("beam_len")){
					if (((event.text.unicode <= 59 && event.text.unicode >= 48) || (event.text.unicode == 46) || (event.text.unicode == 8)))
					{
						std::string nameB2 = app.getTextString("beam_len");

						if (event.text.unicode == 8) { // backspace
							if (nameB2.size() > 0) nameB2.resize(nameB2.size() - 1);
						}
						else {
							if (nameB2.size() < 7)
								nameB2 += static_cast<char>(event.text.unicode);
						}
						app.setTextString("beam_len", nameB2);
					}
				}
			
				if (app.getFieldStatus("ball_rad")){
					if (((event.text.unicode <= 59 && event.text.unicode >= 48) || (event.text.unicode == 46) || (event.text.unicode == 8)))
					{
						std::string nameB4 = app.getTextString("ball_rad");

						if (event.text.unicode == 8) { // backspace
							if (nameB4.size() > 0) nameB4.resize(nameB4.size() - 1);
						}
						else {
							if (nameB4.size() < 7)
								nameB4 += static_cast<char>(event.text.unicode);
						}
						app.setTextString("ball_rad", nameB4);
					}
				}
			}
		}


		// Update Graphics Module - this updates all the modules
		app.Update(clock.restart());
		app.Draw();

		if (graphsWin) {
			// app.UpdateGraphs();
			// app.DrawGraphs(graphsWindow);
		}
	}

	return 0;
}

void DrawAbout(sf::RenderWindow* win)
{
	// Init About Stuff
	sf::RectangleShape* about = new sf::RectangleShape(sf::Vector2f(400.0f, 400.0f));
	sf::Texture aboutbg;
	aboutbg.loadFromFile("Assets/Textures/aboutBackground.png");
	about->setTexture(&aboutbg);

	win->clear(sf::Color::Black);
	
	// Draw Stuff
	win->draw(*about);

	win->display();
}
