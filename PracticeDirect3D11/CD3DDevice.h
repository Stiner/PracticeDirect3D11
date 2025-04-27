#pragma once

class CD3DDevice final
{
public:
    static HRESULT Initialize(HWND hMainWindow, uint32 ClientWidth, uint32 ClientHeight, bool IsWindowed, bool EnableMSAA, uint32 QualityMSAA);
    static void Release() noexcept;

    static bool const& IsInitialized;

    static ID3D11Device* const& Device;
    static ID3D11DeviceContext* const& Context;
    static IDXGISwapChain* const& SwapChain;

    static ID3D11Device1* const& Device1;
    static ID3D11DeviceContext1* const& Context1;
    static IDXGISwapChain1* const& SwapChain1;

    static bool const& IsEnabledMSAA;
    static uint32 const& QualityMSAA;
};
