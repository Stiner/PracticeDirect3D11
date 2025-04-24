#pragma once

class Material
{
public:
    Material() = default;
    virtual ~Material() = default;

    void Initialize();
    void Release();

    ID3DBlob* GetVertexShaderBlob() const noexcept
    {
        return _VertexShaderBlob;
    }
    ID3DBlob* GetPixelShaderBlob() const noexcept
    {
        return _PixelShaderBlob;
    }

protected:
    ID3DBlob* _VertexShaderBlob = nullptr;
    ID3DBlob* _PixelShaderBlob = nullptr;
};
