// PracticeD3D11

#include "PCH.h"
#include "CMeshRendererObject.h"

#include "CD3DDevice.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "Utility.h"

CMeshRendererObject::CMeshRendererObject()
    : CSceneObject()
{
}

void CMeshRendererObject::Initialize(CMesh* SourceMesh, CMaterial* SourceMaterial)
{
    if (SourceMesh == nullptr)
        return;

    _Mesh = SourceMesh;
    _Material = SourceMaterial;

    CreateVertexShader();
    CreatePixelShader();
	CreateRasterizerState();
    CreateVertexIndexBuffer();

    __super::Initialize();
}

void CMeshRendererObject::Update(float DeltaTime)
{
    static float angle = 0;
    angle += XMConvertToRadians(30.f) * DeltaTime;

    SetRotation(angle, angle, 0);

    __super::Update(DeltaTime);
}

void CMeshRendererObject::Draw() const
{
    assert(CD3DDevice::Context);
    assert(_D3DInputLayout);
    assert(_D3DBufferVertex);
    assert(_D3DBufferIndex);
    assert(_D3DVertexShader);
    assert(_D3DBufferMatrix);
    assert(_D3DPixelShader);
    assert(_D3DRasterizerState);

    if (_Mesh == nullptr || _Mesh->GetNumVertex() == 0)
        return;

    CD3DDevice::Context->IASetInputLayout(_D3DInputLayout);

    uint32 NumBuffers = 1;
    uint32 VertexBufferStrides[1] = { _Mesh->GetVertexSize() };
    uint32 VertexBufferOffsets[1] = { 0 };
    CD3DDevice::Context->IASetVertexBuffers(0, NumBuffers, &_D3DBufferVertex, VertexBufferStrides, VertexBufferOffsets);

    CD3DDevice::Context->IASetIndexBuffer(_D3DBufferIndex, DXGI_FORMAT_R32_UINT, 0);

    CD3DDevice::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CD3DDevice::Context->VSSetShader(_D3DVertexShader, nullptr, 0);

    CD3DDevice::Context->VSSetConstantBuffers(1, 1, &_D3DBufferMatrix);

    CD3DDevice::Context->PSSetShader(_D3DPixelShader, nullptr, 0);

    CD3DDevice::Context->RSSetState(_D3DRasterizerState);

    CD3DDevice::Context->DrawIndexed(_Mesh->GetNumIndices(), 0, 0);
}

void CMeshRendererObject::Release()
{
    COM_RELEASE(_D3DInputLayout);
    COM_RELEASE(_D3DVertexShader);
    COM_RELEASE(_D3DPixelShader);
    COM_RELEASE(_D3DRasterizerState);

    COM_RELEASE(_D3DBufferVertex);
    COM_RELEASE(_D3DBufferIndex);
    COM_RELEASE(_D3DBufferMatrix);
}

void CMeshRendererObject::CreateVertexShader()
{
    HRESULT hr = S_OK;

    ID3DBlob* VertexShaderBlob = nullptr;
    if (FAILED(Shader::ReadFileToBlob(TEXT("Unlit_VS.cso"), &VertexShaderBlob)))
    {
        HRESULT hr = Shader::CompileShaderFromFile(TEXT("Unlit_VS.hlsl"), "VS_main", "vs_5_0", &VertexShaderBlob);
        assert(SUCCEEDED(hr));
    }

    CD3DDevice::Device->CreateVertexShader(
        VertexShaderBlob->GetBufferPointer(),
        VertexShaderBlob->GetBufferSize(),
        nullptr,
        &_D3DVertexShader
    );

    CreateInputLayout(VertexShaderBlob);

    COM_RELEASE(VertexShaderBlob);
}

void CMeshRendererObject::CreateInputLayout(ID3DBlob* VertexShaderBlob)
{
    HRESULT hr = S_OK;

    hr = CD3DDevice::Device->CreateInputLayout(
        _Mesh->GetDescVertex(), _Mesh->GetNumDescVertex(),
        VertexShaderBlob->GetBufferPointer(),
        VertexShaderBlob->GetBufferSize(),
        &_D3DInputLayout
    );
    assert(SUCCEEDED(hr));
}

