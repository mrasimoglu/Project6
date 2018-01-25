#include "Entity.h"

Entity::Entity()
{
	if (numOfSprites == NULL)
		numOfSprites = 0;

	sprite = (SpriteSheet**)malloc(numOfSprites * sizeof(SpriteSheet));
}

void Entity::Draw(SpriteBatch *spriteBatch, Camera* cam)
{
	for (int i = 0; i < numOfSprites; i++)
		sprite[i]->Draw(spriteBatch, cam);
}

//SETTERS
void Entity::SetPosition(Vector2 position) { this->position = position; }
void Entity::SetRotation(float rotation) { this->rotation = rotation; }

//GETTERS
Vector2 Entity::GetPosition() {	return this->GetSprite()->GetPosition(); }
float Entity::GetRotation() { return this->rotation; }
float Entity::GetRadius() { return this->sprite[0]->GetSourceRect().right; }
int Entity::GetWidth() { return sprite[0]->GetWidth(); }
int Entity::GetHeight() { return sprite[0]->GetHeight(); }
SpriteSheet * Entity::GetSprite() {	return sprite[0]; }

Vertexs *Entity::GetVertexs()
{
	return &this->ver;
}

Entity::~Entity()
{

}
