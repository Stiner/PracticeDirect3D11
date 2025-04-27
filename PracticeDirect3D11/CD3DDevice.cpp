#include "PCH.h"
#include "CD3DDevice.h"

namespace
{
    bool _IsInitialized = false;

    ID3D11Device*        _D3DDevice = nullptr;
    ID3D11DeviceContext* _D3DDeviceContext = nullptr;
    IDXGISwapChain*      _DXGISwapChain = nullptr;

    ID3D11Device1*        _D3DDevice1 = nullptr;
    ID3D11DeviceContext1* _D3DDeviceContext1 = nullptr;
    IDXGISwapChain1*      _DXGISwapChain1 = nullptr;

    D3D_DRIVER_TYPE _D3DDriverType = D3D_DRIVER_TYPE_UNKNOWN;
    D3D_FEATURE_LEVEL _D3DFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

    bool _EnableMSAA = false;
    uint32 _QualityMSAA = 0;
}

bool const& CD3DDevice::IsInitialized = _IsInitialized;

ID3D11Device*        const& CD3DDevice::Device    = _D3DDevice;
ID3D11DeviceContext* const& CD3DDevice::Context   = _D3DDeviceContext;
IDXGISwapChain*      const& CD3DDevice::SwapChain = _DXGISwapChain;

ID3D11Device1*        const& CD3DDevice::Device1    = _D3DDevice1;
ID3D11DeviceContext1* const& CD3DDevice::Context1   = _D3DDeviceContext1;
IDXGISwapChain1*      const& CD3DDevice::SwapChain1 = _DXGISwapChain1;

bool const& CD3DDevice::IsEnabledMSAA = _EnableMSAA;
uint32 const& CD3DDevice::QualityMSAA = _QualityMSAA;

HRESULT CD3DDevice::Initialize(HWND HandleWindow, uint32 ClientWidth, uint32 ClientHeight, bool IsWindowed, bool EnableMSAA, uint32 QualityMSAA)
{
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

            HRESULT hr = D3D11CreateDevice(nullptr, _D3DDriverType, nullptr, createDeviceFlags,
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

        if (_D3DDevice == nullptr || _D3DDeviceContext == nullptr)
            return E_FAIL;

        // DirectX 11.1 or later
        if (_D3DFeatureLevel == D3D_FEATURE_LEVEL_11_1)
        {
            R_CHECK(_D3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_D3DDevice1)));
            R_CHECK(_D3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_D3DDeviceContext1)));
        }
    }

    // 4x MSAA 품질 지원 확인
    {
        _EnableMSAA = EnableMSAA;
        if (_EnableMSAA)
        {
            uint32 NumQualityMSAA = 0;
            R_CHECK(_D3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &NumQualityMSAA));

            _QualityMSAA = min(NumQualityMSAA - 1, QualityMSAA - 1);
        }
    }

    // IDXGIFactory 생성해서 IDXGISwapChain 생성
    {
        HRESULT hr = S_OK;

        // ID3DDevice에서 IDXGIFactory 얻기
        IDXGIFactory1* pDXGIFactory1 = nullptr; // IDXGIFactory는 D3D10 이후부터 지원되는 DXGI 라이브러리의 주요 인터페이스. 그러므로 IDXGIFactory1을 사용.
        {
            IDXGIDevice* pDXGIDevice = nullptr;
            R_CHECK(_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice)));

            IDXGIAdapter* pDXGIAdapter = nullptr;
            hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
            COM_RELEASE(pDXGIDevice);
            if (FAILED(hr)) { assert(SUCCEEDED(hr)); return E_FAIL; }

            hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDXGIFactory1));
            COM_RELEASE(pDXGIAdapter);
            if (FAILED(hr)) { assert(SUCCEEDED(hr)); return E_FAIL; }
        }

        // D3D 버전에 맞춰 IDXGISwapChain 생성
        {
            // D3D11.1 에서는 IDXGIFactory2 인터페이스를 사용하여 스왑 체인을 생성
            IDXGIFactory2* pDXGIFactory2 = nullptr;
            hr = pDXGIFactory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pDXGIFactory2));
            if (SUCCEEDED(hr))
            {
                // DirectX 11.1 systems
                DXGI_SWAP_CHAIN_DESC1 descSwapchain = {};
                descSwapchain.Width              = ClientWidth;
                descSwapchain.Height             = ClientHeight;
                descSwapchain.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                descSwapchain.BufferCount        = 1;
                descSwapchain.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
                descSwapchain.Flags              = 0;
                descSwapchain.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
                descSwapchain.SampleDesc.Count   = (_EnableMSAA) ? 4 : 1;
                descSwapchain.SampleDesc.Quality = (_EnableMSAA) ? _QualityMSAA : 0;

                DXGI_SWAP_CHAIN_FULLSCREEN_DESC descSwapchainFullScreen = {};
                descSwapchainFullScreen.Windowed                = IsWindowed;
                descSwapchainFullScreen.RefreshRate.Numerator   = 60;
                descSwapchainFullScreen.RefreshRate.Denominator = 1;
                descSwapchainFullScreen.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
                descSwapchainFullScreen.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

                // DirectX 11.1 or later
                hr = pDXGIFactory2->CreateSwapChainForHwnd(_D3DDevice, HandleWindow,
                    &descSwapchain, &descSwapchainFullScreen, nullptr,
                    &_DXGISwapChain1);
                if (SUCCEEDED(hr))
                {
                    hr = _DXGISwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_DXGISwapChain));
                    if (SUCCEEDED(hr))
                    {
                        // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
                        hr = pDXGIFactory2->MakeWindowAssociation(HandleWindow, DXGI_MWA_NO_ALT_ENTER);
                    }
                }
                COM_RELEASE(pDXGIFactory2);
            }
            // D3D11.0 에서는 IDXGIFactory1 인터페이스를 사용하여 스왑 체인을 생성
            else
            {
                DXGI_SWAP_CHAIN_DESC sd = {};
                sd.BufferDesc.Width                   = ClientWidth;
                sd.BufferDesc.Height                  = ClientHeight;
                sd.BufferDesc.RefreshRate.Numerator   = 60;
                sd.BufferDesc.RefreshRate.Denominator = 1;
                sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
                sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
                sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
                sd.SampleDesc.Count                   = (_EnableMSAA) ? 4 : 1;
                sd.SampleDesc.Quality                 = (_EnableMSAA) ? _QualityMSAA : 0;
                sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                sd.BufferCount                        = 1;
                sd.OutputWindow                       = HandleWindow;
                sd.Windowed                           = IsWindowed;
                sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
                sd.Flags                              = 0;

                hr = pDXGIFactory1->CreateSwapChain(_D3DDevice, &sd, &_DXGISwapChain);
                if (SUCCEEDED(hr))
                {
                    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
                    hr = pDXGIFactory1->MakeWindowAssociation(HandleWindow, DXGI_MWA_NO_ALT_ENTER);
                }
            }
        }

        COM_RELEASE(pDXGIFactory1);

        R_CHECK(hr);
    }

    _IsInitialized;

    return S_OK;
}

void CD3DDevice::Release() noexcept
{
    COM_RELEASE(_D3DDevice1);
    COM_RELEASE(_D3DDevice);
    COM_RELEASE(_DXGISwapChain1);
    COM_RELEASE(_DXGISwapChain);
    COM_RELEASE(_D3DDeviceContext1);
    COM_RELEASE(_D3DDeviceContext);
}
