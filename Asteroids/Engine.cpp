#include "Framework.h"
#include "Engine.h"
#include "Walls.h"
#include "Ship.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")

Engine::Engine() : direct2dFactory(NULL), renderTarget(NULL) {

	// TO-DO Make it so asteroids don't spawn where player spawns
	std::mt19937 random_engine(rng());
	std::uniform_int_distribution<int> dist_x(0, 495);
	std::uniform_int_distribution<int> dist_y(0, 590);

	pScore = &score;

	walls = new Walls();
	ship = new Ship();

	for (int i = 0; i < numAsteroids; i++) {
		float x = dist_x(random_engine);
		float y = dist_y(random_engine);
		asteroids[i] = new Asteroid(25.0f, x, y);
	}

	shootDelay = 0.15f;
	accumulatedTime = 0.0f;
}

Engine::~Engine() {

	SafeRelease(&direct2dFactory);
	SafeRelease(&renderTarget);

	delete walls;
	delete ship;
}

HRESULT Engine::InitializeD2D(HWND hwnd) {
	D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct2dFactory);
	direct2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&renderTarget
	);

	InitializeTextAndScore();
	walls->InitializeD2D(renderTarget);
	ship->InitializeD2D(renderTarget);

	for (int i = 0; i < numAsteroids; i++) {
		asteroids[i]->InitializeD2D(renderTarget);
	}

	for (int i = 0; i < numProjectiles; i++) {
		projectiles[i]->InitializeD2D(renderTarget);
	}

	return S_OK;
}

void Engine::KeyUp(WPARAM wParam) {
	if (wParam == VK_UP) {
		upPressed = false;
	}

	if (wParam == VK_LEFT) {
		leftPressed = false;
	}

	if (wParam == VK_RIGHT) {
		rightPressed = false;
	}

	if (wParam == VK_SPACE) {
		spacePressed = false;
	}
}

void Engine::KeyDown(WPARAM wParam) {
	if (wParam == VK_UP) {
		upPressed = true;
	}

	if (wParam == VK_LEFT) {
		leftPressed = true;
	}

	if (wParam == VK_RIGHT) {
		rightPressed = true;
	}

	if (wParam == VK_SPACE) {
		spacePressed = true;
	}

	if (wParam == 0x50) {
		if (isPaused) {
			isPaused = false;
		}
		else {
			isPaused = true;
		}
	}
}

// TO-DO Add functionality for multiple lives
// TO-DO Add scorekeeping
void Engine::Logic(double elapsedTime) {

	if (!running || isPaused || victory) {
		return;
	}

	accumulatedTime += elapsedTime;
	if (accumulatedTime > shootDelay) {
		accumulatedTime = 0;
		if (spacePressed) {
			Projectile* p = ship->Shoot();
			p->InitializeD2D(renderTarget);
			projectiles[numProjectiles] = p;
			numProjectiles++;
		}
	}

	if (leftPressed) {
		ship->Rotate(-0.5f);
	}
	if (rightPressed) {
		ship->Rotate(0.5f);
	}
	if (upPressed) {
		ship->Accelerate();
	}
	else {
		ship->Decelerate();
	}
	ship->Update();

	for (int i = 0; i < numAsteroids; i++) {
		asteroids[i]->Update();
	}

	for (int i = 0; i < numProjectiles; i++) {
		projectiles[i]->Update();
		float x = projectiles[i]->GetCenter().x;
		float y = projectiles[i]->GetCenter().y;
		if (x > RIGHT_EDGE || x < 10.0f || y > 590.0f || y < 10.0f) {
			DeleteProjectile(i);
		}
	}

	for (int i = 0; i < numAsteroids; i++) {
		if (ship->CircleCollision(asteroids[i]->GetCenter().x, asteroids[i]->GetCenter().y, asteroids[i]->GetRadius())) {
			running = false;
		}
		for (int j = 0; j < numProjectiles; j++) {
			if (projectiles[j]->CircleCollision(asteroids[i]->GetCenter().x, asteroids[i]->GetCenter().y, asteroids[i]->GetRadius())) {
				Asteroid* asteroid = asteroids[i];
				float lastRadius = asteroid->GetRadius();
				Coord lastCenter = asteroid->GetCenter();
				DeleteProjectile(j);
				for (int k = i; k < numAsteroids - 1; k++) {
					asteroids[k] = asteroids[k + 1];
				}
				asteroids[numAsteroids - 1] = NULL;
				delete asteroid;
				numAsteroids--;
				i--;
				// TO-DO change if condition
				if (lastRadius > 10.0f && numAsteroids <= 28) {
					Asteroid* a1 = new Asteroid(lastRadius - 5.0f, lastCenter.x - 5.0f, lastCenter.y - 5.0f);
					a1->InitializeD2D(renderTarget);
					asteroids[numAsteroids] = a1;
					numAsteroids++;

					Asteroid* a2 = new Asteroid(lastRadius - 5.0f, lastCenter.x + 5.0f, lastCenter.y + 5.0f);
					a2->InitializeD2D(renderTarget);
					asteroids[numAsteroids] = a2;
					numAsteroids++;
				}
			}
		}
	}

	if (lives < 0) {
		running = false;
	}
}

