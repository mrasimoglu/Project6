#include "Survivor.h"

#include <iostream>
#define PI 3.14159265

Survivor::Survivor(ID3D11Device* device, Camera* camera)
{
	inventory = new Inventory();

	numOfSprites = 2;

	this->camera = camera;

	this->device = device;

	onHand[flashlight] = newFlashlight;
	onHand[knife] = newKnife;
	onHand[pistol] = newShotgun;
	onHand[flashlight]->SetOwner(this);
	onHand[knife]->SetOwner(this);
	onHand[pistol]->SetOwner(this);
	onHand[pistol]->SetCamera(camera);

	speed = 0.0000001f;

	feetAction = idleF;
	bodyAction = idle;

	gun = knife;

	anim = new CharacterAnimations(device);

	for (int i = 0; i < numOfSprites; i++)
		sprite[i] = new SpriteSheet();

	sprite[0] = anim->feet[idleF];
	sprite[1] = anim->idle[knife];

	sprite[0]->SetPosition(Vector2(300, 100));
	sprite[1]->SetPosition(Vector2(300, 100));

	ver.numOfVertex = 6;
	ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);
}

void Survivor::Update(DirectX::Mouse::State state, DirectX::Keyboard::State kb, Map* map, Zombie* zombies[10])
{
	int tempX = state.x - (int)position.x + (int)camera->GetOffset().x;
	int tempY = (int)position.y - state.y - (int)camera->GetOffset().y;

	if ((tempX > 30 || tempX < -30) || (tempY > 30 || tempY < -30))
		SetRotation((float)((atan2(tempX, tempY)) - (PI / 2)));

	//Mouse Inputs
	if (state.leftButton && bodyAction != reload && bodyAction != meleeattack && ((Gun*)onHand[gun])->Shoot(bodyFrame, map, { (float)state.x, (float)state.y }, camera))
	{
		bodyAction = shoot;
		sprite[1] = anim->shoot[((Gun*)onHand[gun])->GetType() - 2];
		bodyFrame = 0;
	}
	if (state.rightButton && bodyAction != meleeattack && bodyAction != shoot && bodyAction != reload)
	{
		bodyAction = meleeattack;
		sprite[1] = anim->meleeattack[((Gun*)onHand[gun])->GetType()];
		bodyFrame = 0;
	}

	if (state.scrollWheelValue < scrollStore)
	{
		gun = static_cast<Guns>(((int)gun + 1) % MAX_WEAPON);
		while (onHand[gun] == nullptr)
			gun = static_cast<Guns>(((int)gun + 1) % MAX_WEAPON);

		bodyAction = FeetActionToBodyAction(feetAction);
	}
	else if (state.scrollWheelValue > scrollStore)
	{
		gun = static_cast<Guns>((((gun - 1) % MAX_WEAPON) + MAX_WEAPON) % MAX_WEAPON);
		while (onHand[gun] == nullptr)
			gun = static_cast<Guns>((((gun - 1) % MAX_WEAPON) + MAX_WEAPON) % MAX_WEAPON);

		bodyAction = FeetActionToBodyAction(feetAction);
	}

	scrollStore = state.scrollWheelValue;

	//Keyboard Inputs
	if (!kb.LeftShift || bodyAction == reload)
	{
		if (kb.W)
		{
			feetAction = walkF;
			Walk(forward);
			if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
			{
				bodyAction = move;
				sprite[1] = anim->move[((Gun*)onHand[gun])->GetType()];
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
				sprite[1] = anim->move[((Gun*)onHand[gun])->GetType()];
			}
		}
	}

	if (kb.A)
	{
		Walk(left);
		feetAction = leftF;
		if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
		{
			sprite[1] = anim->move[((Gun*)onHand[gun])->GetType()];
			bodyAction = move;
		}

	}
	else if (kb.D)
	{
		Walk(right);
		feetAction = rightF;
		if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
		{
			bodyAction = move;
			sprite[1] = anim->move[((Gun*)onHand[gun])->GetType()];
		}
	}
	else if (kb.S)
	{
		feetAction = walkF;
		Walk(backward);
		if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
		{
			sprite[1] = anim->move[((Gun*)onHand[gun])->GetType()];
			bodyAction = move;
		}
	}
	else;
	if (!kb.A && !kb.D & !kb.W && !kb.S)
	{
		if (bodyAction != BodyAction::reload && bodyAction != BodyAction::meleeattack && bodyAction != shoot)
		{
			sprite[1] = anim->idle[((Gun*)onHand[gun])->GetType()];
			bodyAction = idle;
		}
		feetAction = idleF;
	}

	if (kb.D1)
	{
		gun = flashlight;
		bodyAction = FeetActionToBodyAction(feetAction);
	}
	if (kb.D2)
	{
		gun = knife;
		bodyAction = FeetActionToBodyAction(feetAction);
	}
	if (kb.D3 && onHand[2] != nullptr)
	{
		gun = pistol;
		bodyAction = FeetActionToBodyAction(feetAction);
	}
	if (kb.D4 && onHand[3] != nullptr)
	{
		gun = shotgun;
		bodyAction = FeetActionToBodyAction(feetAction);
	}
	if (kb.D5 && onHand[4] != nullptr)
	{
		gun = rifle;
		bodyAction = FeetActionToBodyAction(feetAction);
	}

	if (kb.R)
	{
		if (bodyAction != shoot && bodyAction != reload && bodyAction != meleeattack )
		{
			if(((Gun*)onHand[gun])->CanReload())
			{
				sprite[1] = anim->reload[((Gun*)onHand[gun])->GetType() - 2];
				bodyFrame = 0;
				bodyAction = reload;
			}
		}
	}

	if (bodyAction == reload)
	{
		if (bodyFrame > (sprite[1]->GetXY().x * sprite[1]->GetXY().y))
		{
			int x = ((Gun*)onHand[gun])->Reload();
			inventory->decreaseItem(inventory->GetItemFromID(onHand[gun]->GetAmmoID()), x);

			if (onHand[gun]->GetType() != shotgun)
			{
				//inventory->decreaseItem(inventory->GetItemFromID(onHand[gun]->GetAmmoID()), x);

				map->CreateAmmoClipEmpty((((Gun*)onHand[gun])->CalculateRelativePosition() + position) / Vector2(2.f, 2.f), (float)rand(), device, camera);
				bodyAction = FeetActionToBodyAction(feetAction);
			}
			else
			{
				if (bodyAction == reload && !kb.LeftShift && !state.leftButton && !state.rightButton && ((Gun*)onHand[gun])->CanReload())
				{
					sprite[1] = anim->reload[((Gun*)onHand[gun])->GetType() - 2];
					bodyAction = reload;
				}
				else
					bodyAction = FeetActionToBodyAction(feetAction);
			}

			bodyFrame = 0;
		}
	}
	else if (bodyAction == shoot || bodyAction == meleeattack)
		if (bodyFrame > (sprite[1]->GetXY().x * sprite[1]->GetXY().y))
			bodyAction = FeetActionToBodyAction(feetAction);

	//
	IncFrame();
	sprite[0] = anim->feet[feetAction];

	sprite[0]->CalculateSourceRect(feetFrame);
	sprite[1]->CalculateSourceRect(bodyFrame);

	//
	((Gun*)onHand[gun])->Update();

	if (feetAction == walkF)
		((Gun*)onHand[gun])->SetRebound(250);
	else if (feetAction != idleF)
		((Gun*)onHand[gun])->SetRebound(500);

	//COLLISION
	this->CalculateVertex();

	StaticObject** staticObjects = map->GetStaticObjects();
	for (int i = 0; i < map->GetMaxStatic(); i++)
	{
		if (staticObjects[i] != nullptr)
		{
			Vector2 normal = Collision::CheckCollisionT(this, staticObjects[i]);
			position = position + Vector2(3, 3) * normal;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (zombies[i] != nullptr)
		{
			Vector2 normal = Collision::CheckCollisionT(this, zombies[i]);
			position = position + Vector2(0.3f, 0.3f) * normal;
		}
	}

	StaticObject** groundObjects = map->GetGroundObjects();
	memset(closestGroundObjects, 0, sizeof(StaticObject*)*(MAX_GROUNDSTATICOBJECTS / 3));
	int counter = 0;
	for (int i = 0; i < map->GetMaxGroundObject(); i++)
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

	if (kb.E)
	{
		int i = 0;
		while (closestGroundObjects[i] != nullptr)
		{
			i++;
		}
		if (i != 0)
		{
			TakeItem((Item *)closestGroundObjects[i - 1], map);
		}
	}
	
	sprite[0]->SetPosition(position);
	sprite[0]->SetRotation(rotation);
	sprite[1]->SetPosition(position);
	sprite[1]->SetRotation(rotation);

	if (sprite[0]->GetPosition() != sprite[1]->GetPosition())
		printf("a");
}

Inventory * Survivor::GetInventory()
{
	return inventory;
}

Guns Survivor::GetOnHand() { return gun; }

Gun ** Survivor::GetOnHands()
{
	return onHand;
}

int Survivor::GetAmmoBackup()
{
	if (gun == pistol)
		return ((Gun*)onHand[2])->GetBackup();
	else if (gun == shotgun)
		return ((Gun*)onHand[3])->GetBackup();
	else if (gun == rifle)
		return ((Gun*)onHand[4])->GetBackup();
	else
		return NULL;
}

Camera * Survivor::GetCamera()
{
	return camera;
}

Guns Survivor::GetOnHandType(int row)
{
	if (((Gun*)onHand[row]) != nullptr)
		return ((Gun*)onHand[row])->GetType();
	else
		return noGun;
}

StaticObject ** Survivor::GetClosestGroundObjects()
{
	return closestGroundObjects;
}

int Survivor::GetPlaceFromItem(Item* item)
{
	int i = 2; 
	while (onHand[i] != item && i < MAX_WEAPON)
		i++;


	if (onHand[i] != item)
		return -1;

	return i;
}

void Survivor::SwitchInventoryOnHand(int y, int x)
{
	Item* temp = inventory->GetItemFromPlace(x);
	inventory->SetItemToPlace(onHand[y], x);
	onHand[y] = (Gun*)temp;

	
}

void Survivor::SwitchOnHand(int x, int y)
{
	Gun* temp = onHand[x];
	onHand[x] = onHand[y];
	onHand[y] = temp;
}

void Survivor::SwitchOnHandInventory(int x, int y)
{
	Item* temp = inventory->GetItemFromPlace(y);
	inventory->SetItemToPlace(onHand[x], y);
	onHand[x] = (Gun*)temp;
}

void Survivor::SetOnHand(Guns gun)
{
	this->gun = gun;
}

int Survivor::GetAmmoCurrent()
{
	if (gun == pistol)
		return ((Gun*)onHand[2])->GetCurrentAmmo();
	else if (gun == shotgun)
		return ((Gun*)onHand[3])->GetCurrentAmmo();
	else if (gun == rifle)
		return ((Gun*)onHand[4])->GetCurrentAmmo();
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
	else
		return idle;
}

void Survivor::CalculateVertex()
{
	float sinx = sin(rotation), cosx = cos(rotation);

	if (gun == flashlight)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 105, sinx * 105) + Vector2(-sinx * 5, cosx * 5);	//ön
		ver.vertex[4] = this->GetPosition() + Vector2(cosx * 105, sinx * 105) + Vector2(sinx * 5, -cosx * 5);	//ön
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-sinx * 40, cosx * 40) + Vector2(cosx * 10, sinx * 10);	//sað
		ver.vertex[1] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(sinx * 35, -cosx * 35); //arka
		ver.vertex[0] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(-sinx * 35, cosx * 35); //arka
	}
	else if (gun == knife)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 60, sinx * 60) + Vector2(sinx * 15, -cosx * 15);				//ön
		ver.vertex[4] = this->GetPosition() + Vector2(sinx * 45, -cosx * 45) + Vector2(cosx * 10, sinx * 10);				//sol
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * (-15), sinx * (-15));			//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-cosx * 35, -sinx * 35);																//arka
		ver.vertex[1] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(-cosx * 20, -sinx * 20);				//sað
		ver.vertex[0] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(cosx * 60, sinx * 60);				//sað
	}
	else if (gun == pistol)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 50, sinx * 50) + Vector2(-sinx * 25, cosx * 25);	//ön
		ver.vertex[4] = this->GetPosition() + Vector2(cosx * 50, sinx * 50) + Vector2(sinx * 20, -cosx * 20);	//ön
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-sinx * 40, cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sað
		ver.vertex[1] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(sinx * 35, -cosx * 35); //arka
		ver.vertex[0] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(-sinx * 35, cosx * 35); //arka
	}
	else if (gun == shotgun)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 65, sinx * 65) + Vector2(-sinx * 15, cosx * 15);	//ön
		ver.vertex[4] = this->GetPosition() + Vector2(cosx * 55, sinx * 55) + Vector2(sinx * 15, -cosx * 15);	//ön
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-sinx * 40, cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sað
		ver.vertex[1] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(sinx * 35, -cosx * 35); //arka
		ver.vertex[0] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(-sinx * 35, cosx * 35); //arka
	}
	else if (gun == rifle)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 65, sinx * 65) + Vector2(-sinx * 15, cosx * 15);	//ön
		ver.vertex[4] = this->GetPosition() + Vector2(cosx * 55, sinx * 55) + Vector2(sinx * 15, -cosx * 15);	//ön
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-sinx * 40, cosx * 40) + Vector2(cosx * 15, sinx * 15);	//sað
		ver.vertex[1] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(sinx * 35, -cosx * 35); //arka
		ver.vertex[0] = this->GetPosition() + Vector2(-cosx * 20, -sinx * 20) + Vector2(-sinx * 35, cosx * 35); //arka
	}
}

