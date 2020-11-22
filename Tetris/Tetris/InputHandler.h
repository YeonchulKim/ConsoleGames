#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include "Game.h"
using namespace sf;

enum class Key
{
	BUTTON_X,
	BUTTON_B,
	BUTTON_A,
	BUTTON_Y
};

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(Game& game) = 0;
};

class LeftCommand : public Command //왼쪽 이동
{
public:
	virtual void execute(Game& game)
	{ 
		game.dx = -1; 
	}
};

class RightCommand : public Command //오른쪽 이동
{
public:
	virtual void execute(Game& game) { 
		game.dx = 1; 
	}
};

class ChangeBlock : public Command //블록 변경
{
public:
	virtual void execute(Game& game) { game.rotate = true; };
};

class ChangeSpeed : public Command
{
public:
	virtual void execute(Game& game) { game.delay = 0.05f; }
};

class InputHandler
{
public:
	InputHandler(){	
		btnX = new ChangeBlock(); //up 
		btnY = new ChangeSpeed(); //down
		btnA = new LeftCommand(); //left
		btnB = new RightCommand();//right
	};
	~InputHandler() {
		delete btnX;
		delete btnY;
		delete btnA;
		delete btnB;
	};

private:
	Command* btnX;
	Command* btnY;
	Command* btnA;
	Command* btnB;

	
	
public:
	Command* handleInput(RenderWindow& window)
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) return btnX;
				else if (e.key.code == Keyboard::Left) return btnA;
				else if (e.key.code == Keyboard::Right) return btnB;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) return btnY;		
		return nullptr;
	}	
};

