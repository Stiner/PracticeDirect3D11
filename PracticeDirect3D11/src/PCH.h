// PracticeD3D11

#pragma once

#define WIN32_LEAN_AND_MEAN

// C 런타임 헤더 파일
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
// C++ 런타임 헤더 파일
#include <iostream>
#include <string>
#include <vector>
#include <memory>
// Windows 헤더 파일
#include <windows.h>
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

typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
#ifdef _MSC_VER
typedef signed __int64      int64;
#else
typedef signed long long    int64;
#endif

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
#ifdef _MSC_VER
typedef unsigned __int64    uint64;
#else
typedef unsigned long long  uint64;
#endif

typedef unsigned char   byte;
typedef unsigned short  word;

using namespace DirectX;
