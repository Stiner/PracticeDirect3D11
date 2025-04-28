// PracticeD3D11

#include "PCH.h"
#include "CCameraObject.h"

#include "CD3DDevice.h"
#include "Utility.h"

CCameraObject::CCameraObject()
    : CSceneObject()
{
}

void CCameraObject::Initialize()
{
    SetFov(90.0f);
    SetAspectRatio(1);
    SetNear(1.0f);
    SetFar(1000.0f);

    SetPosition(0, 0, 0);
    SetRotation(0, 0, 0);

    __super::Initialize();
}

void CCameraObject::Update(float DeltaTime)
{
    //static float yaw = 0;
    //yaw += XMConvertToRadians(30.0f) * DeltaTime;
    //
    //SetPosition(0, 0, 3);
    //SetRotation(0, yaw, 0);

    __super::Update(DeltaTime);
}

void CCameraObject::Draw()
{
    CD3DDevice::Context->VSSetConstantBuffers(0, 1, &_D3DBufferMatrix);
}

void CCameraObject::Release()
{
	COM_RELEASE(_D3DBufferMatrix);
}

void CCameraObject::SetScale(float x, float y, float z) noexcept
{
    // do nothing
}

void CCameraObject::SetFov(float fov) noexcept
{
    _fov = XMConvertToRadians(fov); // fov * (XM_PI / 180.0f);

    _DoCalcMatrix = true;
}

void CCameraObject::SetAspectRatio(float aspectRatio) noexcept
{
    _aspectRatio = aspectRatio;

    _DoCalcMatrix = true;
}

void CCameraObject::SetNear(float nearZ) noexcept
{
    _nearZ = nearZ;

    _DoCalcMatrix = true;
}

void CCameraObject::SetFar(float farZ) noexcept
{
    _farZ = farZ;

    _DoCalcMatrix = true;
}

void CCameraObject::UpdateMatrix()
{
    __super::UpdateMatrix();

    XMVECTOR lookDir = XMVector3Rotate(Const::Vector::BaseForward, _Rotation);
    _Forward = XMVector3Normalize(lookDir);

    XMVECTOR up = XMVector3Rotate(Const::Vector::BaseUp, _Rotation);
    _Up = XMVector3Normalize(up);

    XMMATRIX matView = XMMatrixLookToLH(_Position, _Forward, _Up);
    XMMATRIX matProj = XMMatrixPerspectiveFovLH(_fov, _aspectRatio, _nearZ, _farZ);
    _Matrix = matView * matProj;
}
