// Practice D3D11

#include "resource.h"
#include "WindowProcess.h"
#include "ShaderUtility.h"
#include "PracticeD3D11.h"

enum Const
{
    MAX_LOADSTRING = 100
};

struct SimpleVertex
{
    DirectX::XMFLOAT3 Pos;
};

namespace
{
    WCHAR g_szTitle[Const::MAX_LOADSTRING];
    WCHAR g_szWindowClass[Const::MAX_LOADSTRING];

    HINSTANCE g_hInstance;
    HWND      g_hWnd;

    D3D_DRIVER_TYPE         g_driverType         = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL       g_featureLevel       = D3D_FEATURE_LEVEL_11_0;
    ID3D11Device*           g_pd3dDevice         = nullptr;
    ID3D11Device1*          g_pd3dDevice1        = nullptr;
    ID3D11DeviceContext*    g_pImmediateContext  = nullptr;
    ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
    IDXGISwapChain*         g_pSwapChain         = nullptr;
    IDXGISwapChain1*        g_pSwapChain1        = nullptr;
    ID3D11RenderTargetView* g_pRenderTargetView  = nullptr;

    ID3D11VertexShader*     g_pVertexShader      = nullptr;
    ID3D11PixelShader*      g_pPixelShader       = nullptr;
    ID3D11InputLayout*      g_pVertexLayout      = nullptr;
    ID3D11Buffer*           g_pVertexBuffer      = nullptr;

    HRESULT InitWithCmd(LPWSTR lpCmdLine)
    {
        return S_OK;
    }

    HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
    {
        LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
        LoadStringW(hInstance, IDC_PRACTICEDIRECT3D11, g_szWindowClass, MAX_LOADSTRING);

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
        wcex.lpszClassName = g_szWindowClass;
        wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        if (!RegisterClassExW(&wcex))
            return E_FAIL;

        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        g_hWnd = CreateWindowW(g_szWindowClass, g_szTitle,
                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT,           // x, y
                              rc.right - rc.left, rc.bottom - rc.top  // w, h
                              , nullptr, nullptr, hInstance, nullptr);

        if (!g_hWnd)
            return E_FAIL;

        ShowWindow(g_hWnd, nCmdShow);
        UpdateWindow(g_hWnd);

        g_hInstance = hInstance;

        return S_OK;
    }

    HRESULT InitDevice()
    {
        HRESULT hr = S_OK;

        RECT rc;
        GetClientRect(g_hWnd, &rc);
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
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            g_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

            if (hr == E_INVALIDARG)
            {
                // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
                hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                    D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
            }

            if (SUCCEEDED(hr))
                break;
        }
        if (FAILED(hr))
            return hr;

        // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
        IDXGIFactory1* dxgiFactory = nullptr;
        {
            IDXGIDevice* dxgiDevice = nullptr;
            hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
            if (SUCCEEDED(hr))
            {
                IDXGIAdapter* adapter = nullptr;
                hr = dxgiDevice->GetAdapter(&adapter);
                if (SUCCEEDED(hr))
                {
                    hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                    adapter->Release();
                }
                dxgiDevice->Release();
            }
        }
        if (FAILED(hr))
            return hr;

        // Create swap chain
        IDXGIFactory2* dxgiFactory2 = nullptr;
        hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
        if (dxgiFactory2)
        {
            // DirectX 11.1 or later
            hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
            if (SUCCEEDED(hr))
            {
                (void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
            }

            DXGI_SWAP_CHAIN_DESC1 sd = {};
            sd.Width              = width;
            sd.Height             = height;
            sd.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.SampleDesc.Count   = 1;
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount        = 1;

            hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
            if (SUCCEEDED(hr))
            {
                hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
            }

            dxgiFactory2->Release();
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
            sd.OutputWindow                       = g_hWnd;
            sd.SampleDesc.Count                   = 1;
            sd.SampleDesc.Quality                 = 0;
            sd.Windowed                           = TRUE;

            hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
        }

        // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
        dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);

        dxgiFactory->Release();

        if (FAILED(hr))
            return hr;

        // Create a render target view
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (FAILED(hr))
            return hr;

        hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
        pBackBuffer->Release();
        if (FAILED(hr))
            return hr;

        g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width    = (FLOAT)width;
        vp.Height   = (FLOAT)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        g_pImmediateContext->RSSetViewports(1, &vp);

        return hr;
    }

    HRESULT InitShader()
    {
        HRESULT hr;

        // Compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;
        hr = CompileShaderFromFile(TEXT("MyShader.fxh"), "VS", "vs_4_0", &pVSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr, TEXT("The FX file cannot be compiled. Please run this executable from the directory that contains the FX file."), TEXT("Error"), MB_OK);
            return hr;
        }

        // Create the vertex shader
        hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
        if (FAILED(hr))
        {
            pVSBlob->Release();
            return hr;
        }

        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // Create the input layout
        hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
        pVSBlob->Release();
        if (FAILED(hr))
            return hr;

        // Set the input layout
        g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        hr = CompileShaderFromFile(TEXT("MyShader.fxh"), "PS", "ps_4_0", &pPSBlob);
        if (FAILED(hr))
        {
            MessageBox(nullptr, TEXT("The FX file cannot be compiled. Please run this executable from the directory that contains the FX file."), TEXT("Error"), MB_OK);
            return hr;
        }

        // Create the pixel shader
        hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
        pPSBlob->Release();
        if (FAILED(hr))
            return hr;

        return hr;
    }

    HRESULT InitGeometry()
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
            DirectX::XMFLOAT3(  0.0f,  0.0f,  0.0f),
            DirectX::XMFLOAT3(  1.0f, -1.0f,  0.0f),
            DirectX::XMFLOAT3( -1.0f, -1.0f,  0.0f),
        };

        D3D11_BUFFER_DESC bd = {};
        bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.ByteWidth      = sizeof(SimpleVertex) * 3;
        bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData = {};
        InitData.pSysMem = vertices;
        hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
        if (FAILED(hr))
            return hr;

        // Set vertex buffer
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

        // Set primitive topology
        g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        return S_OK;
    }

    void InputProcess()
    {
    }

    void UpdateProcess()
    {
    }

    void RenderProcess()
    {
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);

        // Render a triangle
        g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
        g_pImmediateContext->PSSetShader(g_pPixelShader,  nullptr, 0);
        g_pImmediateContext->Draw(3, 0);

        // Present the information rendered to the back buffer to the front buffer (the screen)
        g_pSwapChain->Present(0, 0);
    }

    void CleanupApp()
    {
        if (g_pImmediateContext)  g_pImmediateContext->ClearState();

        if (g_pVertexBuffer)      g_pVertexBuffer->Release();
        if (g_pVertexLayout)      g_pVertexLayout->Release();
        if (g_pVertexShader)      g_pVertexShader->Release();
        if (g_pPixelShader)       g_pPixelShader->Release();
        if (g_pRenderTargetView)  g_pRenderTargetView->Release();
        if (g_pSwapChain1)        g_pSwapChain1->Release();
        if (g_pSwapChain)         g_pSwapChain->Release();
        if (g_pImmediateContext1) g_pImmediateContext1->Release();
        if (g_pImmediateContext)  g_pImmediateContext->Release();
        if (g_pd3dDevice1)        g_pd3dDevice1->Release();
        if (g_pd3dDevice)         g_pd3dDevice->Release();

        g_hInstance = NULL;
        g_hWnd      = NULL;
    }
}

int RunApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
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
