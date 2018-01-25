#include "SpriteSheet.h"
#include "DDSTextureLoader.h"

SpriteSheet::SpriteSheet(void)
{
	m_Position = Vector2(100, 100);
	m_Alpha = 1.0f;
	m_Rotation = 0.0f;
	m_Scale = Vector2(0.5f, 0.5f);
	m_Tint = DirectX::Colors::White.v;
	m_Width = 0;
	m_Height = 0;
	x = 1;
	y = 1;
}

SpriteSheet::SpriteSheet(const Vector2& position)
{
	m_Position = position;

	m_Alpha = 1.0f;
	m_Rotation = 0.0f;
	m_Scale = Vector2(0.5f, 0.5f);
	m_Tint = DirectX::Colors::White.v;
	x = 1;
	y = 1;
}

SpriteSheet::SpriteSheet(int x, int y)
{
	this->x = x;
	this->y = y;

	m_Position = Vector2(100, 100);
	m_Alpha = 1.0f;
	m_Rotation = 0.0f;
	m_Scale = Vector2(0.5f, 0.5f);
	m_Tint = DirectX::Colors::White.v;
	m_Width = 0;
	m_Height = 0;
}

SpriteSheet::SpriteSheet(const Vector2 & position, float rotation)
{
	m_Position = position;

	m_Alpha = rotation;
	m_Rotation = 0.0f;
	m_Scale = Vector2(0.5f, 0.5f);
	m_Tint = DirectX::Colors::White.v;
	x = 1;
	y = 1;
}

SpriteSheet::~SpriteSheet(void)
{
	Memory::SafeRelease(m_pTexture);
	Memory::SafeRelease(m_pResource);
}

void SpriteSheet::Load(ID3D11Device * device, const wchar_t * file)
{
	//Import texture
	HR(DirectX::CreateDDSTextureFromFile(device, file, &m_pResource, &m_pTexture));
	if (!m_Width && !m_Height)
		Utility::GetTextureDim(m_pResource, &m_Width, &m_Height);

	m_Origin = Vector2(m_Width / 2.0f, m_Height / 2.0f);

	m_SourceRect.left = 0;
	m_SourceRect.top = 0;
	m_SourceRect.right = m_Width;
	m_SourceRect.bottom = m_Height;
}

void SpriteSheet::Draw(DirectX::SpriteBatch *spriteBatch)
{
	spriteBatch->Draw(m_pTexture, m_Position, &m_SourceRect, m_Tint, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

void SpriteSheet::Draw(DirectX::SpriteBatch *spriteBatch, Camera* cam)
{
	spriteBatch->Draw(m_pTexture, m_Position - cam->GetOffset(), &m_SourceRect, m_Tint, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

void SpriteSheet::DrawString(SpriteFont* spriteFont, SpriteBatch* spriteBatch, const wchar_t *text)
{
	spriteFont->DrawString(spriteBatch, text, m_Position, Colors::White.v, 0, m_Origin, m_Scale, DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

 void SpriteSheet::DrawString(SpriteFont* spriteFont, SpriteBatch* spriteBatch, const wchar_t *text, Color color)
{
	spriteFont->DrawString(spriteBatch, text, m_Position, color, 0, m_Origin, m_Scale, DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

const Vector2 & SpriteSheet::GetPosition() const
{
	return m_Position;
}

const Vector2 & SpriteSheet::GetOrigin() const
{
	return m_Origin;
}

const Vector2 & SpriteSheet::GetScale() const
{
	return m_Scale;
}

const Color & SpriteSheet::GetTint() const
{
	return m_Tint;
}

const float & SpriteSheet::GetAlpha() const
{
	return m_Alpha;
}

const float & SpriteSheet::GetRotation() const
{
	return m_Rotation;
}

const RECT& SpriteSheet::GetSourceRect() const
{
	return m_SourceRect;
}

const Vector2& SpriteSheet::GetXY() const
{
	return Vector2(x, y);
}

const int & SpriteSheet::GetWidth()
{
	return this->m_Width;
}

const int & SpriteSheet::GetHeight()
{
	return this->m_Height;
}

void SpriteSheet::AddToPosition(const Vector2 & position)
{
	this->m_Position.x += position.x;
	m_Position.y += position.y;
}

void SpriteSheet::SetPosition(const Vector2 & position)
{
	m_Position = position;
}

void SpriteSheet::SetOrigin(const Vector2 & origin)
{
	m_Origin = origin;
}

void SpriteSheet::SetScale(const Vector2 & scale)
{
	m_Scale = scale;
}

void SpriteSheet::SetTint(const Color & tint)
{
	m_Tint = tint;
}

void SpriteSheet::SetAlpha(const float alpha)
{
	m_Alpha = alpha;
}

void SpriteSheet::SetRotation(const float rotation)
{
	m_Rotation = rotation;
}

void SpriteSheet::SetSourceRect(const RECT rect)
{
	m_SourceRect = rect;
}

void SpriteSheet::SetWidthHeight(Vector2 size)
{
	m_Width = size.x;
	m_Height = size.y;
}

void SpriteSheet::SetXY(Vector2 vect)
{
	this->x = vect.x;
	this->y = vect.y;
}

void SpriteSheet::CalculateSourceRect(int frame)
{
	frame = frame % (x * y);

	m_SourceRect.top = (frame % x) * m_Height;
	m_SourceRect.left = (frame / x) * m_Width;
	m_SourceRect.bottom = (frame % x) * m_Height + m_Height;
	m_SourceRect.right = floor(frame / x) * m_Width + m_Width;
}