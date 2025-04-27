// PracticeD3D11

#include "PCH.h"
#include "CD3DApp.h"

#include "res/Resource.h"
#include <sstream>

#include "CD3DDevice.h"

namespace
{
    CD3D11App* gpD3D11App = nullptr;

    LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return gpD3D11App->MsgProc(hwnd, msg, wParam, lParam);
    }
}

CD3D11App::CD3D11App(HINSTANCE hAppInstance, const WCHAR* szAppName)
    : _hAppInstance(hAppInstance)
    , _strMainWndCaption(szAppName)
{
    gpD3D11App = this;

    _ClientWidth = 800;
    _ClientHeight = 600;
}

CD3D11App::~CD3D11App()
{
    gpD3D11App = nullptr;
}

HINSTANCE CD3D11App::GetHandleAppInst() const
{
    return _hAppInstance;
}

HWND CD3D11App::GetHandleMainWnd() const
{
    return _hMainWindow;
}

float CD3D11App::GetAspectRatio() const
{
    return static_cast<float>(_ClientWidth) / _ClientHeight;
}

int CD3D11App::Run(const WCHAR* szCmdLine)
{
    if (FAILED(ProccessCmdLine(szCmdLine)))
        return -1;

    if (FAILED(Initialize()))
        return -1;

    MSG msg = {};

    _Timer.Reset();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            _Timer.Tick();

            if (_IsAppPaused)
            {
                Sleep(100);
            }
            else
            {
                CalculateFrameStats();
                UpdateScene(_Timer.DeltaTime());
                Draw();
            }
        }
    }

    Release();

    return static_cast<int>(msg.wParam);
}

HRESULT CD3D11App::Initialize()
{
    if (FAILED(InitMainWindow()))
        return E_FAIL;

    if (FAILED(InitDevice()))
        return E_FAIL;

    return S_OK;
}

void CD3D11App::Release()
{
    COM_RELEASE(_D3DDepthStencilView);
    COM_RELEASE(_D3DDepthStencilBuffer);
    COM_RELEASE(_D3DRenderTargetView);

    CD3DDevice::Release();
}

