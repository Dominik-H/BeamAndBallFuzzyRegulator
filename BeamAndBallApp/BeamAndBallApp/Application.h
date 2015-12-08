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
	
	void setTextB1(std::string text){
		ztextB1.setString(text);
	}

	std::string getTextB1(){
		return ztextB1.getString();
	}
	
	void setTextB2(std::string text){
		ztextB2.setString(text);
	}

	std::string getTextB2(){
		return ztextB2.getString();
	}

	sf::Shape* getShape(std::string text ){
		return layoutObjects.find(text)->second;
	}

	
	
private:
	PhysicalModel physicalWorld;
	std::map<std::string, sf::Shape*> modelObjects;
	std::map<std::string, sf::Shape*> layoutObjects;

	sf::Text ztextB1;
	sf::Text ztextB2;
	sf::Text ztextB3;

	sf::Text ztextN1;
	sf::Text ztextN2;
	sf::Text ztextN3;

	sf::Texture texture;
	sf::Texture roof;
	sf::Texture house;
	sf::Texture menu;
	sf::Texture tree;
	sf::Texture background;
	sf::Texture button;
	sf::Texture buttons;

	//sf::String name;
	sf::Font font;
	sf::Event event;

	sf::RenderWindow* window; 
	SFMLDebugDraw* dDraw;

	int totalTime;
};

