// PracticeD3D11

#include "PCH.h"
#include "CameraObject.h"

CameraObject::CameraObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _vecForward({ 0, 0, 1, 0 })
    , _vecUp({ 0, 1, 0, 0 })
    , _mat(XMMatrixIdentity())
    , _aspectRatio(0)
    , _fov(0)
    , _nearZ(0)
    , _farZ(0)
{
}

void CameraObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = { x, y, z };
}

void CameraObject::SetRotationDegree(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(
        XMConvertToRadians(pitch),
        XMConvertToRadians(yaw),
        XMConvertToRadians(roll)
    );

    UpdateMatrix();
}

void CameraObject::SetRotationRadian(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);

    UpdateMatrix();
}

void CameraObject::SetFov(float fov) noexcept
{
    _fov = XMConvertToRadians(fov);
}

void CameraObject::SetAspectRatio(float aspectRatio) noexcept
{
    _aspectRatio = aspectRatio;
}

void CameraObject::SetNear(float nearZ) noexcept
{
    _nearZ = nearZ;
}

void CameraObject::SetFar(float farZ) noexcept
{
    _farZ = farZ;
}

const DirectX::XMMATRIX& CameraObject::GetMatrix() const noexcept
{
    return _mat;
}

void CameraObject::UpdateMatrix()
{
    XMVECTOR vecForward = { 0, 0, 1, 0 }, vecUp = { 0, 1, 0, 0 };

    vecForward = XMVector3Rotate(vecForward, _Rotation);
    vecUp = XMVector3Rotate(vecUp, _Rotation);

    vecForward = XMVector3Normalize(vecForward);
    vecUp = XMVector3Normalize(vecUp);

    XMMATRIX matView = XMMatrixLookAtLH(_Position, vecForward, vecUp);
    XMMATRIX matProjection = XMMatrixPerspectiveFovLH(_fov, _aspectRatio, _nearZ, _farZ);

    _mat = matProjection * matView;
}
