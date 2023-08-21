#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#pragma once
#include <SFML/Graphics.hpp>;
#include <vector>;
#include "MyCircle.h";
#include "MyText.h";

class GameManager
{
public:
	GameManager(sf::RenderWindow* window);
	
private:
	sf::RenderWindow* _window;
	std::vector<MyCircle> _circs;
	std::vector<MyText> _texts;
	sf::Clock _clock;
	sf::Clock _fpsClock;
	sf::Time _dt;
	sf::Time _fpsTimer;
	int _frames;
	float _newTime;
	float _oldTime;
	void handleInput();
	void update(float dt);
	void checkOutOfB();
	void checkForCollisions();
	void draw();
	void run();
	void showFps();
	void showIntroText();
};
#endif