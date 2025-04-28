#include "PCH.h"
#include "CSceneObject.h"

#include "Utility.h"
#include "CD3DDevice.h"

CSceneObject::CSceneObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _Scale({ 1, 1, 1, 0 })
    , _Forward({ 0, 0, 1, 1 })
    , _Up({ 0, 1, 0, 1 })
    , _Matrix(XMMatrixIdentity())
{
}

void CSceneObject::Initialize()
{
    CreateMatrixBuffer();
}

void CSceneObject::Update(float DeltaTime)
{
    if (_IsDirty)
    {
        UpdateMatrix();
        UpdateDeviceBuffer();
    }
}

void CSceneObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = XMVectorSet(x, y, z, 0);

    _IsDirty = true;
}

void CSceneObject::SetRotation(float pitch, float yaw, float roll) noexcept
{
    _Rotation = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

    _IsDirty = true;
}

void CSceneObject::SetScale(float x, float y, float z) noexcept
{
    _Scale = XMVectorSet(x, y, z, 0);

    _IsDirty = true;
}

void CSceneObject::CreateMatrixBuffer()
{
    D3D11_BUFFER_DESC descBuffer = {};
    D3D11_SUBRESOURCE_DATA descSubResData = {};

    descBuffer.Usage               = D3D11_USAGE_DEFAULT;
    descBuffer.ByteWidth           = sizeof(DirectX::XMMATRIX);
    descBuffer.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    descBuffer.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descBuffer.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descBuffer.StructureByteStride = 0;

    descSubResData.pSysMem          = &_Matrix;
    descSubResData.SysMemPitch      = 0;
    descSubResData.SysMemSlicePitch = 0;

    CHECK(CD3DDevice::Device->CreateBuffer(&descBuffer, &descSubResData, &_D3DBufferMatrix));
}

void CSceneObject::UpdateMatrix()
{
    XMVECTOR lookDir = XMVector3Rotate(Const::Vector::BaseForward, _Rotation);
    _Forward = XMVector3Normalize(lookDir);

    XMVECTOR up = XMVector3Rotate(Const::Vector::BaseUp, _Rotation);
    _Up = XMVector3Normalize(up);
}

void CSceneObject::UpdateDeviceBuffer()
{
    // 셰이더로 넘겨줄때는 전치해서 넘겨줘야 한다.
    // DirectX::XMMATRIX는 열 우선 순서로 저장되고, 셰이더에서는 행 우선 순서로 저장되기 때문.
    XMMATRIX matWorldTransposed = XMMatrixTranspose(_Matrix);
    CD3DDevice::Context->UpdateSubresource(_D3DBufferMatrix, 0, nullptr, &matWorldTransposed, 0, 0);
}
