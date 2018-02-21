#include "Inventory.h"

Inventory::Inventory()
{
	items = (Item**)calloc(sizeof(Item*), MAX_ITEM);
	currentWeight = 0;
	maxWeight = 101;
}

bool Inventory::addItem(Item * item)
{
	if (currentWeight + item->GetWeight() < maxWeight)
	{
		int i = -1;
		do
		{
			i++;
			while(items[i] == nullptr && i < MAX_ITEM)
				i++;
			
		} while (i < MAX_ITEM && items[i]->GetID() != item->GetID() );
		if (i == MAX_ITEM)
		{// bulamadýk itemi
			i = 0;
			while (items[i] != nullptr && i < MAX_ITEM)
				i++;
			items[i] = item;

		}
		else // itemi bulduk
		{
			if(!items[i]->IsHandable())
				items[i]->SetNum(items[i]->GetNum() + item->GetNum());
			else
			{
				i = 0;
				while (items[i] != nullptr && i < MAX_ITEM)
					i++;
				items[i] = item;
			}
		}

		return true;
	}
	else
		return false;
}

void Inventory::replaceItem(Item *item, int slot)
{
	int i = 0;
	while (items[i] != item)
		i++;

	Item* temp = items[slot];
	items[slot] = items[i];
	items[i] = temp;
}

void Inventory::removeItem(Item *item)
{
	int i = -1;
	do
	{
		i++;
		while (items[i] == nullptr && i < MAX_ITEM-1)
			i++;

	} while (i < MAX_ITEM && items[i]->GetID() != item->GetID());
	if (i != MAX_ITEM)// itemi bulduk
	{
			items[i] = nullptr;
	}

}

bool Inventory::decreaseItem(Item *item, int num)
{
	int i = -1;
	do
	{
		i++;
		while (items[i] == nullptr && i < MAX_ITEM - 1)
			i++;

	} while (i < MAX_ITEM && items[i]->GetID() != item->GetID());
	if (i != MAX_ITEM)// itemi bulduk
	{
		items[i]->SetNum(items[i]->GetNum() - num);

		if(items[i]->GetNum() <= 0)
			items[i] = nullptr;
	}

	return false;
}

void Inventory::increaseItem(Item *)
{
}

Item ** Inventory::GetItems()
{
	return items;
}

int Inventory::GetItemCount(int ID)
{
	int i = 0;
	while (items[i] != nullptr && items[i]->GetID() != ID)
	{
		i++;
	}
	if (items[i] != nullptr && items[i]->GetID() == ID)
		return items[i]->GetNum();
	else
		return 0;
}

Item * Inventory::GetItemFromID(int ID)
{
	int i = 0;
	while ((items[i]!=nullptr) ? (items[i]->GetID() != ID) : true)
	{		
		i++; 
	}
	if (items[i]->GetID() == ID)
		return items[i];
	else
		return nullptr;
}

Item * Inventory::GetItemFromPlace(int i)
{
	return items[i];
}

int Inventory::GetPlaceFromItem(Item *item)
{
	int i = 0;
	while (items[i] != item &&  i < MAX_ITEM)
		i++;
	if (items[i] != item)
		return -1;

	return i;
}

void Inventory::SetItemToPlace(Item *item, int i)
{
	items[i] = item;
}

Inventory::~Inventory()
{
	free(items);
}
