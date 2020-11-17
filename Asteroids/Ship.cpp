#include "Ship.h"
#include "FloatingObject.h"
#include "Walls.h"
#include "Projectile.h"
#include <cmath>
#include <vector>

Ship::Ship() {
	tip.x = 200.0f;
	tip.y = 300.0f;

	bottomLeft.x = 193.0f;
	bottomLeft.y = 325.0f;

	bottomRight.x = 207.0f;
	bottomRight.y = 325.0f;

	vertices.push_back(&tip);
	vertices.push_back(&bottomRight);
	vertices.push_back(&bottomLeft);

	radius = 15.0f;
	angle = 0.0f;

	CalculateCenter();
}

Ship::~Ship() {
	limeBrush->Release();
}

void Ship::DrawShip(ID2D1HwndRenderTarget* renderTarget) {
	float rads = CalculateRadians();

	D2D1_POINT_2F top = D2D1::Point2F(
		center.x - radius * cos(tipAngle + rads),
		center.y - radius * sin(tipAngle + rads)
	);

	tip.x = top.x;
	tip.y = top.y;

	D2D1_POINT_2F leftBottom = D2D1::Point2F(
		center.x - radius * cos(bottomLeftAngle + rads),
		center.y - radius * sin(bottomLeftAngle + rads)
	);

	bottomLeft.x = leftBottom.x;
	bottomLeft.y = leftBottom.y;

	D2D1_POINT_2F rightBottom = D2D1::Point2F(
		center.x - radius * cos(bottomRightAngle + rads),
		center.y - radius * sin(bottomRightAngle + rads)
	);

	bottomRight.x = rightBottom.x;
	bottomRight.y = rightBottom.y;

	//CalculateCenter();

	if (ResetPosition()) {
		return;
	}

	renderTarget->DrawLine(
		top,
		leftBottom,
		limeBrush
	);

	renderTarget->DrawLine(
		top,
		rightBottom,
		limeBrush
	);

	renderTarget->DrawLine(
		leftBottom,
		rightBottom,
		limeBrush
	);
}

void Ship::Rotate(float degree) {
	angle += degree;
}

void Ship::Decelerate() {
	accelerating = false;
}

void Ship::Accelerate() {
	radians = CalculateRadians();
	accelerating = true;
}

void Ship::Update() {
	if (accelerating) {
		if (speed < MAX_SPEED) {
			speed += 0.0015;
		}

		SetDirection();

		center.x -= direction.x;
		center.y -= direction.y;
	}
	else {
		if (speed > MIN_SPEED) {
			speed -= 0.00015;
		}

		SetDirection();

		center.x -= direction.x;
		center.y -= direction.y;
	}
	//CalculateCenter();
}

Projectile* Ship::Shoot() {
	return new Projectile(tip.x, tip.y, angle);
}