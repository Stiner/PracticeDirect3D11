// PracticeD3D11

#include "PCH.h"
#include "GeometryObject.h"
#include "Utility.h"

using namespace DirectX;

GeometryObject::GeometryObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _Scale({ 0, 0, 0, 0 })
    , _matWorld(XMMatrixIdentity())
    , _D3DInputLayoutVertex1(nullptr)
{
}

void GeometryObject::Initialize(ID3D11Device* D3DDevice)
{
    ID3DBlob* VertexShaderBlob = nullptr;
    if (FAILED(CUtility::ReadFileToBlob(TEXT("MyShader_VS.cso"), &VertexShaderBlob)))
    {
        HRESULT hr = CUtility::CompileShaderFromFile(TEXT("MyShader_VS.hlsl"), "VS", "vs_5_0", &VertexShaderBlob);
        assert(SUCCEEDED(hr));
    }

    D3DDevice->CreateVertexShader(
        VertexShaderBlob->GetBufferPointer(),
        VertexShaderBlob->GetBufferSize(),
        nullptr,
        &_D3DVertexShader
    );

    D3D11_INPUT_ELEMENT_DESC descVertex[] =
    //*/
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    /*/
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD1", 1, DXGI_FORMAT_R32G32_FLOAT,       0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    //*/
    int numDesc = ARRAYSIZE(descVertex);

    HRESULT hr = D3DDevice->CreateInputLayout(
        descVertex, numDesc,
        VertexShaderBlob->GetBufferPointer(),
        VertexShaderBlob->GetBufferSize(),
        &_D3DInputLayoutVertex1
    );
    assert(SUCCEEDED(hr));

    COM_RELEASE(VertexShaderBlob);
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

ID3D11InputLayout* GeometryObject::GetInputLayout() const noexcept
{
    return _D3DInputLayoutVertex1;
}
