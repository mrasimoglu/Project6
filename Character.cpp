#include "Character.h"



#include "GameLoop.h"
#include <chrono>

void Character::Walk(Direction direction)
{
	float speed = this->speed * GameLoop::deltaTime.count();

	if (direction == forward)
		position += { cos(rotation) * speed, sin(rotation) * speed };
	else if (direction == right)
		position += { -sin(rotation) * speed, cos(rotation) * speed };
	else if (direction == backward)
		position += { -cos(rotation) * speed, -sin(rotation) * speed };
	else if (direction == left)
		position += { sin(rotation) * speed, -cos(rotation) * speed };
	else;
}

void Character::Run()
{
	float speed = this->speed * GameLoop::deltaTime.count();

	position += { cos(rotation) * speed * 2.0f, sin(rotation) * speed * 2.0f };
}

void Character::IncFrame()
{
	if (bodyFrame > 60)
		bodyFrame = 0; 
	if (feetFrame > 60)
		feetFrame = 0;

	if (clock() - time > 50)
	{
		bodyFrame++;
		feetFrame++;
		time = clock();
	}
}

int Character::GetHealth() { return health; }

Character::Character()
{
	time = clock();
	health = 100;
}


Character::~Character()
{
}
