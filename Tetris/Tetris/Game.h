#pragma once
class Game
{
public:
	Game() :
		dx(0),
		rotate(false),
		delay(0.3f)
	{		
	}

	void reset()
	{
		dx = 0; rotate = false; delay = 0.3f;
	}
public:
	//friend class Command;
	int dx;
	bool rotate;
	float delay;
	
};

