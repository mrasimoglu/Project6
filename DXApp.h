#pragma once

#include <d3d11.h>
#include <windows.h>
#include <string>

#include "DXUtil.h"

#define WIN_32_LEAN_AND_MEAN

class DXApp
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp(void);

	//MAIN APPLICATION LOOP
	int Run();

	//FRAMEWORK METHODS
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static ID3D11Device*				m_pDevice;

protected:
	//WIN32 ATTRIBUTES
	HWND			m_hAppWnd;
	HINSTANCE		m_hAppInstance;
	UINT			m_ClientWidth;
	UINT			m_ClientHeight;
	std::string		m_AppTitle;
	DWORD			m_WndStyle;

	//DIRECTX ATTIRBUTES
	ID3D11DeviceContext*		m_pImmidiateContext;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	IDXGISwapChain*				m_pSwapChain;
	D3D_DRIVER_TYPE				m_DriverType;
	D3D_FEATURE_LEVEL			m_FeatureLevel;
	D3D11_VIEWPORT				m_Viewport;

	//INITIALIZE WIN32 WINDOW
	bool InitWindow();

	//INITIALIZE DIRECT3D
	bool InitDirect3D();
};