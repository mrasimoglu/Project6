#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include "Mouse.h"

#include "StaticObject.h"
#include "Survivor.h"
#include "FiniteObject.h"
#include "Zombie.h"

#include <complex>

using namespace DirectX::SimpleMath;

class Survivor;
class Zombie;
class FiniteObject;

namespace Collision
{
	Vector2 CalculateRectangleCorner(void*, int);
	void CalculateCircleVertex(void*, Vertexs*);
	
	Vector2 UnitVector(Vector2 v);

	float Distance2D(Vector2 v1, Vector2 v2);
	float Magnitude(Vector2 v);
	
	bool CheckCollision(void*, void*);
	Vector2 CheckCollisionT(void*, void*);
	bool CheckCollision(void*, Vector2 vec, Camera*);

	bool isOver(void * obj, Vector2 pos);

	void DrawVertex(void * obj, Camera * camera);
	bool isOver(DirectX::Mouse::State state, Vector2 pos, Vector2 WidthHeight);
	bool isOver(Camera * cam, Vector2 pos);
	bool CheckDistance(void * obj1, void * obj2, float distance);
	double ProjectVector(Vector2, Vector2);
	bool isOverlap(double, double, double, double);
	bool Trace(void*, StaticObject**, Vector2, Camera*, PrimitiveBatch<VertexPositionColor>* primitiveBatch);
	bool Trace3(void * obj, StaticObject ** statics, Vector2 pos, Camera * cam, PrimitiveBatch<VertexPositionColor>* primitiveBatch);
	bool Traceee(void * obj, StaticObject ** statics, Vector2 pos, Camera * cam, PrimitiveBatch<VertexPositionColor>* primitiveBatch);
	bool Trace2(void*, StaticObject**, Vector2, Camera*);
	bool TraceEfka(void * obj, StaticObject ** statics, Vector2 pos, Camera * cam);
	bool Trace66(void * obj, Vector2 pos, StaticObject **statics, Camera * cam);
	void Shadow(StaticObject**, Vector2, Camera*, Survivor*);
	void Shadow2(StaticObject ** statics, Vector2 pos, float, Camera * cam, Survivor*);
	void DrawRay(PrimitiveBatch<VertexPositionColor>*, StaticObject ** statics, Vector2 pos, Camera * cam);
	int* FindMostDifference(float * arr, int);
	int CalculateMinDistance(void * obj1, void * obj2);

	float DamageDistance(void*, void*, Camera*, PrimitiveBatch<VertexPositionColor>* primitiveBatch);
	float DamageDistance2(void * obj1, void * obj2, Camera * camera);
}