void Survivor::UpdateGunsBackup()
{
	for (int i = 0; i < MAX_WEAPON; i++)
	{
		if(onHand[i]!= nullptr)
			onHand[i]->SetBackup(inventory->GetItemCount(onHand[i]->GetAmmoID()));

	}

	Item** items = inventory->GetItems();
	for (int i = 0; i < MAX_ITEM; i++)
	{
		if(items[i] != nullptr && items[i]->IsHandable())
			((Gun*)items[i])->SetBackup(inventory->GetItemCount(((Gun*)items[i])->GetAmmoID()));
	}
}

float Survivor::CalculateRotation(DirectX::Mouse::State state, float rot, Vector2 position/*, Camera* cam*/)
{
	int tempX = state.x - (int)position.x /*- cam.x*/;
	int tempY = (int)position.y - state.y /*+ cam.y*/;

	if ((tempX > 120 || tempX < -120) || (tempY > 120 || tempY < -120))
		return ((float)((atan2(tempX, tempY)) - (PI / 2)));
	else
		return rot;
}

void Survivor::IncFrame()
{
	if (bodyFrame > 60)
		bodyFrame = 0;
	if (feetFrame > 60)
		feetFrame = 0;

	if (clock() - time > 50)
	{
		bodyFrame++;
		feetFrame++;
		time = clock();
	}
}

void Survivor::TakeItem(Item *x,Map *m)
{
	if(inventory->addItem(x))
	{ 
		m->RemoveItem(x);
		UpdateGunsBackup();

		if (x->IsHandable())
			((Gun*)x)->SetOwner(this);
	}
}

void Survivor::DropItem(Item *x, Map *m)
{
	x->SetPosition(GetPosition());
	m->AddItem(x);
	inventory->removeItem(x);
	UpdateGunsBackup();
}

void Survivor::DropItemToInventory(Item *item)
{
	if (GetPlaceFromItem(item) == gun)
		gun = knife;

	inventory->addItem(item);
	onHand[GetPlaceFromItem(item)] = nullptr;
}

