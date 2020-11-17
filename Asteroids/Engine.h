#pragma once

#include "Resource.h"
#include "Walls.h"
#include "Ship.h"

#include <dwrite.h>
#include <d2d1.h>
#include "Asteroid.h"
#include "Projectile.h"
#include <random>
#include <list>

#define RESOLUTION_X 1000
#define RESOLUTION_Y 600

class Engine {
public:
	Engine();
	~Engine();

	HRESULT InitializeD2D(HWND hwnd);
	HRESULT Render();

	void KeyUp(WPARAM wParam);
	void KeyDown(WPARAM wParam);
	void Logic(double elapsedTime);

private:
	std::random_device rng;

	ID2D1Factory* direct2dFactory;
	ID2D1HwndRenderTarget* renderTarget;

	IDWriteFactory* dWriteFactory;
	IDWriteTextFormat* textFormat;
	ID2D1SolidColorBrush* whiteBrush;

	void InitializeTextAndScore();
	void DrawTextAndScore();
	void UpdateMovement();
	void DrawGameOver();
	void DrawPause();
	void DrawVictory();
	void DeleteProjectile(int i);

	bool LevelComplete();

	Walls* walls;
	Ship* ship;

	// TO-DO Adjust numbers of asteroids and projectiles
	Asteroid* asteroids[31];
	Projectile* projectiles[10];

	int numProjectiles = 0;
	int numAsteroids = 10;

	float shootDelay;
	float accumulatedTime;

	bool running = true;

	bool upPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;
	bool spacePressed = false;
	bool isPaused = false;
	bool victory = false;

	int lives = 3;
	int score = 0;

	// TO-DO Add levels?
	//int level = 1;
	int* pScore;
};