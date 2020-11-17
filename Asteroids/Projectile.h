#pragma once

#include "Coord.h"
#include "FloatingObject.h"

class Projectile : public FloatingObject {
public:
	Projectile(float x, float y, float ang);
	~Projectile();

	void DrawProjectile(ID2D1HwndRenderTarget* renderTarget);

	Coord GetCenter();
private:
};