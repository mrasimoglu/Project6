#pragma once

#include <d3d11.h>
#include "d3d11.h"
#include "DXUtil.h"
#include "SimpleMath.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"

#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

class SpriteSheet
{
public:
	SpriteSheet(void);
	SpriteSheet(int x, int y);
	SpriteSheet(const Vector2& poistion);
	SpriteSheet(const Vector2& position, float rotation);
	~SpriteSheet(void);

	//GETTERS
	const Vector2& GetPosition() const;
	const Vector2& GetOrigin() const;
	const Vector2& GetScale() const;
	const Color& GetTint() const;
	const float& GetAlpha() const;
	const float& GetRotation() const;
	const RECT& GetSourceRect() const;
	const Vector2& GetXY() const;
	const int& GetWidth();
	const int& GetHeight();

	//
	void AddToPosition(const Vector2& position);

	//SETTERS
	void SetPosition(const Vector2& position);
	void SetOrigin(const Vector2& origin);
	void SetScale(const Vector2& scale);
	void SetTint(const Color& tint);
	void SetAlpha(const float alpha);
	void SetRotation(const float rotation);
	void SetSourceRect(const RECT rect);
	void SetWidthHeight(const Vector2 size);
	void SetXY(Vector2);

	//LOAD
	void Load(ID3D11Device* device, const wchar_t* file);

	//DRAW
	void Draw(SpriteBatch* spriteBatch, Camera*);
	void Draw(SpriteBatch* spriteBatch);
	void DrawString(SpriteFont* spriteFont, SpriteBatch* spriteBatch, const wchar_t *, Color);
	void DrawString(SpriteFont* spriteFont, SpriteBatch* spriteBatch, const wchar_t *);

	void CalculateSourceRect(int frame);

protected:
	ID3D11Resource*				m_pResource;
	ID3D11ShaderResourceView*	m_pTexture;

	UINT						m_Width;
	UINT						m_Height;

	Vector2						m_Position;
	Vector2						m_Origin;
	Vector2						m_Scale;
	Color						m_Tint;
	float						m_Alpha;
	float						m_Rotation;
	RECT						m_SourceRect;

	int x, y;
};