#include "CharacterAnimations.h"



CharacterAnimations::CharacterAnimations(ID3D11Device* device)
{
	feet[0] = new SpriteSheet(1, 1);
	feet[0]->Load(device, L"img/survivor/feet/idle.dds");

	feet[1] = new SpriteSheet(5, 4);
	feet[1]->SetWidthHeight(Vector2(172, 124));
	feet[1]->Load(device, L"img/survivor/feet/walk.dds");

	feet[2] = new SpriteSheet(5, 4);
	feet[2]->SetWidthHeight(Vector2(204, 124));
	feet[2]->Load(device, L"img/survivor/feet/run.dds");

	feet[3] = new SpriteSheet(5, 4);
	feet[3]->SetWidthHeight(Vector2(155, 174));
	feet[3]->Load(device, L"img/survivor/feet/left.dds");

	feet[4] = new SpriteSheet(5, 4);
	feet[4]->SetWidthHeight(Vector2(154, 176));
	feet[4]->Load(device, L"img/survivor/feet/right.dds");

	//

	idle[0] = new SpriteSheet(5, 4);
	idle[0]->SetWidthHeight(Vector2(303, 223));
	idle[0]->Load(device, L"img/survivor/idle/flashlight.dds");
	idle[0]->SetOrigin(Vector2(93, 106));

	idle[1] = new SpriteSheet(5, 4);
	idle[1]->SetWidthHeight(Vector2(289, 224));
	idle[1]->Load(device, L"img/survivor/idle/knife.dds");
	idle[1]->SetOrigin(Vector2(106, 113));

	idle[2] = new SpriteSheet(5, 4);
	idle[2]->SetWidthHeight(Vector2(253, 216));
	idle[2]->Load(device, L"img/survivor/idle/pistol.dds");
	idle[2]->SetOrigin(Vector2(97, 120));

	idle[3] = new SpriteSheet(5, 4);
	idle[3]->SetWidthHeight(Vector2(313, 207));
	idle[3]->Load(device, L"img/survivor/idle/shotgun.dds");
	idle[3]->SetOrigin(Vector2(94, 120));

	idle[4] = new SpriteSheet(5, 4);
	idle[4]->SetWidthHeight(Vector2(313, 207));
	idle[4]->Load(device, L"img/survivor/idle/rifle.dds");
	idle[4]->SetOrigin(Vector2(94, 120));

	//

	move[0] = new SpriteSheet(5, 4);
	move[0]->SetWidthHeight(Vector2(305, 231));
	move[0]->Load(device, L"img/survivor/move/flashlight.dds");
	move[0]->SetOrigin(Vector2(93, 113));

	move[1] = new SpriteSheet(5, 4);
	move[1]->SetWidthHeight(Vector2(279, 219));
	move[1]->Load(device, L"img/survivor/move/knife.dds");
	move[1]->SetOrigin(Vector2(106, 112));

	move[2] = new SpriteSheet(5, 4);
	move[2]->SetWidthHeight(Vector2(258, 220));
	move[2]->Load(device, L"img/survivor/move/pistol.dds");
	move[2]->SetOrigin(Vector2(101, 119));

	move[3] = new SpriteSheet(5, 4);
	move[3]->SetWidthHeight(Vector2(313, 206));
	move[3]->Load(device, L"img/survivor/move/shotgun.dds");
	move[3]->SetOrigin(Vector2(94, 119));

	move[4] = new SpriteSheet(5, 4);
	move[4]->SetWidthHeight(Vector2(313, 206));
	move[4]->Load(device, L"img/survivor/move/rifle.dds");
	move[4]->SetOrigin(Vector2(94, 119));

	//

	meleeattack[0] = new SpriteSheet(5, 3);
	meleeattack[0]->SetWidthHeight(Vector2(316, 246));
	meleeattack[0]->Load(device, L"img/survivor/meleeattack/flashlight.dds");
	meleeattack[0]->SetOrigin(Vector2(99, 129));

	meleeattack[1] = new SpriteSheet(5, 3);
	meleeattack[1]->SetWidthHeight(Vector2(329, 300));
	meleeattack[1]->Load(device, L"img/survivor/meleeattack/knife.dds");
	meleeattack[1]->SetOrigin(Vector2(103, 113));

	meleeattack[2] = new SpriteSheet(5, 3);
	meleeattack[2]->SetWidthHeight(Vector2(291, 256));
	meleeattack[2]->Load(device, L"img/survivor/meleeattack/pistol.dds");
	meleeattack[2]->SetOrigin(Vector2(103, 124));

	meleeattack[3] = new SpriteSheet(5, 3);
	meleeattack[3]->SetWidthHeight(Vector2(358, 353));
	meleeattack[3]->Load(device, L"img/survivor/meleeattack/shotgun.dds");
	meleeattack[3]->SetOrigin(Vector2(114, 201));

	meleeattack[4] = new SpriteSheet(5, 3);
	meleeattack[4]->SetWidthHeight(Vector2(358, 353));
	meleeattack[4]->Load(device, L"img/survivor/meleeattack/rifle.dds");
	meleeattack[4]->SetOrigin(Vector2(114, 201));

	//

	shoot[0] = new SpriteSheet(3, 1);
	shoot[0]->SetWidthHeight(Vector2(255, 215));
	shoot[0]->Load(device, L"img/survivor/shoot/pistol.dds");
	shoot[0]->SetOrigin(Vector2(98, 119));

	shoot[1] = new SpriteSheet(5, 3);
	shoot[1]->SetWidthHeight(Vector2(322, 217));
	shoot[1]->Load(device, L"img/survivor/shoot/shotgun.dds");
	shoot[1]->SetOrigin(Vector2(100, 121));

	shoot[2] = new SpriteSheet(3, 1);
	shoot[2]->SetWidthHeight(Vector2(312, 206));
	shoot[2]->Load(device, L"img/survivor/shoot/rifle.dds");
	shoot[2]->SetOrigin(Vector2(93, 119));

	//

	reload[0] = new SpriteSheet(5, 3);
	reload[0]->SetWidthHeight(Vector2(260, 230));
	reload[0]->Load(device, L"img/survivor/reload/pistol.dds");
	reload[0]->SetOrigin(Vector2(100, 119));

	reload[1] = new SpriteSheet(5, 3);
	reload[1]->SetWidthHeight(Vector2(322, 217));
	reload[1]->Load(device, L"img/survivor/reload/shotgun.dds");
	reload[1]->SetOrigin(Vector2(100, 121));

	reload[2] = new SpriteSheet(5, 3);
	reload[2]->SetWidthHeight(Vector2(322, 217));
	reload[2]->Load(device, L"img/survivor/reload/rifle.dds");
	reload[2]->SetOrigin(Vector2(100, 121));
}

CharacterAnimations::~CharacterAnimations()
{
}
