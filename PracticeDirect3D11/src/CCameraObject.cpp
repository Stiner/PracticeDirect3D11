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
    __super::Update(DeltaTime);
}

void CCameraObject::Draw()
{
    // 여기서 계산된 행렬은 ViewProjection 이기 때문에
    // CSceneObject::Draw()에서 쉐이더의 World 슬롯에 셋팅하는 처리는 스킵
    //__super::Draw();

    assert(_D3DBufferMatrix);

    CD3DDevice::Context->VSSetConstantBuffers(0, 1, &_D3DBufferMatrix);
}

void CCameraObject::Release()
{
    __super::Release();
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
    // 여기서 계산되는 행렬은 ViewProjection 이기 때문에
    // CSceneObject::UpdateMatrix()에서 계산되는 World 는 스킵
    //__super::UpdateMatrix();

    XMVECTOR lookDir = XMVector3Rotate(Const::Vector::BaseForward, _Rotation);
    _Forward = XMVector3Normalize(lookDir);

    XMVECTOR up = XMVector3Rotate(Const::Vector::BaseUp, _Rotation);
    _Up = XMVector3Normalize(up);

    XMMATRIX matView = XMMatrixLookToLH(_Position, _Forward, _Up);
    XMMATRIX matProj = XMMatrixPerspectiveFovLH(_fov, _aspectRatio, _nearZ, _farZ);
    _Matrix = matView * matProj;
}
