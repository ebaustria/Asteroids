#include "Asteroid.h"
#include "FloatingObject.h"
#include <d2d1.h>
#include <chrono>
#include <random>

Asteroid::Asteroid(float r, float centerX, float centerY) {
	std::mt19937 random_engine(rng());
	std::uniform_int_distribution<int> dist_angle(0, 360);

	angle = dist_angle(random_engine);
	radius = r;

	center.x = centerX;
	center.y = centerY;
	
	vertexAngle1 = degreeRad * 91.0f;
	vertexAngle2 = degreeRad * 47.0f;
	vertexAngle3 = degreeRad * 62.0f;
	vertexAngle4 = degreeRad * 79.0f;
	vertexAngle5 = degreeRad * 81.0f;

	vertAngles.push_back(vertexAngle1);
	vertAngles.push_back(vertexAngle2);
	vertAngles.push_back(vertexAngle3);
	vertAngles.push_back(vertexAngle4);
	vertAngles.push_back(vertexAngle5);
	
	vertices.push_back(&v1);
	vertices.push_back(&v2);
	vertices.push_back(&v3);
	vertices.push_back(&v4);
	vertices.push_back(&v5);
	

	radians = CalculateRadians();
	SetDirection();
}

Asteroid::~Asteroid() {
	limeBrush->Release();
}

void Asteroid::DrawAsteroid(ID2D1HwndRenderTarget* renderTarget) {
	float vertexAngle = (PI * 2) / 5;

	float rads = CalculateRadians();

	D2D1_POINT_2F p1 = D2D1::Point2F(
		center.x - radius * cos((float) vertexAngle1 * 0 + rads),
		center.y - radius * sin((float) vertexAngle1 * 0 + rads)
	);

	v1.x = p1.x;
	v1.y = p1.y;

	D2D1_POINT_2F p2 = D2D1::Point2F(
		center.x - radius * cos((float) vertexAngle2 * 1 + rads),
		center.y - radius * sin((float) vertexAngle2 * 1 + rads)
	);

	v2.x = p2.x;
	v2.y = p2.y;

	D2D1_POINT_2F p3 = D2D1::Point2F(
		center.x - radius * cos((float) vertexAngle3 * 2 + rads),
		center.y - radius * sin((float) vertexAngle3 * 2 + rads)
	);

	v3.x = p3.x;
	v3.y = p3.y;

	D2D1_POINT_2F p4 = D2D1::Point2F(
		center.x - radius * cosf((float) vertexAngle4 * 3 + rads),
		center.y - radius * sin((float) vertexAngle4 * 3 + rads)
	);

	v4.x = p4.x;
	v4.y = p4.y;

	D2D1_POINT_2F p5 = D2D1::Point2F(
		center.x - radius * cos((float) vertexAngle5 * 4 + rads),
		center.y - radius * sin((float) vertexAngle5 * 4 + rads)
	);

	v5.x = p5.x;
	v5.y = p5.y;

	if (ResetPosition()) {
		return;
	}

	renderTarget->DrawLine(
		p1,
		p2,
		limeBrush
	);

	renderTarget->DrawLine(
		p2,
		p3,
		limeBrush
	);

	renderTarget->DrawLine(
		p3,
		p4,
		limeBrush
	);

	renderTarget->DrawLine(
		p4,
		p5,
		limeBrush
	);

	renderTarget->DrawLine(
		p1,
		p5,
		limeBrush
	);
}

Coord Asteroid::GetCenter() {
	return center;
}

float Asteroid::GetRadius() {
     	return radius;
}