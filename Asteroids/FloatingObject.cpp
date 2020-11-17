#include "FloatingObject.h"
#include <cmath>
#include "Walls.h"

float FloatingObject::CalculateRadians() {
	return angle * (PI / 180);
}

void FloatingObject::CalculateCenter() {
	float centerX = 0.0f;
	float centerY = 0.0f;

	for (Coord* vertex : vertices) {
		float x = vertex->x;
		float y = vertex->y;
		centerX += x;
		centerY += y;
	}

	centerX = centerX / vertices.capacity();
	centerY = centerY / vertices.capacity();

	center.x = centerX;
	center.y = centerY;
}

bool FloatingObject::ResetPosition() {
	for (Coord* vertex : vertices) {
		float x = vertex->x;
		float y = vertex->y;
		if (x > RIGHT_EDGE) {
			center.x = 10.0f + radius;
			//center.y = 590.0f - center.y;
			return true;
		}

		if (x < 10.0f) {
			center.x = RIGHT_EDGE - radius;
			//center.y = 590.0f - center.y;
			return true;
		}

		if (y < 10.0f) {
			center.y = 590.0f - radius;
			//center.x = RIGHT_EDGE - center.x;
			return true;
		}

		if (y > 590.0f) {
			center.y = 10.0f + radius;
			//center.x = RIGHT_EDGE - center.x;
			return true;
		}
	}
	return false;
}

// TO-DO add second collision test
bool FloatingObject::CircleCollision(float otherX, float otherY, float otherRadius) {
	float circleDistance = (center.x - otherX) * (center.x - otherX) + (center.y - otherY) * (center.y - otherY);
	float radiusSumSq = (radius + otherRadius) * (radius + otherRadius);
	if (circleDistance < radiusSumSq) {
		return true;
	}
	return false;
}

void FloatingObject::InitializeD2D(ID2D1HwndRenderTarget* renderTarget) {
	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Lime),
		&limeBrush
	);
}

void FloatingObject::SetDirection() {
	direction.x = cos(radians) * speed;
	direction.y = sin(radians) * speed;
}


void FloatingObject::Update() {
	center.x -= direction.x;
	center.y -= direction.y;
}