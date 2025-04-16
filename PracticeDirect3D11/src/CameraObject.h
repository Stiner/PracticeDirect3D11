// PracticeD3D11

#pragma once

class CameraObject
{
public:
    CameraObject();
    ~CameraObject() = default;

	void Initialize(ID3D11Device* D3DDevice);

	void Update(float DeltaTime);
    void Draw(ID3D11DeviceContext* D3DDeviceContext);

    void Release();

    void SetPosition(float x, float y, float z) noexcept;
    void SetRotationDegree(float roll, float pitch, float yaw);
    void SetRotationRadian(float roll, float pitch, float yaw);

    void SetFov(float fov) noexcept;
    void SetAspectRatio(float aspectRatio) noexcept;
    void SetNear(float nearZ) noexcept;
    void SetFar(float farZ) noexcept;

    const DirectX::XMMATRIX& GetMatrix() const noexcept;

protected:
    void UpdateMatrix();

protected:
    DirectX::XMVECTOR _Position;
    DirectX::XMVECTOR _Rotation;

    DirectX::XMVECTOR _Forward;
    DirectX::XMVECTOR _Up;

    DirectX::XMMATRIX _MatViewProj;

    float _fov         = 0;
    float _aspectRatio = 0;
    float _nearZ       = 0;
    float _farZ        = 0;

	ID3D11Buffer* _D3DBufferMatViewProj = nullptr;

	bool _IsDirty = false;
};
