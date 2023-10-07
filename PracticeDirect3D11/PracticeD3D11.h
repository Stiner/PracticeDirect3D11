// Practice D3D11

#pragma once

#include "framework.h"


class MyApp
{
public:
    enum Const
    {
        MAX_LOADSTRING = 100
    };

public:
    int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

protected:
    HRESULT InitWithCmd(LPWSTR lpCmdLine);
    HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
    HRESULT InitDevice();
    HRESULT InitShader();
    HRESULT InitGeometry();

    void InputProcess();
    void UpdateProcess();
    void RenderProcess();

    void CleanupApp();

protected:
    WCHAR _szTitle[Const::MAX_LOADSTRING];
    WCHAR _szWindowClass[Const::MAX_LOADSTRING];

    HINSTANCE _hInstance;
    HWND      _hWnd;

    D3D_DRIVER_TYPE         _driverType         = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL       _featureLevel       = D3D_FEATURE_LEVEL_11_0;
    ID3D11Device*           _pd3dDevice         = nullptr;
    ID3D11Device1*          _pd3dDevice1        = nullptr;
    ID3D11DeviceContext*    _pImmediateContext  = nullptr;
    ID3D11DeviceContext1*   _pImmediateContext1 = nullptr;
    IDXGISwapChain*         _pSwapChain         = nullptr;
    IDXGISwapChain1*        _pSwapChain1        = nullptr;
    ID3D11RenderTargetView* _pRenderTargetView  = nullptr;

    ID3D11VertexShader*     _pVertexShader      = nullptr;
    ID3D11PixelShader*      _pPixelShader       = nullptr;
    ID3D11InputLayout*      _pVertexLayout      = nullptr;
    ID3D11Buffer*           _pVertexBuffer      = nullptr;
};
