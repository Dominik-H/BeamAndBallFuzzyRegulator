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
	//dDraw = new SFMLDebugDraw(*window);

	physicalWorld.Init(800, 600, 2.0f);

	//physicalWorld.GetWorld()->SetDebugDraw(dDraw);
	//dDraw->SetFlags(b2Draw::e_shapeBit);
	
	if (!texture.loadFromFile("Assets/Textures/ball.png"))
		return -1;
	texture.setSmooth(true);
	if (!roof.loadFromFile("Assets/Textures/roof1.png"))
		return -1;
	roof.setSmooth(true);
	if (!house.loadFromFile("Assets/Textures/house2.png"))
		return -1;
	if (!menu.loadFromFile("Assets/Textures/wood.png"))
		return -1;
	if (!tree.loadFromFile("Assets/Textures/tree1.png"))
		return -1;
	if (!background.loadFromFile("Assets/Textures/background3.png"))
		return -1;
	if (!button.loadFromFile("Assets/Textures/Botton.png"))
		return -1;
	if (!buttons.loadFromFile("Assets/Textures/buttons.png"))
		return -1;
	if (!about.loadFromFile("Assets/Textures/about.png"))
		return -1;
	

	// Ball
	sf::CircleShape* shape = new sf::CircleShape(30.f);
	//shape->setFillColor(sf::Color::Red);
	shape->setTexture(&texture);
	shape->setOrigin(30, 30);
	shape->move(DRIFT + 280, 150);
	modelObjects.insert(std::pair<std::string, sf::Shape*>("zball", shape));

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
	
	// BACKGROUND
	sf::RectangleShape* bg = new sf::RectangleShape(sf::Vector2f(600.0f, 600.0f));
	bg->setTexture(&background);
	bg->move(DRIFT, 0);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("bg", bg));

	// LeftLayout
	sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(200.0f, 600.0f));
	//rect->setFillColor(sf::Color::Magenta);
	rect->setTexture(&menu);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("layoutBg", rect));
	///////////////////////////////TEXT INPTU///////////////////////////////////////
	
	////////////////////////////// BUTTTONS ////////////////////////////////////////

	// BUTTTON1 
	sf::RectangleShape* button1 = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	button1->setFillColor(sf::Color(61, 44, 7));
	button1->setTexture(&buttons);
	button1->move(50,50);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("zbutton1", button1));

	// BUTTON2
	sf::RectangleShape* button2 = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	button2->setFillColor(sf::Color(61, 44, 7));
	button2->setTexture(&buttons);
	button2->move(50, 120);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("zbutton2", button2));

	// BUTTON2
	sf::RectangleShape* button4 = new sf::RectangleShape(sf::Vector2f(100.0f, 30.0f));
	button4->setFillColor(sf::Color(61, 44, 7));
	button4->setTexture(&buttons);
	button4->move(50, 180);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("zbutton4", button4));
	
	/// BUTTON3  SEND
	sf::RectangleShape* button3 = new sf::RectangleShape(sf::Vector2f(100.0f, 40.0f));
	button3->setTexture(&button);
	button3->move(50, 260);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("zbutton3", button3));

	/// BUTTON5  ABOUT
	sf::RectangleShape* button5 = new sf::RectangleShape(sf::Vector2f(40.0f, 40.0f));
	button5->setTexture(&about);
	button5->move(10, 550);
	layoutObjects.insert(std::pair<std::string, sf::Shape*>("zbutton5", button5));
	//sf::String word;

	font.loadFromFile("Assets/Fonts/PINEWOOD.ttf");

	///Button1
	ztextB1.setFont(font);
	ztextB1.setPosition(50, 50);
	ztextB1.setColor(sf::Color::White);
	ztextB1.setCharacterSize(24);

	//Button2
	ztextB2.setFont(font);
	ztextB2.setPosition(50, 120);
	ztextB2.setColor(sf::Color::White);
	ztextB2.setCharacterSize(24);

	//Button3
	ztextB3.setFont(font);
	ztextB3.setPosition(72, 260);
	ztextB3.setColor(sf::Color::Black);
	ztextB3.setCharacterSize(24);
	ztextB3.setString("SEND");

	///Button 4
	ztextB4.setFont(font);
	ztextB4.setPosition(50, 180);
	ztextB4.setColor(sf::Color::White);
	ztextB4.setCharacterSize(24);


	//BUTTON1 nadpis
	ztextN1.setFont(font);
	ztextN1.setPosition(30, 20);
	ztextN1.setColor(sf::Color::White);
	ztextN1.setCharacterSize(16);
	ztextN1.setString("Ball weight in kg");

	//BUTTON 2 nadpis
	ztextN2.setFont(font);
	ztextN2.setPosition(30, 90);
	ztextN2.setColor(sf::Color::White);
	ztextN2.setCharacterSize(16);
	ztextN2.setString("Beam length in m");

	//BUTTON 4 nadpis
	ztextN4.setFont(font);
	ztextN4.setPosition(40, 160);
	ztextN4.setColor(sf::Color::White);
	ztextN4.setCharacterSize(16);
	ztextN4.setString("Ball size in m");



	return true;
}

void Application::Update(sf::Time dt) 
{
	physicalWorld.Update(dt.asSeconds(), 0.25f);
	std::map<std::string, b2Body*>* bodies = physicalWorld.GetBodies();
	
	// Update code...
	// Ball
	modelObjects.find("zball")->second->setRotation(bodies->find("ball")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 30 - radius ... in pixels
	modelObjects.find("zball")->second->setPosition(DRIFT + bodies->find("ball")->second->GetPosition().x * 60, window->getSize().y - bodies->find("ball")->second->GetPosition().y * 60);
	
	// Beam
	modelObjects.find("beam")->second->setRotation(bodies->find("beam")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 150 - half beam width; 3 - half beam height; ... in pixels! (send to physics in meters "div 60")
	modelObjects.find("beam")->second->setPosition(DRIFT + bodies->find("beam")->second->GetPosition().x * 60, window->getSize().y - bodies->find("beam")->second->GetPosition().y * 60);

	// Connector
	modelObjects.find("connector")->second->setRotation(bodies->find("conn")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 3 - half beam width; 120 - half beam height; ... in pixels! (send to physics in meters "div 60")
	modelObjects.find("connector")->second->setPosition(DRIFT  + bodies->find("conn")->second->GetPosition().x * 60, window->getSize().y  - bodies->find("conn")->second->GetPosition().y * 60);

	// Servo
	modelObjects.find("aservo")->second->setRotation(bodies->find("servo")->second->GetTransform().q.GetAngle() * -180 / b2_pi);
	// 60 - radius ... in pixels
	modelObjects.find("aservo")->second->setPosition(DRIFT  + bodies->find("servo")->second->GetPosition().x * 60, window->getSize().y  - bodies->find("servo")->second->GetPosition().y * 60);
		
	totalTime += dt.asMilliseconds();

	
	
}

void Application::Draw()
{

	// Draw code ...
	window->clear(sf::Color::White);
	//physicalWorld.GetWorld()->DrawDebugData();
	for (auto i = layoutObjects.begin(); i != layoutObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	for (auto i = modelObjects.begin(); i != modelObjects.end(); ++i)
	{
		window->draw(*(i->second));
	}

	
	window->draw(ztextB1);
	window->draw(ztextB2);
	window->draw(ztextB3);
	window->draw(ztextB4);
	window->draw(ztextN1);
	window->draw(ztextN2);
	window->draw(ztextN4);
	window->display();
}
