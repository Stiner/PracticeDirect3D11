#pragma once

class GeometryObject
{
public:
    GeometryObject();
    virtual ~GeometryObject() noexcept;

    void Initialize(ID3D11Device* D3DDevice);

    void Update();

    void SetPosition(float x, float y, float z) noexcept;
    void SetRotationDegree(float roll, float pitch, float yaw);
    void SetRotationRadian(float roll, float pitch, float yaw);
    void SetScale(float x, float y, float z) noexcept;

    const DirectX::XMMATRIX* GetWorldMatrix() const noexcept;

protected:
    DirectX::XMVECTOR _Position;
    DirectX::XMVECTOR _Rotation;
    DirectX::XMVECTOR _Scale;

    DirectX::XMMATRIX _matWorld;
};
