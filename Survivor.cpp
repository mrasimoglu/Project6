#include "Survivor.h"

#include <iostream>
#define PI 3.14159265

Survivor::Survivor(ID3D11Device* device, Camera* camera)
{
	numOfSprites = 2;

	this->camera = camera;

	this->device = device;

	guns[flashlight] = new Gun(this, 0, 0, 0, Vector2(0,0), flashlight);
	guns[knife] = new Gun(this, 0, 0, 0, Vector2(0, 0), knife);
	guns[pistol] = new Gun(this, 12, 36, 500, Vector2(87, 20), pistol);
	guns[shotgun] = new Gun(this, 7, 21, 1000, Vector2(115, 16), true, shotgun);
	guns[rifle] = new Gun(this, 30, 90, 100, Vector2(115, 16), rifle);

	speed = 0.00000005f;

	feetAction = idleF;
	bodyAction = idle;

	gun = knife;

	anim = new CharacterAnimations(device);

	for (int i = 0; i < numOfSprites; i++)
		sprite[i] = new SpriteSheet();

	sprite[0] = anim->feet[idleF];
	sprite[1] = anim->idle[knife];

	ver.numOfVertex = 6;
	ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);
}

void Survivor::Update(DirectX::Mouse::State state, DirectX::Keyboard::State kb, Map* map)
{
	int tempX = state.x - position.x + camera->GetOffset().x;
	int tempY = position.y - state.y - camera->GetOffset().y;
	

	if ((tempX > 30 || tempX < -30) || (tempY > 30 || tempY < -30))
		SetRotation((atan2(tempX, tempY)) - (PI / 2));

	//Mouse Inputs
	if (state.leftButton && bodyAction != reload && guns[gun]->Shoot(bodyFrame, map, {(float)state.x, (float)state.y}))
	{
		bodyAction = shoot;
		sprite[1] = anim->shoot[gun - 2]; 
		bodyFrame = 0;
	}
	if (state.rightButton && bodyAction != reload && bodyAction != meleeattack && bodyAction != shoot)
	{
		bodyAction = meleeattack;
		sprite[1] = anim->meleeattack[gun];
		bodyFrame = 0;
	}

	if (state.scrollWheelValue < scrollStore)
	{
		gun = static_cast<Guns>(((int)gun + 1) % MAX_WEAPON);
	}
	else if (state.scrollWheelValue > scrollStore)
	{
		gun = static_cast<Guns>((((gun - 1) % MAX_WEAPON) + MAX_WEAPON) % MAX_WEAPON);
	}

	scrollStore = state.scrollWheelValue;
	
	//Keyboard Inputs
	if (!kb.LeftShift || bodyAction == reload)
	{
		if (kb.A)
		{
			Walk(left);
			feetAction = leftF;
			if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
			{
				sprite[1] = anim->move[gun];
				bodyAction = move;
			}
			
		}
		if (kb.D)
		{
			Walk(right);
			feetAction = rightF;
			if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
			{
				bodyAction = move;
				sprite[1] = anim->move[gun];
			}
		}
		else;
		if (kb.W)
		{
			feetAction = walkF;
			Walk(forward);
			if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
			{
				bodyAction = move;
				sprite[1] = anim->move[gun];
			}
		}
		else if (kb.S)
		{
			feetAction = walkF;
			Walk(backward);
			if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
			{
				sprite[1] = anim->move[gun];
				bodyAction = move;
			}
		}
	}
	else if (bodyAction != reload)
	{
		if (kb.W)
		{
			feetAction = runF;
			Run();
			if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
			{
				bodyAction = move;
				sprite[1] = anim->move[gun];
			}
		}
	}
	if (!kb.A && !kb.D & !kb.W && !kb.S)
	{
		if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
		{
			sprite[1] = anim->idle[gun];
			bodyAction = idle;
		}
		feetAction = idleF;
	}

	if (kb.D1)
		gun = flashlight;
	if (kb.D2)
		gun = knife;
	if (kb.D3)
		gun = pistol;
	if (kb.D4)
		gun = shotgun;
	if (kb.D5)
		gun = rifle;
	
	if (kb.R)
	{
		if (bodyAction != shoot && bodyAction != reload && bodyAction != meleeattack && guns[gun]->Reload())
		{
			sprite[1] = anim->reload[gun - 2];
			bodyFrame = 0;
			bodyAction = reload;
		}
	}

	if (bodyAction == reload || bodyAction == shoot || bodyAction == meleeattack)
	{
		if (bodyFrame > (sprite[1]->GetXY().x * sprite[1]->GetXY().y))
		{
			if(gun != shotgun && bodyAction == reload)
				map->CreateAmmoClipEmpty((guns[gun]->CalculateRelativePosition() + position) / Vector2(2,2), rand(), device);

			if (gun == shotgun && !state.leftButton && !state.rightButton && !kb.LeftShift && guns[shotgun]->Reload());
				
			else
				bodyAction = FeetActionToBodyAction(feetAction);

			bodyFrame = 0;
		}
	}

	sprite[0] = anim->feet[feetAction];

	for (int i = 0; i < numOfSprites; i++)
	{
		sprite[i]->SetPosition(Vector2(position));
		sprite[i]->SetRotation(rotation);
	}

	//
	IncFrame();
	
	sprite[0]->CalculateSourceRect(feetFrame);
	sprite[1]->CalculateSourceRect(bodyFrame);
		
	//
	guns[gun]->Update();

	if (feetAction == walkF)
		guns[gun]->SetRebound(250);
	else if (feetAction != idleF)
		guns[gun]->SetRebound(500);

	//COLLISION
	this->CalculateVertex();

	StaticObject** staticObjects = map->GetStaticObjects();
	for (int i = 0; i < map->GetMaxStatic(); i++)
	{
		if (staticObjects[i] != nullptr)
		{
			Vector2 normal = Collision::CheckCollisionT(this, staticObjects[i]);
			this->SetPosition(this->GetPosition() + (normal));
		}
	}

	StaticObject** groundObjects = map->GetGroundObjects();
	memset(closestGroundObjects, 0,sizeof(StaticObject*)*(MAX_GROUNDSTATICOBJECTS / 3));
	int counter = 0;
	for (int i = 0; i < map->GetMaxStatic(); i++)
	{
		if (groundObjects[i] != nullptr)
		{
			if (Collision::CheckDistance(this, groundObjects[i], Collision::Magnitude({ (float)GetSprite()->GetWidth() / 2, (float)GetSprite()->GetHeight() / 2 }) + 1))
			{
				closestGroundObjects[counter] = groundObjects[i];
				counter++;
			}
			else;
		}
	}
}

