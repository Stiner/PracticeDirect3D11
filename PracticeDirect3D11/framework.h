// Practice D3D11

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
// D3D 헤더 파일
#include <d3d11.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif

#define MEM_DELETE(_x) { if(_x != nullptr) { delete _x; _x = nullptr; } }
#define COM_RELEASE(_x) { if (static_cast<IUnknown*>(_x) != nullptr) { _x->Release(); _x = nullptr; } }
