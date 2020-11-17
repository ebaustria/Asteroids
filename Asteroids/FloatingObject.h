#pragma once
#include <d2d1.h>
#include "Coord.h"
#include <vector>

#define PI 3.1415926535
#define MIN_SPEED 0.01

class FloatingObject {
public:
	void InitializeD2D(ID2D1HwndRenderTarget* renderTarget);
	void SetDirection();
	void CalculateCenter();
	void Update();

	float CalculateRadians();

	bool ResetPosition();
	bool CircleCollision(float otherX, float otherY, float otherRadius);

protected:
	std::vector<Coord*> vertices;

	ID2D1SolidColorBrush* limeBrush;

	float angle;
	float radians;
	float speed = 0.025f;
	float radius;
	Coord direction;
	Coord center;
};