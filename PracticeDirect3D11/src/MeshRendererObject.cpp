// PracticeD3D11

#include "PCH.h"
#include "MeshRendererObject.h"

#include "Mesh.h"
#include "Utility.h"

MeshRendererObject::MeshRendererObject()
    : _Position({ 0, 0, 0, 1 })
    , _Rotation({ 0, 0, 0, 1 })
    , _Scale({ 1, 1, 1, 1 })
    , _MatWorld(XMMatrixIdentity())
{
}

void MeshRendererObject::Initialize(ID3D11Device* D3DDevice)
{
    _Mesh = new Mesh();
    _Mesh->Load();

    CreateVertexShader(D3DDevice);
    CreatePixelShader(D3DDevice);
    CreateBuffer(D3DDevice);
	CreateRasterizerState(D3DDevice);
}

void MeshRendererObject::CreateVertexShader(ID3D11Device* D3DDevice)
{
    HRESULT hr = S_OK;

    ID3DBlob* VertexShaderBlob = nullptr;
    if (FAILED(Utility::ReadFileToBlob(TEXT("Unlit_VS.cso"), &VertexShaderBlob)))
    {
        HRESULT hr = Utility::CompileShaderFromFile(TEXT("Unlit_VS.hlsl"), "VS_main", "vs_5_0", &VertexShaderBlob);
        assert(SUCCEEDED(hr));
    }

    D3DDevice->CreateVertexShader(
        VertexShaderBlob->GetBufferPointer(),
        VertexShaderBlob->GetBufferSize(),
        nullptr,
        &_D3DVertexShader
    );

    InitInputLayout(D3DDevice, VertexShaderBlob);

    COM_RELEASE(VertexShaderBlob);
}

void MeshRendererObject::InitInputLayout(ID3D11Device* D3DDevice, ID3DBlob* VertexShaderBlob)
{
    HRESULT hr = S_OK;

    hr = D3DDevice->CreateInputLayout(
        _Mesh->GetDescVertex(), _Mesh->GetNumDescVertex(),
        VertexShaderBlob->GetBufferPointer(),
        VertexShaderBlob->GetBufferSize(),
        &_D3DInputLayout
    );
    assert(SUCCEEDED(hr));
}

void MeshRendererObject::CreatePixelShader(ID3D11Device* D3DDevice)
{
    HRESULT hr = S_OK;

    ID3DBlob* PixelShaderBlob = nullptr;
    if (FAILED(Utility::ReadFileToBlob(TEXT("Unlit_PS.cso"), &PixelShaderBlob)))
    {
        hr = Utility::CompileShaderFromFile(TEXT("Unlit_PS.hlsl"), "PS_main", "ps_5_0", &PixelShaderBlob);
        assert(SUCCEEDED(hr));
    }

    hr = D3DDevice->CreatePixelShader(
        PixelShaderBlob->GetBufferPointer(),
        PixelShaderBlob->GetBufferSize(),
        nullptr,
        &_D3DPixelShader
    );
    assert(SUCCEEDED(hr));

    COM_RELEASE(PixelShaderBlob);
}

void MeshRendererObject::CreateBuffer(ID3D11Device* D3DDevice)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC descBufferVertex = {};
    descBufferVertex.Usage               = D3D11_USAGE_DEFAULT;
    descBufferVertex.ByteWidth           = _Mesh->GetVertexSize() * _Mesh->GetNumVertex();
    descBufferVertex.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    descBufferVertex.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descBufferVertex.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descBufferVertex.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA descSubResDataVertex = {};
    descSubResDataVertex.pSysMem          = _Mesh->GetVertices();
    descSubResDataVertex.SysMemPitch      = 0;
    descSubResDataVertex.SysMemSlicePitch = 0;

    hr = D3DDevice->CreateBuffer(&descBufferVertex, &descSubResDataVertex, &_D3DBufferVertex);
    assert(SUCCEEDED(hr));

    D3D11_BUFFER_DESC descBufferIndex = {};
    descBufferIndex.Usage               = D3D11_USAGE_DEFAULT;
    descBufferIndex.ByteWidth           = _Mesh->GetIndexSize() * _Mesh->GetNumIndices();
    descBufferIndex.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    descBufferIndex.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descBufferIndex.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descBufferIndex.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA descSubResDataIndex = {};
    descSubResDataIndex.pSysMem          = _Mesh->GetIndices();
    descSubResDataIndex.SysMemPitch      = 0;
    descSubResDataIndex.SysMemSlicePitch = 0;

    hr = D3DDevice->CreateBuffer(&descBufferIndex, &descSubResDataIndex, &_D3DBufferIndex);
    assert(SUCCEEDED(hr));

    D3D11_BUFFER_DESC descBufferMatWorld = {};
    descBufferMatWorld.Usage               = D3D11_USAGE_DEFAULT;
    descBufferMatWorld.ByteWidth           = sizeof(DirectX::XMMATRIX);
    descBufferMatWorld.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    descBufferMatWorld.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descBufferMatWorld.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descBufferMatWorld.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA descSubResDataMatWorld = {};
    descSubResDataMatWorld.pSysMem          = &_MatWorld;
    descSubResDataMatWorld.SysMemPitch      = 0;
    descSubResDataMatWorld.SysMemSlicePitch = 0;

    hr = D3DDevice->CreateBuffer(&descBufferMatWorld, &descSubResDataMatWorld, &_D3DBufferMatWorld);
    assert(SUCCEEDED(hr));
}

