#include "Gun.h"

#include <iostream>

#define PI 3.14159265

bool Gun::Shoot(int counter, Map* map, Vector2 state)
{
	if (currentAmmo > 0 && time + delay < clock())
	{
		currentAmmo--;
		time = clock();

		Fire(map, state);

		return true;
	}
	else
		return false;
}

void Gun::Fire(Map* map, Vector2 state)
{
	Vector2 tempPos = CalculateRelativePosition();
	map->CreateMuzzle(tempPos, owner->GetRotation(), DXApp::m_pDevice);
	switch (type)
	{
		case(shotgun):
		{
			SetRebound(250);
			for (int i = 0; i < 5; i++)
			{
				map->CreateBullet(tempPos, CalculateRelativeRotation({ state.x,state.y }), DXApp::m_pDevice);
			}
			return;
		}break;
		case (pistol):
		{
			SetRebound(100);
		}break;
		case (rifle):
		{
			SetRebound(150);
		}break;	
	}

	map->CreateBullet(tempPos, CalculateRelativeRotation({ state.x,state.y }), DXApp::m_pDevice);
	
}

bool Gun::Reload()
{
	if (!singleReload)
	{
		if (backup > 0 && currentAmmo < maxAmmo)
		{
			if (maxAmmo - currentAmmo <= backup)
			{
				backup -= maxAmmo - currentAmmo;
				currentAmmo = maxAmmo;
			}
			else if (maxAmmo - currentAmmo > backup)
			{
				currentAmmo += backup;
				backup = 0;
			}
			return true;
		}
		else
			return false;
	}
	else
	{
		if (backup > 0 && currentAmmo < maxAmmo)
		{
			currentAmmo++;
			backup--;
			return true;
		}
		else
			return false;
	}
}

Gun::Gun(Survivor* owner, int maximumAmmo, int backup, int delay, Vector2 offset, Guns type)
{
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

Gun::Gun(Survivor* owner, int maximumAmmo, int backup, int delay, Vector2 offset, bool singleReload, Guns type)
{
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
int Gun::GetCurrentAmmo() { return currentAmmo; }

void Gun::SetRebound(int rebound) { this->rebound = rebound; }

void Gun::Update()
{
	if(rebound > 0)
		rebound--;
}

Vector2 Gun::CalculateRelativePosition()
{
	float rotation = owner->GetRotation();
	Vector2 position = owner->GetPosition();
	return Vector2(position.x + cos(rotation) * offset.x - sin(rotation) * offset.y, position.y + sin(rotation) * offset.x + cos(rotation) * offset.y);
}

float Gun::CalculateRelativeRotation(Vector2 state)
{
	float rotation = owner->GetRotation();
	Vector2 position = owner->GetPosition();

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