Guns Survivor::GetOnHand() { return gun; }

int Survivor::GetAmmoBackup()
{
	if (gun == pistol)
		return guns[2]->GetBackup();
	else if (gun == shotgun)
		return guns[3]->GetBackup();
	else if (gun == rifle)
		return guns[4]->GetBackup();
	else
		return NULL;
}

Camera * Survivor::GetCamera()
{
	return camera;
}

StaticObject ** Survivor::GetClosestGroundObjects()
{
	return closestGroundObjects;
}

int Survivor::GetAmmoCurrent()
{
	if (gun == pistol)
		return guns[2]->GetCurrentAmmo();
	else if (gun == shotgun)
		return guns[3]->GetCurrentAmmo();
	else if (gun == rifle)
		return guns[4]->GetCurrentAmmo();
	else
		return NULL;
}

Survivor::~Survivor()
{
}

BodyAction Survivor::FeetActionToBodyAction(FeetAction feetAction)
{
	if (feetAction == idleF)
		return idle;
	else if (feetAction == runF || feetAction == walkF)
		return move;
}

void Survivor::CalculateVertex()
{
	if (gun == flashlight)
	{
		ver.vertex[0] = this->GetPosition() + Vector2(cos(rotation) * 105, sin(rotation) * 105) + Vector2(-sin(rotation) * 5, cos(rotation) * 5);	//ön
		ver.vertex[1] = this->GetPosition() + Vector2(cos(rotation) * 105, sin(rotation) * 105) + Vector2(sin(rotation) * 5, -cos(rotation) * 5);	//ön
		ver.vertex[2] = this->GetPosition() + Vector2(sin(rotation) * 40, -cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sol
		ver.vertex[3] = this->GetPosition() + Vector2(-sin(rotation) * 40, cos(rotation) * 40) + Vector2(cos(rotation) * 10, sin(rotation) * 10);	//sað
		ver.vertex[4] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(sin(rotation) * 35, -cos(rotation) * 35); //arka
		ver.vertex[5] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(-sin(rotation) * 35, cos(rotation) * 35); //arka
	}
	if (gun == knife)
	{
		ver.vertex[0] = this->GetPosition() + Vector2(cos(rotation) * 60, sin(rotation) * 60) + Vector2(sin(rotation) * 15, -cos(rotation) * 15);				//ön
		ver.vertex[1] = this->GetPosition() + Vector2(sin(rotation) * 45, -cos(rotation) * 45) + Vector2(cos(rotation) * 10, sin(rotation) * 10);				//sol
		ver.vertex[2] = this->GetPosition() + Vector2(sin(rotation) * 40, -cos(rotation) * 40) + Vector2(cos(rotation) * (-15), sin(rotation) * (-15));			//sol
		ver.vertex[3] = this->GetPosition() + Vector2(-cos(rotation) * 35, -sin(rotation) * 35);																//arka
		ver.vertex[4] = this->GetPosition() + Vector2(-sin(rotation) * 35, cos(rotation) * 35) + Vector2(-cos(rotation) * 20, -sin(rotation) * 20);				//sað
		ver.vertex[5] = this->GetPosition() + Vector2(-sin(rotation) * 35, cos(rotation) * 35) + Vector2(cos(rotation) * 60, sin(rotation) * 60);				//sað
	}
	else if (gun == pistol)
	{
		ver.vertex[0] = this->GetPosition() + Vector2(cos(rotation) * 50, sin(rotation) * 50) + Vector2(-sin(rotation) * 25, cos(rotation) * 25);	//ön
		ver.vertex[1] = this->GetPosition() + Vector2(cos(rotation) * 50, sin(rotation) * 50) + Vector2(sin(rotation) * 20, -cos(rotation) * 20);	//ön
		ver.vertex[2] = this->GetPosition() + Vector2(sin(rotation) * 40, -cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sol
		ver.vertex[3] = this->GetPosition() + Vector2(-sin(rotation) * 40, cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sað
		ver.vertex[4] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(sin(rotation) * 35, -cos(rotation) * 35); //arka
		ver.vertex[5] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(-sin(rotation) * 35, cos(rotation) * 35); //arka
	}
	else if (gun == shotgun)
	{
		ver.vertex[0] = this->GetPosition() + Vector2(cos(rotation) * 65, sin(rotation) * 65) + Vector2(-sin(rotation) * 15, cos(rotation) * 15);	//ön
		ver.vertex[1] = this->GetPosition() + Vector2(cos(rotation) * 55, sin(rotation) * 55) + Vector2(sin(rotation) * 15, -cos(rotation) * 15);	//ön
		ver.vertex[2] = this->GetPosition() + Vector2(sin(rotation) * 40, -cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sol
		ver.vertex[3] = this->GetPosition() + Vector2(-sin(rotation) * 40, cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sað
		ver.vertex[4] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(sin(rotation) * 35, -cos(rotation) * 35); //arka
		ver.vertex[5] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(-sin(rotation) * 35, cos(rotation) * 35); //arka
	}
	else if (gun == rifle)
	{
		ver.vertex[0] = this->GetPosition() + Vector2(cos(rotation) * 65, sin(rotation) * 65) + Vector2(-sin(rotation) * 15, cos(rotation) * 15);	//ön
		ver.vertex[1] = this->GetPosition() + Vector2(cos(rotation) * 55, sin(rotation) * 55) + Vector2(sin(rotation) * 15, -cos(rotation) * 15);	//ön
		ver.vertex[2] = this->GetPosition() + Vector2(sin(rotation) * 40, -cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sol
		ver.vertex[3] = this->GetPosition() + Vector2(-sin(rotation) * 40, cos(rotation) * 40) + Vector2(cos(rotation) * 15, sin(rotation) * 15);	//sað
		ver.vertex[4] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(sin(rotation) * 35, -cos(rotation) * 35); //arka
		ver.vertex[5] = this->GetPosition() + Vector2(-cos(rotation) * 20, -sin(rotation) * 20) + Vector2(-sin(rotation) * 35, cos(rotation) * 35); //arka
	}
}

float Survivor::CalculateRotation(DirectX::Mouse::State state, float rot, Vector2 position/*, Camera* cam*/)
{
	int tempX = state.x - position.x /*- cam.x*/;
	int tempY = position.y - state.y /*+ cam.y*/;

	if ((tempX > 120 || tempX < -120) || (tempY > 120 || tempY < -120))
		return ((atan2(tempX, tempY)) - (PI / 2));
	else
		return rot;
}