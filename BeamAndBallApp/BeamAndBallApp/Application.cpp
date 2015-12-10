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

	for (auto i = buttonObjects.begin(); i != buttonObjects.end(); ++i)
	{
		delete i->second;
	}

	for (auto i = texts.begin(); i != texts.end(); ++i)
	{
		delete i->second;
	}
}

bool Application::Init(sf::RenderWindow* window)
{
	this->window = window;

	// Init Physics
	physicalWorld.Init(800, 600, 2.0f);
	
	// Load Textures
	if (!texture.loadFromFile("Assets/Textures/ball.png"))
		return false;

	if (!roof.loadFromFile("Assets/Textures/roof1.png"))
		return false;

	if (!house.loadFromFile("Assets/Textures/house2.png"))
		return false;

	if (!menu.loadFromFile("Assets/Textures/wood.png"))
		return false;

	if (!tree.loadFromFile("Assets/Textures/tree1.png"))
		return false;

	if (!background.loadFromFile("Assets/Textures/background3.png"))
		return false;

	if (!button.loadFromFile("Assets/Textures/Botton.png"))
		return false;

	if (!buttons.loadFromFile("Assets/Textures/buttons.png"))
		return false;

	if (!about.loadFromFile("Assets/Textures/about.png"))
		return false;

	texture.setSmooth(true);
	roof.setSmooth(true);	
	
	// Objects Init

	// Ball
	sf::CircleShape* shape = new sf::CircleShape(30.f);
	//shape->setFillColor(sf::Color::Red);
	shape->setTexture(&texture);
	shape->setOrigin(30, 30);
	shape->move(DRIFT + 280, 150);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("zball", shape)); // Z is just to make it the last drawn item

	// Beam
	sf::RectangleShape* beam = new sf::RectangleShape(sf::Vector2f(300.0f, 6.0f));
	beam->setFillColor(sf::Color(139, 69, 19, 255));
	beam->setOrigin(150, 3.0f);
	beam->move(DRIFT + 150, 210);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("beam", beam));

	// Connnector
	sf::RectangleShape* connector = new sf::RectangleShape(sf::Vector2f(6.0f, 120.0f));
	connector->setFillColor(sf::Color(100, 100, 100, 255));
	connector->setOrigin(3, 60);
	connector->move(DRIFT + 450, 216);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("connector", connector));

	// Triangle
	sf::CircleShape* triangle = new sf::CircleShape(103.923f,3);
	//triangle->setFillColor(sf::Color::Red);
	triangle->setTexture(&roof);
	triangle->move(DRIFT + 46, 216);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("triangle", triangle));

	// Servo
	sf::CircleShape* servo = new sf::CircleShape(60.f);
	servo->setFillColor(sf::Color::Yellow);
	servo->setTexture(&tree);
	servo->setOrigin(60,60);
	servo->move(DRIFT + 330, 276);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("aservo", servo));

	// Box
	sf::RectangleShape* box = new sf::RectangleShape(sf::Vector2f(180.0f, 190.0f));
	box->setTexture(&house);
	box->move(DRIFT + 60, 372);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("box", box));
	
	// Background
	sf::RectangleShape* bg = new sf::RectangleShape(sf::Vector2f(600.0f, 600.0f));
	bg->setTexture(&background);
	bg->move(DRIFT, 0);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("bg", bg));

	// LeftLayout
	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(200.0f, 600.0f));
	//rect->setFillColor(sf::Color::Magenta);
	rect->setTexture(&menu);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("layoutBg", rect));

	// Input Fields

	// Position
	sf::RectangleShape* button1 = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	button1->setFillColor(sf::Color(61, 44, 7));
	button1->setTexture(&buttons);
	button1->move(50,50);
	inFieldObjects.insert(std::pair<std::string, sf::Shape*>("des_pos", button1));
	inFieldStatus.insert(std::pair<std::string, bool>("des_pos", false));

	// Beam Length
	sf::RectangleShape* button2 = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	button2->setFillColor(sf::Color(61, 44, 7));
	button2->setTexture(&buttons);
	button2->move(50, 120);
	inFieldObjects.insert(std::pair<std::string, sf::Shape*>("beam_len", button2));
	inFieldStatus.insert(std::pair<std::string, bool>("beam_len", false));

	// Ball Radius
	sf::RectangleShape* button4 = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	button4->setFillColor(sf::Color(61, 44, 7));
	button4->setTexture(&buttons);
	button4->move(50, 180);
	inFieldObjects.insert(std::pair<std::string, sf::Shape*>("ball_rad", button4));
	inFieldStatus.insert(std::pair<std::string, bool>("ball_rad", false));
	
	// Buttons

	// Send Button
	sf::RectangleShape* button3 = new sf::RectangleShape(sf::Vector2f(100.0f, 40.0f));
	button3->setTexture(&button);
	button3->move(50, 260);
	buttonObjects.insert(std::pair<std::string, sf::Shape*>("send", button3));

	// About Button
	sf::RectangleShape* button5 = new sf::RectangleShape(sf::Vector2f(40.0f, 40.0f));
	button5->setTexture(&about);
	button5->move(10, 550);
	buttonObjects.insert(std::pair<std::string, sf::Shape*>("about", button5));

	// Texts
	font.loadFromFile("Assets/Fonts/PINEWOOD.ttf");
	sf::Text* text;

	// Desired Position Field
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(50, 50);
	text->setColor(sf::Color::White);
	text->setCharacterSize(24);
	texts.insert(std::pair<std::string, sf::Text*>("des_pos", text));

	// Beam Length
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(50, 120);
	text->setColor(sf::Color::White);
	text->setCharacterSize(24);
	texts.insert(std::pair<std::string, sf::Text*>("beam_len", text));

	// Ball Radius
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(50, 180);
	text->setColor(sf::Color::White);
	text->setCharacterSize(24);
	texts.insert(std::pair<std::string, sf::Text*>("ball_rad", text));

	// Send Button
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(72, 260);
	text->setColor(sf::Color::Black);
	text->setCharacterSize(24);
	text->setString("SEND");
	texts.insert(std::pair<std::string, sf::Text*>("send", text));

	// Desired Position Title
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(30, 20);
	text->setColor(sf::Color::White);
	text->setCharacterSize(16);
	text->setString("Desired Ball Position in m");
	texts.insert(std::pair<std::string, sf::Text*>("des_pos_title", text));

	// Beam Length Title
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(30, 90);
	text->setColor(sf::Color::White);
	text->setCharacterSize(16);
	text->setString("Beam length in m");
	texts.insert(std::pair<std::string, sf::Text*>("beam_len_title", text));

	// Ball Radius Title
	text = new sf::Text();
	text->setFont(font);
	text->setPosition(40, 160);
	text->setColor(sf::Color::White);
	text->setCharacterSize(16);
	text->setString("Ball Radius in m");
	texts.insert(std::pair<std::string, sf::Text*>("ball_rad_title", text));

	return true;
}

