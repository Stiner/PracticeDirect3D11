// PracticeD3D11

#include "PCH.h"
#include "D3DApp.h"

#include "res/Resource.h"
#include <sstream>

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
    if (!ProccessCmdLine(szCmdLine))
        return -1;

    if (!Initialize())
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

bool CD3D11App::Initialize()
{
    if (!InitMainWindow())
        return false;

    if (!InitDirect3D())
        return false;

    return true;
}

void CD3D11App::Release()
{
    COM_RELEASE(_D3DDepthStencilView);
    COM_RELEASE(_D3DDepthStencilBuffer);
    COM_RELEASE(_D3DRenderTargetView);
    COM_RELEASE(_DXGISwapChain1);
    COM_RELEASE(_DXGISwapChain);
    COM_RELEASE(_D3DDeviceContext1);
    COM_RELEASE(_D3DDeviceContext);
    COM_RELEASE(_D3DDevice1);
    COM_RELEASE(_D3DDevice);
}

bool CD3D11App::OnResize()
{
    HRESULT hr = S_OK;

    assert(_D3DDeviceContext1 || _D3DDeviceContext);
    assert(_D3DDevice1 || _D3DDevice);
    assert(_DXGISwapChain1 || _DXGISwapChain);

    COM_RELEASE(_D3DRenderTargetView);
    COM_RELEASE(_D3DDepthStencilView);
    COM_RELEASE(_D3DDepthStencilBuffer);

    // D3DRenderTargetView 생성

    {
        hr = _DXGISwapChain->ResizeBuffers(1, _ClientWidth, _ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }

        ID3D11Texture2D* backBuffer;
        hr = _DXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }

        hr = _D3DDevice->CreateRenderTargetView(backBuffer, 0, &_D3DRenderTargetView);
        COM_RELEASE(backBuffer);
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }
    }

    // DepthStencil을 위한 ID3DTexture2D Buffer 생성 및 ID3DDepthStencilView 생성
    {
        D3D11_TEXTURE2D_DESC descDepthStencilBuffer = {};
        descDepthStencilBuffer.Width              = _ClientWidth;
        descDepthStencilBuffer.Height             = _ClientHeight;
        descDepthStencilBuffer.MipLevels          = 1;
        descDepthStencilBuffer.ArraySize          = 1;
        descDepthStencilBuffer.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepthStencilBuffer.SampleDesc.Count   = _Enable4xMSAA ? 4 : 1;
        descDepthStencilBuffer.SampleDesc.Quality = _Enable4xMSAA ? _Quality4xMSAA - 1 : 0;
        descDepthStencilBuffer.Usage              = D3D11_USAGE_DEFAULT;
        descDepthStencilBuffer.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        descDepthStencilBuffer.CPUAccessFlags     = 0;
        descDepthStencilBuffer.MiscFlags          = 0;

        D3D11_SUBRESOURCE_DATA subResourceData = {};

        hr = _D3DDevice->CreateTexture2D(&descDepthStencilBuffer, nullptr/*&subResourceData*/, &_D3DDepthStencilBuffer);
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }

        D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView = {};

        hr = _D3DDevice->CreateDepthStencilView(_D3DDepthStencilBuffer, nullptr/*&descDepthStencilView*/, &_D3DDepthStencilView);
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }
    }

    {
        // 두번째 인자는 배열 포인터. 첫번째 인자로 배열 갯수
        _D3DDeviceContext->OMSetRenderTargets(1, &_D3DRenderTargetView, _D3DDepthStencilView);
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
        _D3DDeviceContext->RSSetViewports(1, &_D3DScreenViewport);
    }

    return SUCCEEDED(hr);
}

void CD3D11App::Draw()
{
    HRESULT hr;

    assert(_D3DDeviceContext);
    assert(_D3DRenderTargetView);
    _D3DDeviceContext->ClearRenderTargetView(_D3DRenderTargetView, _ClearColor);
    assert(_D3DDepthStencilView);
    _D3DDeviceContext->ClearDepthStencilView(_D3DDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    DrawScene();

    assert(_DXGISwapChain);
    hr = _DXGISwapChain->Present(0, 0);
    assert(SUCCEEDED(hr));
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

                if (_D3DDevice)
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

bool CD3D11App::ProccessCmdLine(const WCHAR* szCmdLine)
{
    return true;
}

bool CD3D11App::InitMainWindow()
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
        return false;
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
        return false;
    }

    ShowWindow(_hMainWindow, SW_SHOW);
    UpdateWindow(_hMainWindow);

    return true;
}