void Engine::DeleteProjectile(int i) {
	Projectile* projectile = projectiles[i];
	for (int j = i; j < numProjectiles - 1; j++) {
		projectiles[j] = projectiles[j + 1];
	}
	projectiles[numProjectiles - 1] = NULL;
	delete projectile;
	numProjectiles--;
	i--;
}

HRESULT Engine::Render() {

	HRESULT hr;

	renderTarget->BeginDraw();
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	DrawTextAndScore();
	if (!running) {
		DrawGameOver();
	}
	if (victory) {
		DrawVictory();
	}
	if (isPaused) {
		DrawPause();
	}
	walls->DrawWalls(renderTarget);
	ship->DrawShip(renderTarget);

	for (int i = 0; i < numAsteroids; i++) {
		asteroids[i]->DrawAsteroid(renderTarget);
	}

	for (int i = 0; i < numProjectiles; i++) {
		projectiles[i]->DrawProjectile(renderTarget);
	}

	hr = renderTarget->EndDraw();

	return S_OK;
}

void Engine::InitializeTextAndScore() {
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(&dWriteFactory)
	);

	dWriteFactory->CreateTextFormat(
		L"OCR A Extended",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20,
		L"", //locale
		&textFormat
	);

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&whiteBrush
	);
}

void Engine::DrawVictory() {
	float middleX = (RIGHT_EDGE / 2);
	float middleY = (RESOLUTION_Y - TOP_EDGE) / 2;

	D2D1_RECT_F youWin = D2D1::RectF(middleX - 200, middleY, middleX + 200, middleY + 300);
	renderTarget->DrawText(
		L"YOU WIN",
		7,
		textFormat,
		youWin,
		whiteBrush
	);
}

void Engine::DrawGameOver() {
	float middleX = (RIGHT_EDGE / 2);
	float middleY = (RESOLUTION_Y - TOP_EDGE) / 2;

	D2D1_RECT_F gameOver = D2D1::RectF(middleX - 200, middleY, middleX + 200, middleY + 300);
	renderTarget->DrawText(
		L"GAME OVER",
		9,
		textFormat,
		gameOver,
		whiteBrush
	);
}

void Engine::DrawPause() {
	float middleX = (RIGHT_EDGE / 2);
	float middleY = (RESOLUTION_Y - TOP_EDGE) / 2;

	D2D1_RECT_F pause = D2D1::RectF(middleX - 200, middleY, middleX + 200, middleY + 300);
	renderTarget->DrawText(
		L"PAUSED",
		6,
		textFormat,
		pause,
		whiteBrush
	);
}

void Engine::DrawTextAndScore() {
	// Text and score
	float padding = (RESOLUTION_Y - (20 + 1) * 20) / 2;
	float centerRight = RESOLUTION_X - (RESOLUTION_X - padding - (20 + 2) * 20) / 2;

	D2D1_RECT_F livesRect = D2D1::RectF(centerRight - 200, padding + 50, centerRight + 200, padding + 100);
	WCHAR livesStr[64];
	swprintf_s(livesStr, L"Lives: %d ", lives);
	if (lives < 0) {
		swprintf_s(livesStr, L"Lives: %d ", 0);
	}
	renderTarget->DrawText(
		livesStr,
		8,
		textFormat,
		livesRect,
		whiteBrush
	);

	D2D1_RECT_F score1 = D2D1::RectF(centerRight - 200, padding + 100, centerRight + 200, padding + 150);
	renderTarget->DrawText(
		L"Score:",
		6,
		textFormat,
		score1,
		whiteBrush
	);


	D2D1_RECT_F score2 = D2D1::RectF(centerRight - 200, padding + 150, centerRight + 200, padding + 200);
	WCHAR scoreStr[64];
	swprintf_s(scoreStr, L"%d        ", score);
	renderTarget->DrawText(
		scoreStr,
		7,
		textFormat,
		score2,
		whiteBrush
	);
}