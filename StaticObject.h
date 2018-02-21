#ifndef STATICOBJECT_H
#define STATICOBJECT_H
#pragma once

#include "Entity.h"
#include "DXApp.h"

class StaticObject : public Entity
{
protected:
public:
	StaticObject();
	StaticObject(wchar_t *path, Vector2 pos, float rot, Shape, Camera*);
	~StaticObject();

	void CalculateVertex();
};

#endif