bool CD3D11App::InitDirect3D()
{
    HRESULT hr = S_OK;

    // ID3DDevice 및 ID3DDeviceContext 생성
    {
        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            _D3DDriverType = driverTypes[driverTypeIndex];

            hr = D3D11CreateDevice(nullptr, _D3DDriverType, nullptr, createDeviceFlags,
                featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
                &_D3DDevice, &_D3DFeatureLevel, &_D3DDeviceContext);
            if (hr == E_INVALIDARG)
            {
                // DirectX 11.0 플랫폼은 D3D_FEATURE_LEVEL_11_1을 인식하지 못하므로 이를 제외하고 다시 시도
                hr = D3D11CreateDevice(nullptr, _D3DDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &_D3DDevice, &_D3DFeatureLevel, &_D3DDeviceContext);
            }

            if (SUCCEEDED(hr))
                break;
        }

        
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }

        // DirectX 11.1 or later
        if (_D3DFeatureLevel == D3D_FEATURE_LEVEL_11_1)
        {
            hr = _D3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_D3DDevice1));
            hr = _D3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_D3DDeviceContext1));
        }

        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }
    }

    // 4x MSAA 품질 지원 확인
    {
        hr = _D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_Quality4xMSAA);
        _Enable4xMSAA = _Quality4xMSAA > 0;

        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }
    }

    // IDXGIFactory 생성해서 IDXGISwapChain 생성
    {
        // ID3DDevice에서 IDXGIFactory 얻기
        IDXGIFactory1* pDXGIFactory1 = nullptr; // IDXGIFactory는 D3D10 이후부터 지원되는 DXGI 라이브러리의 주요 인터페이스. 그러므로 IDXGIFactory1을 사용.
        {
            IDXGIDevice* pDXGIDevice = nullptr;
            hr = _D3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
            if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }

            IDXGIAdapter* pDXGIAdapter = nullptr;
            hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
            COM_RELEASE(pDXGIDevice);
            if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }

            hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDXGIFactory1));
            COM_RELEASE(pDXGIAdapter);
            if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }
        }

        // D3D 버전에 맞춰 IDXGISwapChain 생성
        {
            // D3D11.1 이상에서는 IDXGIFactory2 인터페이스를 사용하여 스왑 체인을 생성
            IDXGIFactory2* pDXGIFactory2 = nullptr;
            hr = pDXGIFactory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pDXGIFactory2));
            if (SUCCEEDED(hr))
            {
                // DirectX 11.1 systems
                DXGI_SWAP_CHAIN_DESC1 descSwapchain = {};
                descSwapchain.Width              = _ClientWidth;
                descSwapchain.Height             = _ClientHeight;
                descSwapchain.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                descSwapchain.BufferCount        = 1;
                descSwapchain.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
                descSwapchain.Flags              = 0;
                descSwapchain.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
                descSwapchain.SampleDesc.Count   = (_Enable4xMSAA) ? 4 : 1;
                descSwapchain.SampleDesc.Quality = (_Enable4xMSAA) ? _Quality4xMSAA - 1 : 0;

                DXGI_SWAP_CHAIN_FULLSCREEN_DESC descSwapchainFullScreen = {};
                descSwapchainFullScreen.Windowed                = true;
                descSwapchainFullScreen.RefreshRate.Numerator   = 60;
                descSwapchainFullScreen.RefreshRate.Denominator = 1;
                descSwapchainFullScreen.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
                descSwapchainFullScreen.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

                // DirectX 11.1 or later
                hr = pDXGIFactory2->CreateSwapChainForHwnd(_D3DDevice, _hMainWindow,
                    &descSwapchain, &descSwapchainFullScreen, nullptr,
                    &_DXGISwapChain1);
                if (SUCCEEDED(hr))
                {
                    hr = _DXGISwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_DXGISwapChain));
                    if (SUCCEEDED(hr))
                    {
                        // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
                        hr = pDXGIFactory2->MakeWindowAssociation(_hMainWindow, DXGI_MWA_NO_ALT_ENTER);
                    }
                }
                COM_RELEASE(pDXGIFactory2);
            }
            // D3D11.0 이하에서는 IDXGIFactory1 인터페이스를 사용하여 스왑 체인을 생성
            else
            {
                DXGI_SWAP_CHAIN_DESC sd = {};
                sd.BufferDesc.Width                   = _ClientWidth;
                sd.BufferDesc.Height                  = _ClientHeight;
                sd.BufferDesc.RefreshRate.Numerator   = 60;
                sd.BufferDesc.RefreshRate.Denominator = 1;
                sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
                sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
                sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
                sd.SampleDesc.Count                   = (_Enable4xMSAA) ? 4 : 1;
                sd.SampleDesc.Quality                 = (_Enable4xMSAA) ? _Quality4xMSAA - 1 : 0;
                sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sd.BufferCount                        = 1;
                sd.OutputWindow                       = _hMainWindow;
                sd.Windowed                           = true;
                sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
                sd.Flags                              = 0;

                hr = pDXGIFactory1->CreateSwapChain(_D3DDevice, &sd, &_DXGISwapChain);
                if (SUCCEEDED(hr))
                {
                    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
                    hr = pDXGIFactory1->MakeWindowAssociation(_hMainWindow, DXGI_MWA_NO_ALT_ENTER);
                }
            }
        }

        COM_RELEASE(pDXGIFactory1);
        if (FAILED(hr)) { assert(SUCCEEDED(hr)); return false; }
    }

    if (!OnResize())
        return false;

    return true;
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
