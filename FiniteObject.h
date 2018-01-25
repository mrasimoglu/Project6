#pragma once

#include "Entity.h"
#include "DXUtil.h"

#include "Mustafa.h"

#include <time.h>

class FiniteObject : public Entity
{
private:
	clock_t time;
	int lifetime;
	float speed;

public:
	bool Update();
	void CalculateVertex();

	FiniteObject(ID3D11Device* device, wchar_t *path, Vector2 pos, float rot, int lifetime, float speed);
	~FiniteObject();
};

