#pragma once
#include "PhysicsEngine.h"
#include "MyText.h"
#include "FPSCounter.h"
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class GameManager
{
public:
	GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font);
	static bool getIntroState();
	static int getObjectCount();
private:
	
	PhysicsEngine _physicsEngine;
    std::shared_ptr<sf::Font> _font;
	FPSCounter _fpsCounter;
	sf::RenderWindow* _window;
	std::vector<std::shared_ptr<MyCircle>> _circs;
	std::vector<std::unique_ptr<MyText>> _texts;
	//std::vector<std::unique_ptr<MyText>> _trash;
	sf::Clock _clock;
	sf::Time _dt;
	void handleInput(float deltaTime);
	void intro(float deltaTime);
	void update(float dt);
	void draw(float deltaTime);
	void run();
	void keyboardInput(float deltaTime, sf::Event);
	void mouseInput(float deltaTime, sf::Event);
	void disposeTrash();
	void showIntro();
	
};