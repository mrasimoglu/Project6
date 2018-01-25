#include "StaticObject.h"

#include "Mustafa.h"

StaticObject::StaticObject(ID3D11Device* device, wchar_t *path, Vector2 pos, float rot, Shape shape)
{
	numOfSprites = 1;
	sprite[0] = new SpriteSheet(pos);
	sprite[0]->Load(device, path);
	sprite[0]->SetRotation(rot);
	sprite[0]->SetScale(Vector2(0.25f, 0.25f));

	this->shape = shape;

	if (shape == rectangle)
	{
		ver.numOfVertex = 4;
		ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

		ver.vertex[0] = Collision::CalculateRectangleCorner(this, 0);
		ver.vertex[1] = Collision::CalculateRectangleCorner(this, 1);
		ver.vertex[2] = Collision::CalculateRectangleCorner(this, 2);
		ver.vertex[3] = Collision::CalculateRectangleCorner(this, 3);
	}
	else if (shape == circle)
	{
		ver.numOfVertex = 8;
		ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

		Collision::CalculateCircleVertex(this, &ver);
	}
}


StaticObject::~StaticObject()
{

}

void StaticObject::CalculateVertex()
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
