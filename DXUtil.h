#pragma once

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <DirectXColors.h>
#include "DXERR/dxerr.h"

#ifndef HR
#ifdef  _DEBUG
#define  HR(x) \
		{ \
			HRESULT hr = x; \
			if(FAILED(hr)) \
			{\
				DXTraceW(__FILEW__, __LINE__, hr, L#x, TRUE); \
			}\
		}
#else  // !_DEBUG
#define  HR(x) x;
#endif  // _DEBUG
#endif // !HR?


namespace Memory
{
	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template <class T> void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}

	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}


namespace Utility
{
	void GetTextureDim(ID3D11Resource* res, UINT* width, UINT* height);
}