#pragma once

#include "Entity.h"

#include <time.h>

typedef enum BodyAction { idle, move, reload, shoot, meleeattack };
typedef enum FeetAction { idleF, walkF, runF, leftF, rightF};
typedef enum Direction { forward, right, backward, left };

class Character : public Entity
{
protected:
	int health;
	float speed;
	BodyAction bodyAction;
	FeetAction feetAction;

	Vector2 relativePosition;

	int bodyFrame;
	int feetFrame;
	void IncFrame();
	clock_t time;

	void Walk(Direction);
	void Run();
public:
	int GetHealth();

	Character();
	~Character();
};

