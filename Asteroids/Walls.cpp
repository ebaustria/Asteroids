#include "Walls.h"
#include "Engine.h"

Walls::Walls() : limeBrush(NULL) {
}

Walls::~Walls() {
	limeBrush->Release();
}

void Walls::InitializeD2D(ID2D1HwndRenderTarget* renderTarget) {
	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Lime),
		&limeBrush
	);
}

void Walls::DrawWalls(ID2D1HwndRenderTarget* renderTarget) {
	float buffer = 10.0f;
	float floorBuffer = RESOLUTION_Y - 10;

	D2D1_POINT_2F topLeft = D2D1::Point2F(buffer, buffer);
	D2D1_POINT_2F bottomLeft = D2D1::Point2F(buffer, floorBuffer);
	D2D1_POINT_2F topRight = D2D1::Point2F(RIGHT_EDGE, buffer);
	D2D1_POINT_2F bottomRight = D2D1::Point2F(RIGHT_EDGE, floorBuffer);

	renderTarget->DrawLine(
		topLeft,
		bottomLeft,
		limeBrush
		);

	renderTarget->DrawLine(
		topLeft,
		topRight,
		limeBrush
	);

	renderTarget->DrawLine(
		topRight,
		bottomRight,
		limeBrush
	);

	renderTarget->DrawLine(
		bottomLeft,
		bottomRight,
		limeBrush
	);
}