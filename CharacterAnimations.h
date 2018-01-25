#pragma once

#include "SpriteSheet.h"

class CharacterAnimations
{
public:
	CharacterAnimations(ID3D11Device*);
	~CharacterAnimations();

	SpriteSheet* feet[5];

	SpriteSheet* idle[5];
	SpriteSheet* move[5];
	SpriteSheet* meleeattack[5];
	SpriteSheet* shoot[3];
	SpriteSheet* reload[3];
};

