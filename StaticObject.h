#pragma once

#include "Entity.h"

typedef enum Shape {circle, rectangle};

class StaticObject : public Entity
{
private:
	Shape shape;
public:
	StaticObject(ID3D11Device* device, wchar_t *path, Vector2 pos, float rot, Shape);
	~StaticObject();

	void CalculateVertex();
};