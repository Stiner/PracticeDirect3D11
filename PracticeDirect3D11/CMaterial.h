// PracticeD3D11

#pragma once

class CMaterial
{
public:
    CMaterial() = default;
    virtual ~CMaterial() = default;

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
    void CreateInputLayout();
    void CreateVertexShader();
    void CreatePixelShader();

protected:
    ID3DBlob* _VertexShaderBlob = nullptr;
    ID3DBlob* _PixelShaderBlob = nullptr;
};
