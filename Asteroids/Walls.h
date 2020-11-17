#pragma once
#include "Resource.h"
#include <d2d1.h>

//#define WALL_THICKNESS 1.0f
#define RIGHT_EDGE 495.0f
#define LEFT_EDGE 30.0f
#define TOP_EDGE 30.0f

class Walls {
public:
	Walls();
	~Walls();

	void InitializeD2D(ID2D1HwndRenderTarget* renderTarget);
	void DrawWalls(ID2D1HwndRenderTarget* renderTarget);

private:
	ID2D1SolidColorBrush* limeBrush;
};