void MeshRendererObject::CreateRasterizerState(ID3D11Device* D3DDevice)
{
    HRESULT hr = S_OK;

    D3D11_RASTERIZER_DESC descRasterizer = {};
    descRasterizer.FillMode              = D3D11_FILL_SOLID;
    descRasterizer.CullMode              = D3D11_CULL_BACK;
    descRasterizer.FrontCounterClockwise = false;
    descRasterizer.DepthBias             = D3D11_DEFAULT_DEPTH_BIAS;
    descRasterizer.DepthBiasClamp        = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
    descRasterizer.SlopeScaledDepthBias  = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    descRasterizer.DepthClipEnable       = true;
    descRasterizer.ScissorEnable         = false;
    descRasterizer.MultisampleEnable     = false;
    descRasterizer.AntialiasedLineEnable = false;

    hr = D3DDevice->CreateRasterizerState(&descRasterizer, &_D3DRasterizerState);
    assert(SUCCEEDED(hr));
}

void MeshRendererObject::Update(float DeltaTime)
{
    static float e = 0;

    e += DeltaTime;
    float x = sinf(e);

    SetPosition(x, 0, 0);

    UpdateMatrix();
}

void MeshRendererObject::UpdateMatrix()
{
    if (!_IsDirty)
        return;

    XMMATRIX matTranslate = XMMatrixTranslationFromVector(_Position);
    XMMATRIX matRotation = XMMatrixRotationRollPitchYawFromVector(_Rotation);
    XMMATRIX matScale = XMMatrixScalingFromVector(_Scale);

    _MatWorld = matScale * matRotation * matTranslate;
}

void MeshRendererObject::Draw(ID3D11DeviceContext* D3DDeviceContext) const
{
    assert(D3DDeviceContext);

    if (_Mesh == nullptr || _Mesh->GetNumVertex() == 0)
        return;

    D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    assert(_D3DInputLayout);
    D3DDeviceContext->IASetInputLayout(_D3DInputLayout);

    uint32 NumBuffers = 1;
    uint32 VertexBufferStrides[1] = { _Mesh->GetVertexSize() };
    uint32 VertexBufferOffsets[1] = { 0 };
    assert(_D3DBufferVertex);
    D3DDeviceContext->IASetVertexBuffers(0, NumBuffers, &_D3DBufferVertex, VertexBufferStrides, VertexBufferOffsets);

    assert(_D3DBufferIndex);
    D3DDeviceContext->IASetIndexBuffer(_D3DBufferIndex, DXGI_FORMAT_R32_UINT, 0);

    assert(_D3DVertexShader);
    D3DDeviceContext->VSSetShader(_D3DVertexShader, nullptr, 0);

    assert(_D3DBufferMatWorld);
    D3DDeviceContext->UpdateSubresource(_D3DBufferMatWorld, 0, nullptr, &_MatWorld, 0, 0);
    D3DDeviceContext->VSSetConstantBuffers(1, 1, &_D3DBufferMatWorld);

    assert(_D3DPixelShader);
    D3DDeviceContext->PSSetShader(_D3DPixelShader, nullptr, 0);

    assert(_D3DRasterizerState);
    D3DDeviceContext->RSSetState(_D3DRasterizerState);

    D3DDeviceContext->DrawIndexed(_Mesh->GetNumIndices(), 0, 0);
}

void MeshRendererObject::Release()
{
    COM_RELEASE(_D3DInputLayout);
    COM_RELEASE(_D3DVertexShader);
    COM_RELEASE(_D3DPixelShader);
    COM_RELEASE(_D3DRasterizerState);

    COM_RELEASE(_D3DBufferVertex);
    COM_RELEASE(_D3DBufferIndex);
    COM_RELEASE(_D3DBufferMatWorld);

    _Mesh->Release();
    MEM_DELETE(_Mesh);
}

void MeshRendererObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = { x, y, z, 1 };

    _IsDirty = true;
}

void MeshRendererObject::SetRotation(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);

    _IsDirty = true;
}

void MeshRendererObject::SetScale(float x, float y, float z) noexcept
{
    _Scale = { x, y, z };

    _IsDirty = true;
}

const DirectX::XMMATRIX* MeshRendererObject::GetWorldMatrix() const noexcept
{
    return &_MatWorld;
}

ID3D11InputLayout* MeshRendererObject::GetInputLayout() const noexcept
{
    return _D3DInputLayout;
}
