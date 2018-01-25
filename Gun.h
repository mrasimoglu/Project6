#pragma once

#include <time.h>

#include <d3d11.h>
#include "SimpleMath.h"
#include "Map.h"
#include "Survivor.h"
#include "Camera.h"

#include "DXApp.h"

using namespace DirectX::SimpleMath;

class Survivor;
extern enum Guns;

class Gun
{
private:
	Survivor* owner;

	Guns type;

	int maxAmmo;
	int backup;
	int currentAmmo;
	
	int delay;
	int rebound;

	bool singleReload;
	
	clock_t time;

	Vector2 offset;
public:
	bool Shoot(int, Map*, Vector2);
	bool Reload();
	void Fire(Map*, Vector2);

	int GetBackup();
	int GetCurrentAmmo();
	int GetRebound();

	void SetRebound(int);

	void Update();

	Vector2 CalculateRelativePosition();
	float CalculateRelativeRotation(Vector2);

	Gun(Survivor*, int, int, int, Vector2, Guns);
	Gun(Survivor*, int, int, int, Vector2, bool, Guns);
	~Gun();
};

