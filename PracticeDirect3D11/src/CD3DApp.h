// PracticeD3D11

#pragma once

#include "CGameTimer.h"

class CD3DDevice;

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
    virtual HRESULT ProccessCmdLine(const WCHAR* szCmdLine);
    virtual HRESULT Initialize();
    virtual void Release();
    virtual HRESULT OnResize();
    virtual void UpdateScene(float dt) = 0;
    virtual void DrawScene() = 0;

    virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
    virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
    virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

    HRESULT InitMainWindow();
    HRESULT InitDevice();

    void CalculateFrameStats();

    void Draw();

protected:
    HINSTANCE _hAppInstance = nullptr;

    HWND _hMainWindow = nullptr;

    bool _IsAppPaused = false;
    bool _IsMinimized = false;
    bool _IsMaximized = false;
    bool _IsResizing = false;

    CGameTimer _Timer;

    ID3D11RenderTargetView* _D3DRenderTargetView = nullptr;
    ID3D11DepthStencilView* _D3DDepthStencilView = nullptr;
    ID3D11Texture2D* _D3DDepthStencilBuffer = nullptr;

    D3D11_VIEWPORT _D3DScreenViewport = {};

    std::wstring _strMainWndCaption;

    int32 _ClientWidth = 0;
    int32 _ClientHeight = 0;

    float _ClearColor[4] = { 0.098039225f, 0.098039225f, 0.439215720f, 1.f };
};
