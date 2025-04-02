@@ -1,426 +1,69 @@
﻿// Practice D3D11

#include "resource.h"
#include "WindowProcess.h"
#include "MyUtility.h"
#include "PracticeD3D11.h"

struct SimpleVertex



{
    DirectX::XMFLOAT3 Pos;
};

int MyApp::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)

{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    if (FAILED(InitWithCmd(lpCmdLine)))
        return E_FAIL;

    if (FAILED(InitWindow(hInstance, nCmdShow)))
        return E_FAIL;

    if (FAILED(InitDevice()))
    {
        CleanupApp();
        return E_FAIL;
    }

    if (FAILED(InitShader()))
    {
        CleanupApp();
        return E_FAIL;
    }

    if (FAILED(InitGeometry()))
    {
        CleanupApp();
        return E_FAIL;
    }

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            InputProcess();
            UpdateProcess();
            RenderProcess();
        }
    }

    CleanupApp();

    return (int)msg.wParam;
}

HRESULT MyApp::InitWithCmd(LPWSTR lpCmdLine)
{
    return S_OK;
}

HRESULT MyApp::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    LoadStringW(hInstance, IDS_APP_TITLE, _szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PRACTICEDIRECT3D11, _szWindowClass, MAX_LOADSTRING);

    // 윈도우 클래스를 등록합니다.
    WNDCLASSEXW wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WindowProcess;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRACTICEDIRECT3D11));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_PRACTICEDIRECT3D11);
    wcex.lpszClassName = _szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassExW(&wcex))
        return E_FAIL;

    RECT rc = { 0, 0, 800, 600 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindowW(_szWindowClass, _szTitle,
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                            CW_USEDEFAULT, CW_USEDEFAULT,           // x, y
                            rc.right - rc.left, rc.bottom - rc.top  // w, h
                            , nullptr, nullptr, hInstance, nullptr);

    if (!_hWnd)
        return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);
    UpdateWindow(_hWnd);

    _hInstance = hInstance;

    return S_OK;
}

HRESULT MyApp::InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(_hWnd, &rc);
    UINT width  = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

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
        _DriverType = driverTypes[driverTypeIndex];

        hr = D3D11CreateDevice(nullptr, _DriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &_pD3DDevice, &_FeatureLevel, &_pD3DImmediateContext);
        if (hr == E_INVALIDARG)
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(nullptr, _DriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &_pD3DDevice, &_FeatureLevel, &_pD3DImmediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr)) return hr;

    // DirectX 11.1 or later
    if (_FeatureLevel == D3D_FEATURE_LEVEL_11_1)
    {
        hr = _pD3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_pD3DDevice1));
        hr = _pD3DImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_pD3DImmediateContext1));
    }

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* pDXGIFactory1 = nullptr;
    // dxgi.lib 링크 안하고 생성시.
    {
        IDXGIDevice* pDXGIDevice = nullptr;
        hr = _pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
        if (FAILED(hr)) return hr;

        IDXGIAdapter* pDXGIAdapter = nullptr;
        hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
        COM_RELEASE(pDXGIDevice);
        if (FAILED(hr)) return hr;

        hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDXGIFactory1));
        COM_RELEASE(pDXGIAdapter);
        if (FAILED(hr)) return hr;
    }
    // dxgi.lib 사용한다면 이 방식으로.
    {
        //hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDXGIFactory1));
        //SAFE_RELEASE(pDXGIFactory);
        //if (FAILED(hr)) return hr;
    }

    // Create swap chain
    {
        IDXGIFactory2* pDXGIFactory2 = nullptr;
        hr = pDXGIFactory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pDXGIFactory2));
        if (SUCCEEDED(hr))
        {
            DXGI_SWAP_CHAIN_DESC1 sd = {};
            sd.Width              = width;
            sd.Height             = height;
            sd.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.SampleDesc.Count   = 1;
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount        = 1;

            // DirectX 11.1 or later
            hr = pDXGIFactory2->CreateSwapChainForHwnd(_pD3DDevice, _hWnd, &sd, nullptr, nullptr, &_pDXGISwapChain1);
            if (SUCCEEDED(hr))
            {
                hr = _pDXGISwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_pDXGISwapChain));
                if (SUCCEEDED(hr))
                {
                    hr = pDXGIFactory2->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);
                }
            }
        }
        else
        {
            // DirectX 11.0 systems
            DXGI_SWAP_CHAIN_DESC sd = {};
            sd.BufferCount                        = 1;
            sd.BufferDesc.Width                   = width;
            sd.BufferDesc.Height                  = height;
            sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator   = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow                       = _hWnd;
            sd.SampleDesc.Count                   = 1;
            sd.SampleDesc.Quality                 = 0;
            sd.Windowed                           = TRUE;

            hr = pDXGIFactory1->CreateSwapChain(_pD3DDevice, &sd, &_pDXGISwapChain);
            if (SUCCEEDED(hr))
            {
                // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
                hr = pDXGIFactory1->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);
            }
        }

        COM_RELEASE(pDXGIFactory2);
    }

    COM_RELEASE(pDXGIFactory1);

    if (FAILED(hr)) return hr;

    // Create a render target view
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = _pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (FAILED(hr)) return hr;

        hr = _pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pD3DRenderTargetView);
        COM_RELEASE(pBackBuffer);
        if (FAILED(hr)) return hr;
    }

    _pD3DImmediateContext->OMSetRenderTargets(1, &_pD3DRenderTargetView, nullptr);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width    = (FLOAT)width;
    vp.Height   = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pD3DImmediateContext->RSSetViewports(1, &vp);

    return hr;
}

