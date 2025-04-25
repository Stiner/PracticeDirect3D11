// PracticeD3D11

#include "PCH.h"
#include "CCameraObject.h"
#include "Utility.h"

CameraObject::CameraObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _Forward({ 0, 0, 1, 1 })
    , _Up({ 0, 1, 0, 1 })
    , _MatView(XMMatrixIdentity())
    , _MatProj(XMMatrixIdentity())
    , _MatViewProj(XMMatrixIdentity())
{
}

void CameraObject::Initialize(ID3D11Device* D3DDevice)
{
    SetFov(90.0f);
    SetAspectRatio(1);
    SetNear(1.0f);
    SetFar(1000.0f);

    SetPosition(0, 0, 0);
    SetRotation(0, 0, 0);

    UpdateMatrix();

    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC descBufferMatViewProj = {};
    descBufferMatViewProj.Usage               = D3D11_USAGE_DEFAULT;
    descBufferMatViewProj.ByteWidth           = sizeof(DirectX::XMMATRIX);
    descBufferMatViewProj.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    descBufferMatViewProj.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descBufferMatViewProj.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descBufferMatViewProj.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA descSubResDataMatViewProj = {};
    descSubResDataMatViewProj.pSysMem          = &_MatViewProj;
    descSubResDataMatViewProj.SysMemPitch      = 0;
    descSubResDataMatViewProj.SysMemSlicePitch = 0;

    hr = D3DDevice->CreateBuffer(&descBufferMatViewProj, &descSubResDataMatViewProj, &_D3DBufferMatViewProj);
    assert(SUCCEEDED(hr));
}

void CameraObject::Update(float DeltaTime)
{
    //static float yaw = 0;
    //yaw += XMConvertToRadians(30.0f) * DeltaTime;
    //
    //SetPosition(0, 0, 3);
    //SetRotation(0, yaw, 0);

    if (_IsDirty)
    {
        UpdateMatrix();
    }
}

void CameraObject::Draw(ID3D11DeviceContext* D3DDeviceContext)
{
    // 셰이더로 넘겨줄때는 전치해서 넘겨줘야 한다.
    // DirectX::XMMATRIX는 열 우선 순서로 저장되고, 셰이더에서는 행 우선 순서로 저장되기 때문.
    XMMATRIX matViewProjTransposed = XMMatrixTranspose(_MatViewProj);
    D3DDeviceContext->UpdateSubresource(_D3DBufferMatViewProj, 0, nullptr, &matViewProjTransposed, 0, 0);
    D3DDeviceContext->VSSetConstantBuffers(0, 1, &_D3DBufferMatViewProj);
}

void CameraObject::Release()
{
	COM_RELEASE(_D3DBufferMatViewProj);
}

void CameraObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = XMVectorSet(x, y, z, 0);
}

void CameraObject::SetRotation(float pitch, float yaw, float roll)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	_IsDirty = true;
}

void CameraObject::SetFov(float fov) noexcept
{
    _fov = XMConvertToRadians(fov); // fov * (XM_PI / 180.0f);

    _IsDirty = true;
}

void CameraObject::SetAspectRatio(float aspectRatio) noexcept
{
    _aspectRatio = aspectRatio;

    _IsDirty = true;
}

void CameraObject::SetNear(float nearZ) noexcept
{
    _nearZ = nearZ;

    _IsDirty = true;
}

void CameraObject::SetFar(float farZ) noexcept
{
    _farZ = farZ;

    _IsDirty = true;
}

void CameraObject::UpdateMatrix()
{
    XMVECTOR lookDir = XMVector3Rotate(Const::Vector::BaseForward, _Rotation);
    _Forward = XMVector3Normalize(lookDir);

    XMVECTOR up = XMVector3Rotate(Const::Vector::BaseUp, _Rotation);
    _Up = XMVector3Normalize(up);

    XMMATRIX matView = XMMatrixLookToLH(_Position, _Forward, _Up);
    XMMATRIX matProj = XMMatrixPerspectiveFovLH(_fov, _aspectRatio, _nearZ, _farZ);
    _MatViewProj = matView * matProj;
}
