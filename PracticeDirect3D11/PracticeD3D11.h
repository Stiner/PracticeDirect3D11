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
    WCHAR _szTitle[Const::MAX_LOADSTRING] = {};
    WCHAR _szWindowClass[Const::MAX_LOADSTRING] = {};

    HINSTANCE _hInstance = nullptr;
    HWND      _hWnd = nullptr;

    D3D_DRIVER_TYPE         _DriverType            = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL       _FeatureLevel          = D3D_FEATURE_LEVEL_11_0;

    ID3D11Device*           _pD3DDevice            = nullptr;
    ID3D11Device1*          _pD3DDevice1           = nullptr;

    ID3D11DeviceContext*    _pD3DImmediateContext  = nullptr;
    ID3D11DeviceContext1*   _pD3DImmediateContext1 = nullptr;

    IDXGISwapChain*         _pDXGISwapChain        = nullptr;
    IDXGISwapChain1*        _pDXGISwapChain1       = nullptr;

    ID3D11RenderTargetView* _pD3DRenderTargetView  = nullptr;
    ID3D11DepthStencilView* _pD3DDepthStencilView  = nullptr;

    ID3D11VertexShader*     _pD3DVertexShader      = nullptr;
    ID3D11PixelShader*      _pD3DPixelShader       = nullptr;
    ID3D11InputLayout*      _pD3DVertexLayout      = nullptr;
    ID3D11Buffer*           _pD3DVertexBuffer      = nullptr;
};
