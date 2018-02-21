#include "Item.h"

Item::Item()
{
}

Item::Item(wchar_t* iconPath, wchar_t* spritePath, wchar_t* name, int ID, int numOfItem, int weight, Vector2 pos, float rot, Shape shape, Camera* cam)
	: StaticObject( spritePath, pos, rot, rectangle, cam)
{
	infoBox = new InfoBox(name);

	this->name = name;

	handable = false;

	icon = new SpriteSheet();
	icon->Load(DXApp::m_pDevice, iconPath);
	icon->SetScale({1.f,1.f});
	this->ID = ID;
	this->numOfItem = numOfItem;
	this->weight = weight;
}


Item::~Item()
{
	free(*sprite);
}

void Item::SetNum(int num)
{
	numOfItem = num;
}

SpriteSheet * Item::GetSSIcon()
{
	return icon;
}

int Item::GetID()
{
	return ID;
}

bool Item::IsHandable()
{
	return handable;
}

SpriteSheet * Item::GetIcon()
{
	return icon;
}

InfoBox * Item::GetInfoBox()
{
	return infoBox;
}

int Item::GetNum()
{
	return numOfItem;
}

int Item::GetWeight()
{
	return numOfItem * weight;
}
