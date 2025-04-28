#pragma once

class CSceneObject
{
public:
    CSceneObject();
    virtual ~CSceneObject() = default;

    virtual void Initialize();

    virtual void Update(float DeltaTime);

    virtual void Draw();

    virtual void Release();

    virtual void SetPosition(float x, float y, float z) noexcept;
    virtual void SetRotation(float pitch, float yaw, float roll) noexcept;
    virtual void SetScale(float x, float y, float z) noexcept;

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
    bool _DoCalcMatrix = false;

    ID3D11Buffer* _D3DBufferMatrix = nullptr;
};
