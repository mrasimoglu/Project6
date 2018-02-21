#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include "SpriteSheet.h"

#include "Camera.h"

using namespace DirectX::SimpleMath;

enum Shape { circle, rectangle, ghost };

typedef struct
{
	Vector2 *vertex;
	int numOfVertex;
}Vertexs;

class Entity
{
protected:
	Vector2 position;
	float rotation;
	SpriteSheet *(*sprite);
	int numOfSprites;
	Shape shape;

	Vertexs ver;

	Camera* camera;

public:
	Entity();
	~Entity();

	void Draw(SpriteBatch*);

	//SETTERS
	void SetPosition(Vector2);
	void SetRotation(float);
	void SetCamera(Camera*);

	//GETTERS
	Vector2 GetPosition();
	float GetRotation();
	float GetRadius();
	int GetWidth();
	int GetHeight();
	SpriteSheet* GetSprite();
	Vertexs *GetVertexs();
};

#endif