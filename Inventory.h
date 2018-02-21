#pragma once

#define MAX_ITEM 12
#include "Item.h"

class Inventory
{
private:
	Item** items;

	int maxWeight;
	int currentWeight;
public:

	bool addItem(Item*);
	void replaceItem(Item*, int);
	void removeItem(Item*);
	bool decreaseItem(Item*, int);
	void increaseItem(Item*);
	
	Item** GetItems();
	int GetItemCount(int);
	Item* GetItemFromID(int ID);
	Item* GetItemFromPlace(int);
	int GetPlaceFromItem(Item*);

	void SetItemToPlace(Item*, int);

	Inventory();
	~Inventory();
};

