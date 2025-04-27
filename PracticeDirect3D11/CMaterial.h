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
    ID3D11InputLayout* GetInputLayout() const noexcept
    {
        return _InputLayout;
    }
    ID3DBlob* GetPixelShaderBlob() const noexcept
    {
        return _PixelShaderBlob;
    }

protected:
    void CreateVertexShader();
    void CreateInputLayout();
    void CreatePixelShader();

protected:
    ID3D11InputLayout* _InputLayout = nullptr;

    ID3DBlob* _VertexShaderBlob = nullptr;
    ID3DBlob* _PixelShaderBlob = nullptr;
};
