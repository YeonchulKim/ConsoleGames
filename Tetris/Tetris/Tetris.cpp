#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
using namespace sf;

#ifdef _DEBUG
#pragma comment (lib,"sfml-graphics-d.lib")
#pragma comment (lib,"sfml-system-d.lib")
#pragma comment (lib,"sfml-window-d.lib")
#pragma comment (lib,"sfml-audio-d.lib")
#else
#pragma comment (lib,"sfml-graphics.lib")
#pragma comment (lib,"sfml-system.lib")
#pragma comment (lib,"sfml-window.lib")
#pragma comment (lib,"sfml-audio.lib")
#endif

const int M = 20;
const int N = 10;
bool gameover = false;

int field[M][N] = { 0 };

struct Point
{
	int x, y;
} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	//gameover
	

	for (int i = 0; i < 4; i++)
	{
		if (field[0][a[i].x] != 0)
			gameover = true;

		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
			return 0;
		else if (field[a[i].y][a[i].x])
			return 0;	

		
	}

	return 1;
};

void init()
{
	time_t curTime;
	srand((unsigned)time(&curTime));

	int colorNum = 1 + rand() % 7;
	int n = curTime % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}
}
int main()
{
	time_t curTime;	
	srand((unsigned)time(&curTime));

	init();

	SoundBuffer sbuf;
	if (!sbuf.loadFromFile("sounds/BGM Tetris Bradinsky.wav"))
	{
		return -1;
	}
	
	Sound bgm;
	bgm.setBuffer(sbuf);
	bgm.play();

	RenderWindow window(VideoMode(320, 480), "The Game!");

	Texture t1, t2, t3, t4;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");
	t4.loadFromFile("images/game_over.png");

	Sprite s(t1), background(t2), frame(t3);
	Sprite over(t4);

	int dx = 0; bool rotate = 0; int colorNum = 1;
	float timer = 0.0f, delay = 0.3f;

	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) rotate = true;
				else if (e.key.code == Keyboard::Left) dx = -1;
				else if (e.key.code == Keyboard::Right) dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05f;

		//// <- Move -> ///
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

		//////Rotate//////
		if (rotate)
		{
			Point p = a[1]; //center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
		}

		///////Tick//////
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		///////check lines//////////
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;			
		}

		dx = 0; rotate = false; delay = 0.3f;

		/////////draw//////////
		window.clear(Color::White);
		window.draw(background);

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition((float)(j * 18), (float)(i * 18));
				s.move(28, 31); //offset
				window.draw(s);
			}

		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition((float)(a[i].x * 18),(float)( a[i].y * 18));
			s.move(28, 31); //offset
			window.draw(s);
		}

		window.draw(frame);
		if (gameover == true)
		{
			over.move(28, 31); //offset
			window.draw(over);
		}
		
		window.display();
		if (gameover) break;
	}
	getchar();
	return 0;
}
