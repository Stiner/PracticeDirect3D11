// PracticeD3D11

#pragma once

class CameraObject
{
public:
    CameraObject();
    ~CameraObject() = default;

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

    DirectX::XMVECTOR _vecForward;
    DirectX::XMVECTOR _vecUp;

    DirectX::XMMATRIX _mat;

    float _fov;
    float _aspectRatio;
    float _nearZ;
    float _farZ;
};
