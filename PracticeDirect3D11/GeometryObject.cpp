// PracticeD3D11

#include "PCH.h"
#include "GeometryObject.h"

using namespace DirectX;

GeometryObject::GeometryObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _Scale({ 0, 0, 0, 0 })
    , _matWorld(XMMatrixIdentity())
{
}

void GeometryObject::Initialize(ID3D11Device* D3DDevice)
{
    D3D11_INPUT_ELEMENT_DESC descVertex1[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    D3D11_INPUT_ELEMENT_DESC descVertex2[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,       0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ID3D11InputLayout* _D3DInputLayoutVertex1 = nullptr;

    HRESULT hr = D3DDevice->CreateInputLayout(descVertex1, ARRAYSIZE(descVertex1), nullptr, 0, &_D3DInputLayoutVertex1);
    assert(SUCCEEDED(hr));
}

void GeometryObject::Update()
{
    XMMATRIX matTranslate = XMMatrixTranslationFromVector(_Position);
    XMMATRIX matRotation  = XMMatrixRotationRollPitchYawFromVector(_Rotation);
    XMMATRIX matScale     = XMMatrixScalingFromVector(_Scale);

    _matWorld = matScale * matRotation * matTranslate;
}

void GeometryObject::Release()
{
}

void GeometryObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = { x, y, z };
}

void GeometryObject::SetRotationDegree(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(
        XMConvertToRadians(pitch),
        XMConvertToRadians(yaw),
        XMConvertToRadians(roll)
    );
}

void GeometryObject::SetRotationRadian(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);
}

void GeometryObject::SetScale(float x, float y, float z) noexcept
{
    _Scale = { x, y, z };
}

const DirectX::XMMATRIX* GeometryObject::GetWorldMatrix() const noexcept
{
    return &_matWorld;
}
