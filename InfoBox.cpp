#include "InfoBox.h"



InfoBox::InfoBox(wchar_t* info)
{
	this->info = info;

	sprite = new SpriteSheet(Vector2(100, 100));
	sprite->SetWidthHeight((Vector2)DXApp::spriteFont->MeasureString(info) * 2.f + Vector2(10.f, 10.f));
	sprite->Load(DXApp::m_pDevice, L"img/hud/infobox.dds");

	twh = DXApp::spriteFont->MeasureString(info) / 2.f;
}


InfoBox::~InfoBox()
{
}

void InfoBox::Draw(DirectX::Mouse::State mouse)
{
	sprite->SetPosition(Vector2(mouse.x + twh.x, mouse.y + twh.y));
	sprite->Draw(DXApp::spriteBatch);

	DXApp::spriteFont->DrawString(DXApp::spriteBatch, info, sprite->GetPosition(), DirectX::Colors::White, 0.0f, twh, Vector2(1.f, 1.f), DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}
