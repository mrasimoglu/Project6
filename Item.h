#ifndef ITEM_H
#define ITEM_H

#pragma once

#include "StaticObject.h"

#include "SpriteSheet.h"
#include "InfoBox.h"

class Item : public StaticObject
{
protected:
	InfoBox* infoBox;
	SpriteSheet* icon;
	wchar_t* name;
	int ID;
	int numOfItem;
	int weight;

	bool handable;

public:
	Item();
	Item(wchar_t* iconPath, wchar_t* spritePath, wchar_t* name, int ID, int numOfItem, int weight, Vector2 pos, float rot, Shape shape, Camera* cam);
	~Item();

	void SetNum(int);

	SpriteSheet* GetSSIcon();

	int GetNum();
	int GetWeight();
	int GetID();
	bool IsHandable();
	SpriteSheet* GetIcon();

	InfoBox* GetInfoBox();
};

#endif
