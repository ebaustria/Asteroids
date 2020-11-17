#pragma once

#include "Coord.h"
#include "Projectile.h"
#include "FloatingObject.h"
#include <d2d1.h>
//#include <string>

#define MAX_SPEED 0.4

class Ship : public FloatingObject {
public:
	Ship();
	~Ship();

	void DrawShip(ID2D1HwndRenderTarget* renderTarget);

	void Rotate(float degree);
	void Accelerate();
	void Decelerate();
	void Update();

	//float GetAngle();

	Projectile* Shoot();

private:
	Coord tip;
	Coord bottomLeft;
	Coord bottomRight;

	float tipAngle = 0.0f;
	float bottomLeftAngle = (3.0f * PI) / 4.0f;
	float bottomRightAngle = (5.0f * PI) / 4.0f;

	bool accelerating = false;
};