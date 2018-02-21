#include "Gun.h"

#include <iostream>

#define PI 3.14159265

bool Gun::Shoot(int counter, Map* map, Vector2 state, Camera* camera)
{
	if (currentAmmo > 0 && time + delay < clock())
	{
		currentAmmo--;
		time = clock();

		Fire(map, state, camera);

		return true;
	}
	else
		return false;
}

void Gun::Fire(Map* map, Vector2 state, Camera* camera)
{
	Vector2 tempPos = CalculateRelativePosition();
	map->CreateMuzzle(tempPos, owner->GetRotation(), DXApp::m_pDevice, camera);
	switch (type)
	{
		case(shotgun):
		{
			SetRebound(250);
			for (int i = 0; i < 5; i++)
			{
				map->CreateBullet(tempPos, CalculateRelativeRotation({ state.x,state.y }), DXApp::m_pDevice, camera, AmmoID);
			}
			return;
		}break;
		case (pistol):
		{
			SetRebound(10);
		}break;
		case (rifle):
		{
			SetRebound(15);
		}break;	
	}

	map->CreateBullet(tempPos, CalculateRelativeRotation({ state.x,state.y }), DXApp::m_pDevice, camera, AmmoID);
	
}

int Gun::Reload()
{
	int rv = 0;

	if (!singleReload)
	{
		if (backup > 0 && currentAmmo < maxAmmo)
		{
			if (maxAmmo - currentAmmo <= backup)
			{
				rv = maxAmmo - currentAmmo;
				backup -= maxAmmo - currentAmmo;
				currentAmmo = maxAmmo;
			}
			else if (maxAmmo - currentAmmo > backup)
			{
				rv = backup;
				currentAmmo += backup;
				backup = 0;
			}
			return rv;
		}
		else
			return rv;
	}
	else
	{
		if (backup > 0 && currentAmmo < maxAmmo)
		{
			rv = 1;
			currentAmmo++;
			backup--;
			return rv;
		}
		else
			return rv;
	}
}

bool Gun::CanReload()
{
	if (!singleReload)
	{
		if (backup > 0 && currentAmmo < maxAmmo)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (backup > 0 && currentAmmo < maxAmmo)
		{
			return true;
		}
		else
			return false;
	}
}

Gun::Gun(Survivor* owner, int maximumAmmo, int backup, int delay, Vector2 offset, Guns type, wchar_t* iconPath, wchar_t* spritePath, wchar_t* name, int ID, int numOfItem, int weight, int AmmoID)
{
	this->AmmoID = AmmoID;
	this->name = name;

	handable = true;

	this->infoBox = new InfoBox(name);
	
	numOfSprites = 1;
	sprite = (SpriteSheet**)malloc(sizeof(SpriteSheet*) * numOfSprites);
	sprite[0] = new SpriteSheet();
	sprite[0]->Load(DXApp::m_pDevice, spritePath);

	icon = new SpriteSheet();
	icon->Load(DXApp::m_pDevice, iconPath);

	this->ID = ID;
	this->numOfItem = numOfItem;
	this->weight = weight;

	this->type = type;
	this->owner = owner;
	this->offset = offset;
	currentAmmo = 0;
	this->maxAmmo = maximumAmmo;
	this->backup = backup;
	this->delay = delay;
	this->singleReload = 0;
	rebound = 0;
}

Gun::Gun(Survivor* owner, int maximumAmmo, int backup, int delay, Vector2 offset, Guns type, wchar_t* iconPath, wchar_t* spritePath, wchar_t* name, int ID, int numOfItem, int weight, int AmmoID, bool singleReload)
{
	this->AmmoID = AmmoID;
	this->name = name;

	handable = true;

	this->infoBox = new InfoBox(name);

	numOfSprites = 1;
	sprite = (SpriteSheet**)malloc(sizeof(SpriteSheet*) * numOfSprites);
	sprite[0] = new SpriteSheet();
	sprite[0]->Load(DXApp::m_pDevice, spritePath);

	icon = new SpriteSheet();
	icon->Load(DXApp::m_pDevice, iconPath);

	this->ID = ID;
	this->numOfItem = numOfItem;
	this->weight = weight;

	this->type = type;
	this->owner = owner;
	this->offset = offset;
	currentAmmo = 0;
	this->maxAmmo = maximumAmmo;
	this->backup = backup;
	this->delay = delay;
	this->singleReload = singleReload;
	rebound = 0;
}

Gun::Gun(Survivor* owner, int maximumAmmo, int backup, int delay, Vector2 offset, Guns type, int AmmoID)
{
	this->AmmoID = AmmoID;
	this->type = type;
	this->owner = owner;
	this->offset = offset;
	currentAmmo = 0;
	this->maxAmmo = maximumAmmo;
	this->backup = backup;
	this->delay = delay;
	this->singleReload = 0;
	rebound = 0;
}

Gun::Gun(Survivor* owner, int maximumAmmo, int backup, int delay, Vector2 offset, bool singleReload, Guns type, int AmmoID)
{
	this->AmmoID = AmmoID;
	this->type = type;
	this->owner = owner;
	this->offset = offset;
	currentAmmo = 0;
	this->maxAmmo = maximumAmmo;
	this->backup = backup;
	this->delay = delay;
	this->singleReload = singleReload;
	rebound = 0;
}

int Gun::GetBackup() { return backup; }
int Gun::GetRebound() { return rebound; }
Guns Gun::GetType() { return type; }
int Gun::GetAmmoID() { return AmmoID; }
int Gun::GetCurrentAmmo() { return currentAmmo; }
int Gun::GetMaxAmmo(){ return maxAmmo; }

void Gun::SetRebound(int rebound) { this->rebound = rebound; }
void Gun::SetBackup(int x) { backup = x; }
void Gun::SetOwner(Survivor *s) { this->owner = s; }

void Gun::Update()
{
	if(rebound > 0)
		rebound--;
}

Vector2 Gun::CalculateRelativePosition()
{
	float rotation = owner->GetRotation();
	Vector2 position = owner->GetPosition();

	float sinx = sin(rotation);
	float cosx = cos(rotation);

	return Vector2(position.x + cosx * offset.x - sinx * offset.y, position.y + sinx * offset.x + cosx * offset.y);
}

float Gun::CalculateRelativeRotation(Vector2 state)
{
	float rotation = owner->GetRotation();
	Vector2 position = CalculateRelativePosition();

	int tempX = state.x - position.x + owner->GetCamera()->GetOffset().x;
	int tempY = position.y - state.y - owner->GetCamera()->GetOffset().y;

	if ((tempX > 120 || tempX < -120) || (tempY > 120 || tempY < -120))
		switch (type)
		{
		case shotgun:
			return ((atan2(tempX, tempY)) - (PI / 2)) + ((rand() % 20) - 10) / 100.0f;
		default:
			return ((atan2(tempX, tempY)) - (PI / 2)) + (GetRebound() * ((rand() % 3) - 1) / 62.5f) * PI / 180.0f;
		}
		
	else;
		return rotation + (GetRebound() * ((rand() % 3) - 1) / 62.5f) * PI / 180.0f;
}

Gun::~Gun()
{
}
