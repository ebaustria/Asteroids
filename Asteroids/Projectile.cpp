#include "Projectile.h"
#include <d2d1.h>
#include "Framework.h"

Projectile::Projectile(float x, float y, float ang) {
	center.x = x;
	center.y = y;
	angle = ang;
	speed = 1.0f;

	radians = CalculateRadians();
	SetDirection();
}

Projectile::~Projectile() {
	limeBrush->Release();
}

void Projectile::DrawProjectile(ID2D1HwndRenderTarget* renderTarget) {
	D2D1_ELLIPSE proj = D2D1::Ellipse(
		D2D1::Point2F(center.x, center.y),
		1.0f,
		1.0f
	);

	renderTarget->DrawEllipse(proj, limeBrush);
}

Coord Projectile::GetCenter() {
	return center;
}

/*
void Projectile::Update() {

}
*/