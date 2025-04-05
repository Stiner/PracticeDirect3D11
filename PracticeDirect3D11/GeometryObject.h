// PracticeD3D11

#pragma once

class GeometryObject
{
public:
    GeometryObject();
    virtual ~GeometryObject() = default;

    void Initialize(ID3D11Device* D3DDevice);

    void Update();
    void Draw(ID3D11DeviceContext* D3DDeviceContext, ID3D11Buffer* D3DConstantBuffer) const;

    void Release();

    void SetPosition(float x, float y, float z) noexcept;
    void SetRotationDegree(float roll, float pitch, float yaw);
    void SetRotationRadian(float roll, float pitch, float yaw);
    void SetScale(float x, float y, float z) noexcept;

    const DirectX::XMMATRIX* GetWorldMatrix() const noexcept;

    ID3D11InputLayout* GetInputLayout() const noexcept;

protected:
    DirectX::XMVECTOR _Position;
    DirectX::XMVECTOR _Rotation;
    DirectX::XMVECTOR _Scale;

    DirectX::XMMATRIX _matWorld;

    ID3D11InputLayout* _D3DInputLayoutVertex = nullptr;
    ID3D11VertexShader* _D3DVertexShader = nullptr;
    ID3D11PixelShader* _D3DPixelShader = nullptr;
};
