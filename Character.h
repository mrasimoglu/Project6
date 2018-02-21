#pragma once

#include "Entity.h"

#include <time.h>

enum Direction { forward, right, backward, left };

class Character : public Entity
{
protected:
	int health;
	float speed;

	Vector2 relativePosition;
	clock_t time;

	void Walk(Direction);
	void Run();
public:
	int GetHealth();

	Character();
	~Character();
};

