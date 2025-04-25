// PracticeD3D11

#pragma once

#include "CGameTimer.h"

class CD3D11App
{
public:
    CD3D11App(HINSTANCE hAppInstance, const WCHAR* szAppName);
    virtual ~CD3D11App();

    HINSTANCE GetHandleAppInst() const;
    HWND GetHandleMainWnd() const;
    float GetAspectRatio() const;

    int Run(const WCHAR* szCmdLine);

    virtual LRESULT MsgProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

protected:
    virtual bool ProccessCmdLine(const WCHAR* szCmdLine);
    virtual bool Initialize();
    virtual void Release();
    virtual bool OnResize();
    virtual void UpdateScene(float dt) = 0;
    virtual void DrawScene() = 0;

    virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
    virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
    virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

    bool InitMainWindow();
    bool InitDevice();

    void CalculateFrameStats();

    void Draw();

protected:
    HINSTANCE _hAppInstance = nullptr;

    HWND _hMainWindow = nullptr;

    bool _IsAppPaused = false;
    bool _IsMinimized = false;
    bool _IsMaximized = false;
    bool _IsResizing = false;

    uint32 _Quality4xMSAA = 0;

    CGameTimer _Timer;

    ID3D11Device* _D3DDevice = nullptr;
    ID3D11Device1* _D3DDevice1 = nullptr;

    ID3D11DeviceContext* _D3DDeviceContext = nullptr;
    ID3D11DeviceContext1* _D3DDeviceContext1 = nullptr;

    IDXGISwapChain* _DXGISwapChain = nullptr;
    IDXGISwapChain1* _DXGISwapChain1 = nullptr;

    ID3D11RenderTargetView* _D3DRenderTargetView = nullptr;
    ID3D11DepthStencilView* _D3DDepthStencilView = nullptr;
    ID3D11Texture2D* _D3DDepthStencilBuffer = nullptr;

    D3D11_VIEWPORT _D3DScreenViewport = {};

    std::wstring _strMainWndCaption;

    D3D_DRIVER_TYPE _D3DDriverType = D3D_DRIVER_TYPE_UNKNOWN;
    D3D_FEATURE_LEVEL _D3DFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

    int32 _ClientWidth = 0;
    int32 _ClientHeight = 0;

    bool _Enable4xMSAA = false;

    float _ClearColor[4] = { 0.098039225f, 0.098039225f, 0.439215720f, 1.f };
};
