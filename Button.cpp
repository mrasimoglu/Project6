#include "Button.h"



Button::Button(ID3D11Device* device, wchar_t *path, wchar_t* text, Vector2 pos, float rot)
{
	sprite = new SpriteSheet(pos, rot);
	sprite->Load(device, path);

	position = pos;
	rotation = rot;

	this->text = text;
}

void Button::Draw(SpriteBatch* spriteBatch, SpriteFont* spriteFont)
{
	Vector2 textLen = spriteFont->MeasureString(text) / 2.f;
	sprite->Draw(spriteBatch);
	
	spriteFont->DrawString(spriteBatch, this->text, position, DirectX::Colors::White, rotation, textLen, sprite->GetScale() * 4,  DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

bool Button::isPressed(DirectX::Mouse::State state)
{
	int w = sprite->GetWidth() * sprite->GetScale().x;
	int h = sprite->GetHeight() * sprite->GetScale().y;

	int x = sprite->GetPosition().x;
	int y = sprite->GetPosition().y;

	if (state.x < x + (w / 2) && state.x > x - (w / 2))
		if (state.y < y + (h / 2) && state.y > y - (h / 2))
			if(state.leftButton)
				return true;
		
	return false;
}

bool Button::isOver(DirectX::Mouse::State state)
{
	int w = sprite->GetWidth() * sprite->GetScale().x;
	int h = sprite->GetHeight() * sprite->GetScale().y;

	int x = sprite->GetPosition().x;
	int y = sprite->GetPosition().y;

	if (state.x < x + (w / 2) && state.x > x - (w / 2))
		if (state.y < y + (h / 2) && state.y > y - (h / 2))
			return true;

	return false;
}

void Button::Maximize()
{
	sprite->SetScale(Vector2(0.35f, 0.35f));
}

void Button::Minimize()
{
	sprite->SetScale(Vector2(0.25f, 0.25f));
}

Button::~Button()
{
}