HRESULT CD3D11App::OnResize()
{
    assert(CD3DDevice::Device);
    assert(CD3DDevice::Context);
    assert(CD3DDevice::SwapChain);

    COM_RELEASE(_D3DRenderTargetView);
    COM_RELEASE(_D3DDepthStencilView);
    COM_RELEASE(_D3DDepthStencilBuffer);

    // D3DRenderTargetView 생성
    {
        R_CHECK(CD3DDevice::SwapChain->ResizeBuffers(1, _ClientWidth, _ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

        ID3D11Texture2D* backBuffer;
        R_CHECK(CD3DDevice::SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));

        HRESULT hr = CD3DDevice::Device->CreateRenderTargetView(backBuffer, 0, &_D3DRenderTargetView);
        COM_RELEASE(backBuffer);
        R_CHECK(hr);
    }

    // DepthStencil을 위한 ID3DTexture2D Buffer 생성 및 ID3DDepthStencilView 생성
    {
        D3D11_TEXTURE2D_DESC descDepthStencilBuffer = {};
        descDepthStencilBuffer.Width              = _ClientWidth;
        descDepthStencilBuffer.Height             = _ClientHeight;
        descDepthStencilBuffer.MipLevels          = 1;
        descDepthStencilBuffer.ArraySize          = 1;
        descDepthStencilBuffer.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepthStencilBuffer.SampleDesc.Count   = CD3DDevice::IsEnabledMSAA ? 4 : 1;
        descDepthStencilBuffer.SampleDesc.Quality = CD3DDevice::IsEnabledMSAA ? CD3DDevice::QualityMSAA : 0;
        descDepthStencilBuffer.Usage              = D3D11_USAGE_DEFAULT;
        descDepthStencilBuffer.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        descDepthStencilBuffer.CPUAccessFlags     = 0;
        descDepthStencilBuffer.MiscFlags          = 0;

        //D3D11_SUBRESOURCE_DATA subResourceData = {};
        R_CHECK(CD3DDevice::Device->CreateTexture2D(&descDepthStencilBuffer, nullptr/*&subResourceData*/, &_D3DDepthStencilBuffer));

        //D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView = {};
        R_CHECK(CD3DDevice::Device->CreateDepthStencilView(_D3DDepthStencilBuffer, nullptr/*&descDepthStencilView*/, &_D3DDepthStencilView));
    }

    {
        // 두번째 인자는 배열 포인터. 첫번째 인자로 배열 갯수
        CD3DDevice::Context->OMSetRenderTargets(1, &_D3DRenderTargetView, _D3DDepthStencilView);
    }

    // Viewport 설정
    {
        _D3DScreenViewport.Width    = static_cast<float>(_ClientWidth);
        _D3DScreenViewport.Height   = static_cast<float>(_ClientHeight);
        _D3DScreenViewport.MinDepth = 0.0f;
        _D3DScreenViewport.MaxDepth = 1.0f;
        _D3DScreenViewport.TopLeftX = 0;
        _D3DScreenViewport.TopLeftY = 0;

        // 두번째 인자는 배열 포인터. 첫번째 인자로 배열 갯수
        CD3DDevice::Context->RSSetViewports(1, &_D3DScreenViewport);
    }

    return S_OK;
}

void CD3D11App::Draw()
{
    HRESULT hr;

    assert(CD3DDevice::Device);
    assert(CD3DDevice::Context);
    assert(CD3DDevice::SwapChain);
    assert(_D3DRenderTargetView);
    assert(_D3DDepthStencilView);

    CD3DDevice::Context->ClearRenderTargetView(_D3DRenderTargetView, _ClearColor);
    CD3DDevice::Context->ClearDepthStencilView(_D3DDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    DrawScene();

    CHECK(CD3DDevice::SwapChain->Present(0, 0));
}

LRESULT CD3D11App::MsgProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_ACTIVATE:
            {
                if (LOWORD(wParam) == WA_INACTIVE)
                {
                    _IsAppPaused = true;
                    _Timer.Stop();
                }
                else
                {
                    _IsAppPaused = false;
                    _Timer.Start();
                }
                return 0;
            }
        case WM_SIZE:
            {
                _ClientWidth  = LOWORD(lParam);
                _ClientHeight = HIWORD(lParam);

                if (CD3DDevice::IsInitialized)
                {
                    if (wParam == SIZE_MINIMIZED)
                    {
                        _IsAppPaused = true;
                        _IsMinimized = true;
                        _IsMaximized = false;
                    }
                    else if (wParam == SIZE_MAXIMIZED)
                    {
                        _IsAppPaused = false;
                        _IsMinimized = false;
                        _IsMaximized = true;
                    }
                    else if (wParam == SIZE_RESTORED)
                    {
                        if (_IsMinimized)
                        {
                            _IsAppPaused = false;
                            _IsMinimized = false;
                            OnResize();
                        }
                        else if (_IsMaximized)
                        {
                            _IsAppPaused = false;
                            _IsMaximized = false;
                            OnResize();
                        }
                        else if (_IsResizing)
                        {
                        }
                        else
                        {
                            OnResize();
                        }
                    }
                }
                return 0;
            }
        case WM_ENTERSIZEMOVE:
            {
                _IsAppPaused = true;
                _IsResizing  = true;
                _Timer.Stop();
                return 0;
            }
        case WM_EXITSIZEMOVE:
            {
                _IsAppPaused = false;
                _IsResizing  = false;
                _Timer.Start();
                OnResize();
                return 0;
            }
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        case WM_MENUCHAR:
            {
                return MAKELRESULT(0, MNC_CLOSE);
            }
        case WM_GETMINMAXINFO:
            {
                RECT rc = { 0, 0, 640, 480 };
                AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
                int minWidth  = rc.right - rc.left;
                int minHeight = rc.bottom - rc.top;

                ((MINMAXINFO*)lParam)->ptMinTrackSize.x = minWidth;
                ((MINMAXINFO*)lParam)->ptMinTrackSize.y = minHeight;
                return 0;
            }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            {
                OnMouseDown(wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
                return 0;
            }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            {
                OnMouseUp(wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
                return 0;
            }
        case WM_MOUSEMOVE:
            {
                OnMouseMove(wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
                return 0;
            }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HRESULT CD3D11App::ProccessCmdLine(const WCHAR* szCmdLine)
{
    return S_OK;
}

HRESULT CD3D11App::InitMainWindow()
{
    WNDCLASSEX wcex = {};
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = MainWndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = _hAppInstance;
    wcex.hIcon         = LoadIcon(_hAppInstance, MAKEINTRESOURCE(IDI_PRACTICEDIRECT3D11));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszMenuName  = 0;
    wcex.lpszClassName = _strMainWndCaption.c_str();
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassExW(&wcex))
    {
        MessageBox(0, TEXT("Failed to register WndClass."), TEXT("ERROR"), 0);
        return E_FAIL;
    }

    RECT rc = { 0, 0, _ClientWidth, _ClientHeight };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = rc.right - rc.left;
    int windowHeight = rc.bottom - rc.top;

    _hMainWindow = CreateWindow(_strMainWndCaption.c_str(), _strMainWndCaption.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowWidth, windowHeight,
        nullptr, nullptr,
        _hAppInstance,
        nullptr);

    if (_hMainWindow == nullptr)
    {
        MessageBox(0, TEXT("Failed to create window."), TEXT("ERROR"), 0);
        return E_FAIL;
    }

    ShowWindow(_hMainWindow, SW_SHOW);
    UpdateWindow(_hMainWindow);

    return S_OK;
}

HRESULT CD3D11App::InitDevice()
{
    HRESULT hr = S_OK;

    R_CHECK(CD3DDevice::Initialize(_hMainWindow, _ClientWidth, _ClientHeight, true, false, 0));
    R_CHECK(OnResize());

    return S_OK;
}

void CD3D11App::CalculateFrameStats()
{
    static int32 frameCount = 0;
    static float elapsedTime = 0.0f;

    frameCount++;

    if ((_Timer.TotalTime() - elapsedTime) >= 1.0f)
    {
        float fps = static_cast<float>(frameCount);
        float milliSecPerFPS = 1000.0f / fps;

#ifdef _DEBUG
        std::wostringstream outs;
        outs.precision(6);
        outs << _strMainWndCaption << TEXT(" | ");
        outs << TEXT("FPS: ") << fps << TEXT(" | ");
        outs << TEXT("FrameTime: ") << milliSecPerFPS << TEXT(" (ms)");
        SetWindowText(_hMainWindow, outs.str().c_str());
#endif

        frameCount = 0;
        elapsedTime += 1.0f;
    }
}
