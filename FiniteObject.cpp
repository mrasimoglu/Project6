#include "FiniteObject.h"

#include <iostream>

#include "GameLoop.h"
#include <chrono>

bool FiniteObject::Update()
{
	if (time + lifetime < clock())
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
	ver.vertex[0] = Collision::CalculateRectangleCorner(this, 0);
	ver.vertex[1] = Collision::CalculateRectangleCorner(this, 1);
	ver.vertex[2] = Collision::CalculateRectangleCorner(this, 2);
	ver.vertex[3] = Collision::CalculateRectangleCorner(this, 3);
}


FiniteObject::FiniteObject(ID3D11Device* device, wchar_t *path, Vector2 pos, float rot, int lifetime, float speed)
{
	time = clock();
	this->lifetime = lifetime;
	this->speed = speed * 0.000001f;

	numOfSprites = 1;

	sprite[0] = new SpriteSheet();
	sprite[0]->Load(device, path);

	sprite[0]->SetPosition(pos);
	sprite[0]->SetRotation(rot);

	sprite[0]->SetScale({ 0.2f, 0.2f });

	ver.numOfVertex = 4;
	ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);
}


FiniteObject::~FiniteObject()
{

}
