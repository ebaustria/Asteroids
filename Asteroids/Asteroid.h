#pragma once

#include "Coord.h"
#include "FloatingObject.h"
#include <d2d1.h>
#include <random>

class Asteroid : public FloatingObject {
public:
	Asteroid(float r, float centerX, float centerY);
	~Asteroid();

	void DrawAsteroid(ID2D1HwndRenderTarget* renderTarget);

	Coord GetCenter();
	
	float GetRadius();

private:
	float degreeRad = 0.0174533;

	std::random_device rng;

	float vertexAngle1;
	float vertexAngle2;
	float vertexAngle3;
	float vertexAngle4;
	float vertexAngle5;

	Coord v1;
	Coord v2;
	Coord v3;
	Coord v4;
	Coord v5;

	std::vector<float> vertAngles;
};