void CMeshRendererObject::CreatePixelShader()
{
    HRESULT hr = S_OK;

    ID3DBlob* PixelShaderBlob = nullptr;
    if (FAILED(Shader::ReadFileToBlob(TEXT("Unlit_PS.cso"), &PixelShaderBlob)))
    {
        hr = Shader::CompileShaderFromFile(TEXT("Unlit_PS.hlsl"), "PS_main", "ps_5_0", &PixelShaderBlob);
        assert(SUCCEEDED(hr));
    }

    hr = CD3DDevice::Device->CreatePixelShader(
        PixelShaderBlob->GetBufferPointer(),
        PixelShaderBlob->GetBufferSize(),
        nullptr,
        &_D3DPixelShader
    );
    assert(SUCCEEDED(hr));

    COM_RELEASE(PixelShaderBlob);
}

void CMeshRendererObject::CreateRasterizerState()
{
    HRESULT hr = S_OK;

    D3D11_RASTERIZER_DESC descRasterizer = {};
    descRasterizer.FillMode = D3D11_FILL_SOLID;
    descRasterizer.CullMode = D3D11_CULL_BACK;
    descRasterizer.FrontCounterClockwise = false;
    descRasterizer.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
    descRasterizer.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
    descRasterizer.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    descRasterizer.DepthClipEnable = true;
    descRasterizer.ScissorEnable = false;
    descRasterizer.MultisampleEnable = false;
    descRasterizer.AntialiasedLineEnable = false;

    hr = CD3DDevice::Device->CreateRasterizerState(&descRasterizer, &_D3DRasterizerState);
    assert(SUCCEEDED(hr));
}

void CMeshRendererObject::CreateVertexIndexBuffer()
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC descBuffer = {};
    D3D11_SUBRESOURCE_DATA descSubResData = {};

    {
        descBuffer.Usage = D3D11_USAGE_DEFAULT;
        descBuffer.ByteWidth = _Mesh->GetVertexSize() * _Mesh->GetNumVertex();
        descBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        descBuffer.CPUAccessFlags = (D3D11_CPU_ACCESS_FLAG)0;
        descBuffer.MiscFlags = (D3D11_RESOURCE_MISC_FLAG)0;
        descBuffer.StructureByteStride = 0;

        descSubResData.pSysMem = _Mesh->GetVertices();
        descSubResData.SysMemPitch = 0;
        descSubResData.SysMemSlicePitch = 0;

        hr = CD3DDevice::Device->CreateBuffer(&descBuffer, &descSubResData, &_D3DBufferVertex);
        assert(SUCCEEDED(hr));
    }

    descBuffer = {}; descSubResData = {};

    {
        descBuffer.Usage = D3D11_USAGE_DEFAULT;
        descBuffer.ByteWidth = _Mesh->GetIndexSize() * _Mesh->GetNumIndices();
        descBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
        descBuffer.CPUAccessFlags = (D3D11_CPU_ACCESS_FLAG)0;
        descBuffer.MiscFlags = (D3D11_RESOURCE_MISC_FLAG)0;
        descBuffer.StructureByteStride = 0;

        descSubResData.pSysMem = _Mesh->GetIndices();
        descSubResData.SysMemPitch = 0;
        descSubResData.SysMemSlicePitch = 0;

        hr = CD3DDevice::Device->CreateBuffer(&descBuffer, &descSubResData, &_D3DBufferIndex);
        assert(SUCCEEDED(hr));
    }
}

void CMeshRendererObject::UpdateMatrix()
{
    __super::UpdateMatrix();

    XMMATRIX matTranslate = XMMatrixTranslationFromVector(_Position);
    XMMATRIX matRotation = XMMatrixRotationQuaternion(_Rotation);
    XMMATRIX matScale = XMMatrixScalingFromVector(_Scale);

    _Matrix = matScale * matRotation * matTranslate;
}
