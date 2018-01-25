#pragma once

#include <d3d11.h>
#include "SimpleMath.h"

#include "StaticObject.h"

#include <complex>

namespace Collision
{
	Vector2 CalculateRectangleCorner(void*, int);
	void CalculateCircleVertex(void*, Vertexs*);
	Vector2 CheckCollisionT(void*, void*);
	float Distance2D(Vector2 v1, Vector2 v2);
	float Magnitude(Vector2 v);
	bool CheckCollision(void*, void*);
	bool CheckDistance(void * obj1, void * obj2, float distance);
	double ProjectVector(Vector2, Vector2);
	bool isOverlap(double, double, double, double);
}