void Application::Update(sf::Time dt) 
{
	physicalWorld.Update(dt.asSeconds(), 0.25f);
	std::map<std::string, b2Body*>* bodies = physicalWorld.GetBodies();
	
	// Update code...

	// Ball
	modelObjects.find("zball")->second->setRotation(bodies->find("ball")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	modelObjects.find("zball")->second->setPosition(DRIFT + bodies->find("ball")->second->GetPosition().x * 60, window->getSize().y - bodies->find("ball")->second->GetPosition().y * 60);
	
	// Beam
	modelObjects.find("beam")->second->setRotation(bodies->find("beam")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	modelObjects.find("beam")->second->setPosition(DRIFT + bodies->find("beam")->second->GetPosition().x * 60, window->getSize().y - bodies->find("beam")->second->GetPosition().y * 60);

	// Connector
	modelObjects.find("connector")->second->setRotation(bodies->find("conn")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	modelObjects.find("connector")->second->setPosition(DRIFT  + bodies->find("conn")->second->GetPosition().x * 60, window->getSize().y  - bodies->find("conn")->second->GetPosition().y * 60);

	// Servo
	modelObjects.find("aservo")->second->setRotation(bodies->find("servo")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	modelObjects.find("aservo")->second->setPosition(DRIFT  + bodies->find("servo")->second->GetPosition().x * 60, window->getSize().y  - bodies->find("servo")->second->GetPosition().y * 60);
		
	totalTime += dt.asMilliseconds();	
}

void Application::Draw()
{
	// Draw code ...
	window->clear(sf::Color::Black);

	for (auto i = layoutObjects.begin(); i != layoutObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	for (auto i = modelObjects.begin(); i != modelObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	for (auto i = inFieldObjects.begin(); i != inFieldObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	for (auto i = buttonObjects.begin(); i != buttonObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	for (auto i = texts.begin(); i != texts.end(); ++i)
	{
		window->draw(*(i->second));
	}

	window->display();
}

std::string Application::getTextString(std::string textFieldName) 
{
	return texts.find(textFieldName)->second->getString();
}

void Application::setTextString(std::string textFieldName, std::string text) 
{
	texts.find(textFieldName)->second->setString(text);
}

sf::Shape* Application::getShape(std::string text)
{
	return buttonObjects.find(text)->second;
}

void Application::resetButtons()
{
	for (auto i = buttonObjects.begin(); i != buttonObjects.end(); ++i)
	{
		i->second->setFillColor(sf::Color::White);
	}
}

void Application::resetFields()
{
	for (auto i = inFieldStatus.begin(); i != inFieldStatus.end(); ++i)
	{
		i->second = false;
	}

	for (auto i = inFieldObjects.begin(); i != inFieldObjects.end(); ++i)
	{
		i->second->setFillColor(sf::Color(61, 44, 7));
	}
}

void Application::setFieldActive(std::string fieldName)
{
	inFieldObjects.find(fieldName)->second->setFillColor(sf::Color(188, 136, 22));
	inFieldStatus.find(fieldName)->second = true;
}

void Application::setButtonActive(std::string buttonName)
{
	buttonObjects.find(buttonName)->second->setFillColor(sf::Color::Cyan);
}

bool Application::getFieldStatus(std::string fieldName)
{
	return inFieldStatus.find(fieldName)->second;
}

void Application::resetField(std::string fieldName)
{
	inFieldObjects.find(fieldName)->second->setFillColor(sf::Color(61, 44, 7));
	inFieldStatus.find(fieldName)->second = false;
}
