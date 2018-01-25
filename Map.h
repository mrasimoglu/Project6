#pragma once

#include <d3d11.h>
#include "Mouse.h"

#include "FiniteObject.h"
#include "StaticObject.h"
#include "Camera.h"

#include "DXUtil.h"

#define MAX_STATIC 100
#define MAX_BULLET 100
#define MAX_FINITE 100
#define MAX_NOCOLLISION 100
#define MAX_GROUNDSTATICOBJECTS 100
#define MAX_GROUNDFINITEOBJECTS 100

class Map
{
private:
	Vector2 size;

	FiniteObject *bullets[MAX_BULLET];
	FiniteObject *finites[MAX_FINITE];
	StaticObject *statics[MAX_STATIC];
	StaticObject *groundStaticObjects[MAX_GROUNDSTATICOBJECTS];
	FiniteObject *groundFiniteObjects[MAX_GROUNDFINITEOBJECTS];
	FiniteObject *nocollisions[MAX_NOCOLLISION];

	ID3D11Device *device;
public:
	void CreateBullet(Vector2 pos, float rot, ID3D11Device* device);
	void CreateMuzzle(Vector2 pos, float rot, ID3D11Device* device);
	void CreateBox(Vector2 pos, float rot, ID3D11Device* device);
	void CreateBarrel(Vector2 pos, float rot, ID3D11Device* device);
	void CreateAmmoClipEmpty(Vector2 pos, float rot, ID3D11Device* device);
	void CreateAmmoClipFull(Vector2 pos, float rot, ID3D11Device* device);

	void DrawMap(SpriteBatch*, SpriteFont*, Camera*);
	void UpdateMap(DirectX::Mouse::State);

	void InitMap(ID3D11Device*);

	StaticObject** GetStaticObjects();
	int GetMaxStatic();

	StaticObject** GetGroundObjects();
	int GetMaxGroundObject();

	Vector2 GetSize();

	Map(Vector2);
	~Map();
};

