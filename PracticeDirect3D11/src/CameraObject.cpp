// PracticeD3D11

#include "PCH.h"
#include "CameraObject.h"

CameraObject::CameraObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _Forward({ 0, 0, 1, 0 })
    , _Up({ 0, 1, 0, 0 })
    , _MatViewProj(XMMatrixIdentity())
{
}

void CameraObject::Initialize(ID3D11Device* D3DDevice)
{
    SetFov(90.0f);
    SetAspectRatio(1);
    SetNear(0.1f);
    SetFar(1000.0f);

    SetPosition(0, 0, -5);
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
    if (_IsDirty)
    {
        UpdateMatrix();
        _IsDirty = false;
    }
}

void CameraObject::Draw(ID3D11DeviceContext* D3DDeviceContext)
{
    D3DDeviceContext->UpdateSubresource(_D3DBufferMatViewProj, 0, nullptr, &_MatViewProj, 0, 0);
    D3DDeviceContext->VSSetConstantBuffers(0, 1, &_D3DBufferMatViewProj);
}

void CameraObject::Release()
{
	COM_RELEASE(_D3DBufferMatViewProj);
}

void CameraObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = { x, y, z };
}

void CameraObject::SetRotation(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);

	_IsDirty = true;
}

void CameraObject::SetFov(float fov) noexcept
{
    _fov = XMConvertToRadians(fov);

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

const DirectX::XMMATRIX& CameraObject::GetMatrix() const noexcept
{
    return _MatViewProj;
}

void CameraObject::UpdateMatrix()
{
    XMVECTOR vecForward = { 0, 0, 1, 0 }, vecUp = { 0, 1, 0, 0 };

    vecForward = XMVector3Rotate(vecForward, _Rotation);
    vecUp = XMVector3Rotate(vecUp, _Rotation);

    vecForward = XMVector3Normalize(vecForward);
    vecUp = XMVector3Normalize(vecUp);

    XMMATRIX matView = XMMatrixLookToLH(_Position, vecForward, vecUp);
    XMMATRIX matProjection = XMMatrixPerspectiveFovLH(_fov, _aspectRatio, _nearZ, _farZ);

    _MatViewProj = matProjection * matView;
}
