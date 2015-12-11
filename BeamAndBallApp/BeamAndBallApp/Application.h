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

	std::string getTextString(std::string textFieldName);
	void setTextString(std::string textFieldName, std::string text);
	sf::Shape* getShape(std::string text);

	void resetButtons();
	void resetFields();

	void resetField(std::string fieldName);

	void setFieldActive(std::string fieldName);
	void setButtonActive(std::string buttonName);

	bool getFieldStatus(std::string fieldName);
	
private:
	PhysicalModel physicalWorld;
	std::map<std::string, sf::Shape*> modelObjects;
	std::map<std::string, sf::Shape*> layoutObjects;
	std::map<std::string, sf::Shape*> buttonObjects;
	std::map<std::string, sf::Shape*> inFieldObjects;
	std::map<std::string, bool> inFieldStatus;
	std::map<std::string, sf::Text*> texts;

	sf::Texture staticBall;
	sf::Texture movingBall;
	sf::Texture roof;
	sf::Texture house;
	sf::Texture menu;
	sf::Texture tree;
	sf::Texture background;
	sf::Texture button;
	sf::Texture buttons;
	sf::Texture about;

	float oldPosX;
	float oldPosY;
	float oldRot;

	sf::Font font;
	sf::RenderWindow* window; 
	int totalTime;
};

