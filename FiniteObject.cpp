#include "FiniteObject.h"

#include <iostream>

#include "GameLoop.h"
#include <chrono>

FiniteObject::FiniteObject(wchar_t *path, Vector2 pos, float rot, int lifetime, float speed, Camera* cam, Shape shape, int ammoID)
{
	this->ammoID = ammoID;

	if (ammoID == 9)
		damage = 10;
	else if (ammoID == 10)
		damage = 35;
	else if (ammoID == 762)
		damage = 25;
	else
		damage = 0;

	this->camera = cam;

	this->shape = shape;

	time = clock();
	this->lifetime = lifetime;
	this->speed = speed * 0.000001f;

	numOfSprites = 1;

	sprite[0] = new SpriteSheet();
	sprite[0]->Load(DXApp::m_pDevice, path);

	sprite[0]->SetPosition(pos);
	sprite[0]->SetRotation(rot);

	sprite[0]->SetScale({ 0.2f, 0.2f });

	if (this->shape == ghost)
	{
		ver.numOfVertex = 0;
	}
	else if (this->shape == rectangle)
	{
		ver.numOfVertex = 4;
		ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

		ver.vertex[0] = Collision::CalculateRectangleCorner(this, 0);
		ver.vertex[1] = Collision::CalculateRectangleCorner(this, 1);
		ver.vertex[2] = Collision::CalculateRectangleCorner(this, 2);
		ver.vertex[3] = Collision::CalculateRectangleCorner(this, 3);
	}
	else if (this->shape == circle)
	{
		ver.numOfVertex = 8;
		ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

		Collision::CalculateCircleVertex(this, &ver);
	}

	counter = 0;
}

FiniteObject::FiniteObject(wchar_t *path, Vector2 pos, float rot, int lifetime, float speed, Camera* cam, Shape shape)
{
	this->camera = cam;

	this->shape = shape;

	damage = 0;

	time = clock();
	this->lifetime = lifetime;
	this->speed = speed * 0.000001f;

	numOfSprites = 1;

	sprite[0] = new SpriteSheet();
	sprite[0]->Load(DXApp::m_pDevice, path);

	sprite[0]->SetPosition(pos);
	sprite[0]->SetRotation(rot);

	sprite[0]->SetScale({ 0.2f, 0.2f });

	if (this->shape == ghost)
	{
		ver.numOfVertex = 0;
	}
	else if (this->shape == rectangle)
	{
		ver.numOfVertex = 4;
		ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

		ver.vertex[0] = Collision::CalculateRectangleCorner(this, 0);
		ver.vertex[1] = Collision::CalculateRectangleCorner(this, 1);
		ver.vertex[2] = Collision::CalculateRectangleCorner(this, 2);
		ver.vertex[3] = Collision::CalculateRectangleCorner(this, 3);
	}
	else if (this->shape == circle)
	{
		ver.numOfVertex = 8;
		ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

		Collision::CalculateCircleVertex(this, &ver);
	}
}

bool FiniteObject::Update(Map* map)
{
	if (ammoID == 10)
		if (time + (counter * 25) < clock())
			if (damage > 5)
			{
				damage--;
				counter++;
			}

	position = sprite[0]->GetPosition();
	if ((time + lifetime < clock()) || (position.x < 0 || position.x > map->GetSize().x || position.y < 0 || position.y > map->GetSize().y))
		return 1;
	else
	{
		float speed = this->speed * GameLoop::deltaTime.count();
		sprite[0]->SetPosition({ sprite[0]->GetPosition().x + cos(sprite[0]->GetRotation()) * speed, sprite[0]->GetPosition().y + sin(sprite[0]->GetRotation()) * speed });
		return 0;
	}
}

void FiniteObject::CalculateVertex()
{
	if (shape == rectangle)
	{
		ver.vertex[0] = Collision::CalculateRectangleCorner(this, 0);
		ver.vertex[1] = Collision::CalculateRectangleCorner(this, 1);
		ver.vertex[2] = Collision::CalculateRectangleCorner(this, 2);
		ver.vertex[3] = Collision::CalculateRectangleCorner(this, 3);
	}
	else if (shape == circle)
	{
		Collision::CalculateCircleVertex(this, &ver);
	}
	
}

float FiniteObject::GetDamage()
{
	return damage;
}


FiniteObject::~FiniteObject()
{

}
