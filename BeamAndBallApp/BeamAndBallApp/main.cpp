#include "pch.h"
#include "Application.h"

void DrawAbout(sf::RenderWindow* win);

int main(int argc, char** argv)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Beam & Ball App", sf::Style::Close, settings);
	sf::RenderWindow* aboutWindow = nullptr;
	bool aboutWin = false;
	bool graphsWin = false;
	sf::RenderWindow* graphsWindow = nullptr;
	window.setFramerateLimit(60);

	BandB_Data dat;
	float desiredPos = 0.25f;

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

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				// Position Field
				if (((sf::Mouse::getPosition(window).y > 35) && (sf::Mouse::getPosition(window).y < 65)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("des_pos");
				}
				else {
					app.resetField("des_pos");
				}

				// Beam Length Field
				if (((sf::Mouse::getPosition(window).y > 135) && (sf::Mouse::getPosition(window).y < 165)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("beam_len");
				}
				else {
					app.resetField("beam_len");
				}

				// Ball Radius Field
				if (((sf::Mouse::getPosition(window).y > 195) && (sf::Mouse::getPosition(window).y < 225)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("ball_rad");
				}
				else {
					app.resetField("ball_rad");
				}

				// Ball Weight Field
				if (((sf::Mouse::getPosition(window).y > 255) && (sf::Mouse::getPosition(window).y < 285)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("ball_weight");
				}
				else {
					app.resetField("ball_weight");
				}

				// Servo Radius Field
				if (((sf::Mouse::getPosition(window).y > 315) && (sf::Mouse::getPosition(window).y < 345)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("servo_rad");
				}
				else {
					app.resetField("servo_rad");
				}

				// Servo Max Speed Field
				if (((sf::Mouse::getPosition(window).y > 375) && (sf::Mouse::getPosition(window).y < 405)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("servo_speed");
				}
				else {
					app.resetField("servo_speed");
				}

				// Gravitational Acceleration Field
				if (((sf::Mouse::getPosition(window).y > 435) && (sf::Mouse::getPosition(window).y < 465)) && ((sf::Mouse::getPosition(window).x > 50) && (sf::Mouse::getPosition(window).x < 150))){
					app.resetFields();
					app.setFieldActive("grav_acc");
				}
				else {
					app.resetField("grav_acc");
				}

				// Send Button
				if (((sf::Mouse::getPosition(window).y > 475) && (sf::Mouse::getPosition(window).y < 505)) && ((sf::Mouse::getPosition(window).x > 30) && (sf::Mouse::getPosition(window).x < 170))){
					app.resetFields();
					app.setButtonActive("send");
					//send();
				}

				// Update Button
				if (((sf::Mouse::getPosition(window).y > 70) && (sf::Mouse::getPosition(window).y < 100)) && ((sf::Mouse::getPosition(window).x > 30) && (sf::Mouse::getPosition(window).x < 170))){
					app.resetFields();
					app.setButtonActive("update");
					
					std::string pos = app.getTextString("des_pos");
					if (pos.empty() || pos[0] == '.' || std::count(pos.begin(), pos.end(), '.') > 1) {
						desiredPos = 0.25;
						app.setTextString("des_pos", "0.25");
					}
					else
					{
						desiredPos = std::stof(pos);
					}

					app.setDesiredPos(desiredPos);
				}

				// Graphs Button
				if (((sf::Mouse::getPosition(window).y > 515) && (sf::Mouse::getPosition(window).y < 545)) && ((sf::Mouse::getPosition(window).x > 30) && (sf::Mouse::getPosition(window).x < 170))){
					app.resetFields();
					app.setButtonActive("graphs");
					//send();
				}

				// About Button 
				if (((sf::Mouse::getPosition(window).y > 550) && (sf::Mouse::getPosition(window).y < 590)) && ((sf::Mouse::getPosition(window).x > 10) && (sf::Mouse::getPosition(window).x < 50))){
					app.resetFields();
					app.setButtonActive("about");
					if (!aboutWin)
					{
						aboutWin = true;
						aboutWindow = new sf::RenderWindow(sf::VideoMode(400, 400), "Beam & Ball App - About", sf::Style::Close);
						DrawAbout(aboutWindow);
					}
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
	sf::Texture aboutbg;
	aboutbg.loadFromFile("Assets/Textures/abg.png");
	sf::RectangleShape* aboutWin = new sf::RectangleShape(sf::Vector2f(400.0f, 400.0f));
	aboutWin->setTexture(&aboutbg);

	win->clear(sf::Color::Black);
	
	// Draw Stuff
	win->draw(*aboutWin);

	win->display();
}
