#include "Zombie.h"



Zombie::Zombie(ID3D11Device* device, Camera* cam, Vector2 pos)
{
	numOfSprites = 1;
	sprite[0] = new SpriteSheet();

	sprites[0] = new SpriteSheet(4, 4);
	sprites[0]->SetWidthHeight(Vector2(241, 222));
	sprites[0]->Load(device, L"img/zombie/idle.dds");
	sprites[0]->SetOrigin(Vector2(90, 120));

	sprites[1] = new SpriteSheet(4, 4);
	sprites[1]->SetWidthHeight(Vector2(288, 311));
	sprites[1]->Load(device, L"img/zombie/move.dds");
	sprites[1]->SetOrigin(Vector2(103, 171));

	sprites[2] = new SpriteSheet(3, 3);
	sprites[2]->SetWidthHeight(Vector2(318, 294));
	sprites[2]->Load(device, L"img/zombie/attack.dds");
	sprites[2]->SetOrigin(Vector2(130, 158));
	
	this->camera = cam;

	speed = 0.00000005f;

	action = idleZ;

	sprite[0] = sprites[action];
	sprite[0]->SetPosition(pos);

	position = pos;

	health = 100;

	ver.numOfVertex = 6;
	ver.vertex = (Vector2*)malloc(sizeof(Vector2) * ver.numOfVertex);

	target = Vector2(-1000, -1000);
	lastSeen = clock();
}

void Zombie::Update(Map* map, Survivor* survivor, Zombie** zombies)
{
	if (action != deadZ)
	{
		CalculateVertex();
		IncFrame();
		sprite[0]->CalculateSourceRect(frame);
		
		if (Collision::CalculateMinDistance(survivor, this) < 50)
		{
			action = attackZ;
			rotation = atan2(survivor->GetPosition().y - position.y, survivor->GetPosition().x - position.x);
		}
		else
			action = idleZ;

		if (action != attackZ)
		{
			if (Collision::Trace66(survivor, GetPosition(), map->GetStaticObjects(), camera) /*&& Collision::Distance2D(position, survivor->GetPosition()) < 750*/)
			{
				target = survivor->GetPosition();
				printf("1\n");
				lastSeen = clock();
			}

			if (Collision::Distance2D(position, target) < 150)
				target = Vector2(-1000, -1000);

			if (target != Vector2(-1000, -1000))
			{
				rotation = atan2((target.y - position.y), (target.x - position.x));
				this->Walk(forward);
				action = moveZ;

				if (lastSeen + 10000 < clock())
				{
					target = Vector2(-1000, -1000);
				}
			}
			else
				action = idleZ;
		}

		StaticObject** staticObjects = map->GetStaticObjects();
		for (int i = 0; i < map->GetMaxStatic(); i++)
		{
			if (staticObjects[i] != nullptr)
			{
				Vector2 normal = Collision::CheckCollisionT(this, staticObjects[i]);
				position = position + Vector2(1, 1) * normal;
			}
		}

		for (int i = 0; i < 10; i++)
		{
			if (zombies[i] != nullptr && zombies[i] != this)
			{
				Vector2 normal = Collision::CheckCollisionT(this, zombies[i]);
				position = position + Vector2(3, 3) * normal;
			}
		}


		sprite[0] = sprites[action];
		sprite[0]->SetRotation(rotation);
		sprite[0]->SetPosition(position);


		FiniteObject** bullets = map->GetBullets();
		for (int i = 0; i < map->GetMaxBullet(); i++)
		{
			if (bullets[i] && Collision::CheckCollision(this, bullets[i]))
			{
				map->CreateBlood(bullets[i]->GetPosition(), survivor->GetRotation(), camera);
				int temp = health;
				health -= bullets[i]->GetDamage() * (1 / Collision::DamageDistance2(this, bullets[i], camera));
				Memory::SafeDelete(bullets[i]);
				printf("%d\n", temp - health);
			}
		}
	}

}

void Zombie::IncFrame()
{
	if (frame > 60)
		frame = 0;

	if (clock() - time > 100)
	{
		frame++;
		time = clock();
	}
}

void Zombie::CalculateVertex()
{
	float sinx = sin(rotation), cosx = cos(rotation);

	if (action == idleZ)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 60, sinx * 60) + Vector2(sinx * 15, -cosx * 15);				//ön
		ver.vertex[4] = this->GetPosition() + Vector2(sinx * 45, -cosx * 45) + Vector2(cosx * 10, sinx * 10);				//sol
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * (-15), sinx * (-15));			//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-cosx * 35, -sinx * 35);																//arka
		ver.vertex[1] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(-cosx * 20, -sinx * 20);				//sað
		ver.vertex[0] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(cosx * 60, sinx * 60);				//sað
	}
	else if (action == moveZ)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 60, sinx * 60) + Vector2(sinx * 15, -cosx * 15);				//ön
		ver.vertex[4] = this->GetPosition() + Vector2(sinx * 45, -cosx * 45) + Vector2(cosx * 10, sinx * 10);				//sol
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * (-15), sinx * (-15));			//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-cosx * 35, -sinx * 35);																//arka
		ver.vertex[1] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(-cosx * 20, -sinx * 20);				//sað
		ver.vertex[0] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(cosx * 60, sinx * 60);				//sað
	}
	else if (action == attackZ)
	{
		ver.vertex[5] = this->GetPosition() + Vector2(cosx * 60, sinx * 60) + Vector2(sinx * 15, -cosx * 15);				//ön
		ver.vertex[4] = this->GetPosition() + Vector2(sinx * 45, -cosx * 45) + Vector2(cosx * 10, sinx * 10);				//sol
		ver.vertex[3] = this->GetPosition() + Vector2(sinx * 40, -cosx * 40) + Vector2(cosx * (-15), sinx * (-15));			//sol
		ver.vertex[2] = this->GetPosition() + Vector2(-cosx * 35, -sinx * 35);																//arka
		ver.vertex[1] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(-cosx * 20, -sinx * 20);				//sað
		ver.vertex[0] = this->GetPosition() + Vector2(-sinx * 35, cosx * 35) + Vector2(cosx * 60, sinx * 60);				//sað
	}
}

Zombie::~Zombie()
{
}