HRESULT MyApp::InitShader()
{
    HRESULT hr;

    // Vertex Shader
    {
        ID3DBlob* pVertexShaderBlob = nullptr;
        {
            // Try load compiled vertex shader file
            if (FAILED(MyUtility::ReadFileToBlob(TEXT("MyShader_VS.cso"), &pVertexShaderBlob)))
            {
                // Compile the vertex shader
                hr = MyUtility::CompileShaderFromFile(TEXT("MyShader_VS.hlsl"), "VS", "vs_4_0", &pVertexShaderBlob);
                if (FAILED(hr))
                {
                    MessageBox(nullptr, TEXT("The FX file cannot be compiled. Please run this executable from the directory that contains the FX file."), TEXT("Error"), MB_OK);
                    return hr;
                }
            }

            // Create the vertex shader
            hr = _pD3DDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &_pD3DVertexShader);
            if (FAILED(hr))
            {
                COM_RELEASE(pVertexShaderBlob);
                return hr;
            }

            // Define the input layout
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            UINT numElements = ARRAYSIZE(layout);

            // Create the input layout
            hr = _pD3DDevice->CreateInputLayout(layout, numElements, pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), &_pD3DVertexLayout);
        }
        COM_RELEASE(pVertexShaderBlob);
        if (FAILED(hr)) return hr;
    }

    // Set the input layout
    _pD3DImmediateContext->IASetInputLayout(_pD3DVertexLayout);

    // Pixel Shader
    {
        ID3DBlob* pPixelShaderBlob = nullptr;
        {
            // Try load compiled pixel shader file
            if (FAILED(MyUtility::ReadFileToBlob(TEXT("MyShader_PS.cso"), &pPixelShaderBlob)))
            {
                // Compile the pixel shader
                hr = MyUtility::CompileShaderFromFile(TEXT("MyShader_PS.hlsl"), "PS", "ps_4_0", &pPixelShaderBlob);
                if (FAILED(hr))
                {
                    MessageBox(nullptr, TEXT("The FX file cannot be compiled. Please run this executable from the directory that contains the FX file."), TEXT("Error"), MB_OK);
                    return hr;
                }
            }

            // Create the pixel shader
            hr = _pD3DDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &_pD3DPixelShader);
        }
        COM_RELEASE(pPixelShaderBlob);
        if (FAILED(hr)) return hr;
    }

    return hr;
}

HRESULT MyApp::InitGeometry()
{
    HRESULT hr;

    //            (0,1)           
    //       ┌──────▲──────┐      
    //       │      │      │      
    //       │      │(0,0) │      
    // (-1,0)◄──────┼──────►(1,0) 
    //       │      │      │      
    //       │      │      │      
    //       └──────▼──────┘      
    //            (0,-1)          

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        DirectX::XMFLOAT3(  0.0f,  0.5f,  0.0f),
        DirectX::XMFLOAT3(  0.5f, -0.5f,  0.0f),
        DirectX::XMFLOAT3( -0.5f, -0.5f,  0.0f),
    };

    D3D11_BUFFER_DESC descBufferForVertex = {};
    descBufferForVertex.Usage          = D3D11_USAGE_DEFAULT;
    descBufferForVertex.ByteWidth      = sizeof(SimpleVertex) * 3;
    descBufferForVertex.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    descBufferForVertex.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initDataForVertexBuffer = {};
    initDataForVertexBuffer.pSysMem = vertices;
    hr = _pD3DDevice->CreateBuffer(&descBufferForVertex, &initDataForVertexBuffer, &_pD3DVertexBuffer);
    if (FAILED(hr)) return hr;

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    _pD3DImmediateContext->IASetVertexBuffers(0, 1, &_pD3DVertexBuffer, &stride, &offset);

    // Set primitive topology
    _pD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return S_OK;
}

void MyApp::InputProcess()
{
}

void MyApp::UpdateProcess()
{
}

void MyApp::RenderProcess()
{
    _pD3DImmediateContext->ClearRenderTargetView(_pD3DRenderTargetView, DirectX::Colors::MidnightBlue);

    // Render a triangle
    _pD3DImmediateContext->VSSetShader(_pD3DVertexShader, nullptr, 0);
    _pD3DImmediateContext->PSSetShader(_pD3DPixelShader,  nullptr, 0);
    _pD3DImmediateContext->Draw(3, 0);

    // Present the information rendered to the back buffer to the front buffer (the screen)
    _pDXGISwapChain->Present(0, 0);
}

void MyApp::CleanupApp()
{
    if (_pD3DImmediateContext)  _pD3DImmediateContext->ClearState();


    if (_pD3DVertexBuffer)      _pD3DVertexBuffer->Release();
    if (_pD3DVertexLayout)      _pD3DVertexLayout->Release();
    if (_pD3DVertexShader)      _pD3DVertexShader->Release();
    if (_pD3DPixelShader)       _pD3DPixelShader->Release();
    if (_pD3DRenderTargetView)  _pD3DRenderTargetView->Release();
    if (_pDXGISwapChain1)       _pDXGISwapChain1->Release();
    if (_pDXGISwapChain)        _pDXGISwapChain->Release();
    if (_pD3DImmediateContext1) _pD3DImmediateContext1->Release();
    if (_pD3DImmediateContext)  _pD3DImmediateContext->Release();
    if (_pD3DDevice1)           _pD3DDevice1->Release();
    if (_pD3DDevice)            _pD3DDevice->Release();

    _hInstance = NULL;
    _hWnd      = NULL;

}
