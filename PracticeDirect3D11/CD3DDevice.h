#pragma once

class CD3DDevice
{
public:
    static void Initialize();

    static inline ID3D11Device* Get() noexcept;
};
