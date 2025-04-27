#pragma once

class CSceneObject
{
public:
    CSceneObject();
    virtual ~CSceneObject() = default;

    virtual void Initialize();

    virtual void Update(float DeltaTime);

protected:
    void CreateMatrixBuffer();

    virtual void UpdateMatrix();

    void UpdateDeviceBuffer();

protected:
    DirectX::XMVECTOR _Position;
    DirectX::XMVECTOR _Rotation; // Quaternion
    DirectX::XMVECTOR _Scale;

    DirectX::XMVECTOR _Forward;
    DirectX::XMVECTOR _Up;

    DirectX::XMMATRIX _Matrix;
    bool _IsDirty = false;

    ID3D11Buffer* _D3DBufferMatrix = nullptr;
};
