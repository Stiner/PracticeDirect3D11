#pragma once

#include "framework.h"

enum Const
{
    MAX_LOADSTRING = 100
};

class MyGameApp
{
public:
    int Run(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);

private:
    HRESULT InitWithCmd(LPWSTR lpCmdLine);
    HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
    HRESULT InitDevice();
    HRESULT Reset();
    void InputProcess();
    void UpdateProcess();
    void RenderProcess();
    void ReleaseApp();

    static LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    WCHAR szTitle[Const::MAX_LOADSTRING];
    WCHAR szWindowClass[Const::MAX_LOADSTRING];

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
};
