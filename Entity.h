#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include "SpriteSheet.h"

#include "Camera.h"

using namespace DirectX::SimpleMath;

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

	Vertexs ver;

public:
	Entity();
	~Entity();

	void Draw(SpriteBatch*, Camera*);

	//SETTERS
	void SetPosition(Vector2);
	void SetRotation(float);

	//GETTERS
	Vector2 GetPosition();
	float GetRotation();
	float GetRadius();
	int GetWidth();
	int GetHeight();
	SpriteSheet* GetSprite();
	Vertexs *GetVertexs();
};

