// PracticeD3D11

#include "PCH.h"
#include "MeshRendererObject.h"

#include "Mesh.h"
#include "Utility.h"

MeshRendererObject::MeshRendererObject()
    : _Position({ 0, 0, 0, 0 })
    , _Rotation({ 0, 0, 0, 0 })
    , _Scale({ 0, 0, 0, 0 })
    , _matWorld(XMMatrixIdentity())
{
}

void MeshRendererObject::Initialize(ID3D11Device* D3DDevice)
{
    _Mesh = new Mesh();
    _Mesh->Load();

    InitVertexShader(D3DDevice);
    InitPixelShader(D3DDevice);
    InitBuffer(D3DDevice);
}

void MeshRendererObject::Update(float DeltaTime)
{
    XMMATRIX matTranslate = XMMatrixTranslationFromVector(_Position);
    XMMATRIX matRotation  = XMMatrixRotationRollPitchYawFromVector(_Rotation);
    XMMATRIX matScale     = XMMatrixScalingFromVector(_Scale);

    _matWorld = matScale * matRotation * matTranslate;
}

void MeshRendererObject::Draw(ID3D11DeviceContext* D3DDeviceContext) const
{
    assert(D3DDeviceContext);

    if (_Mesh == nullptr || _Mesh->GetNumVertex() == 0)
        return;

    uint32 NumBuffers = 1;
    uint32 VertexBufferStrides[1] = { _Mesh->GetVertexSize() };
    uint32 VertexBufferOffsets[1] = { 0 };
    assert(_D3DVertexBuffer);
    D3DDeviceContext->IASetVertexBuffers(0, NumBuffers, &_D3DVertexBuffer, VertexBufferStrides, VertexBufferOffsets);

    assert(_D3DIndexBuffer);
    D3DDeviceContext->IASetIndexBuffer(_D3DIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    assert(_D3DInputLayout);
    D3DDeviceContext->IASetInputLayout(_D3DInputLayout);

    assert(_D3DVertexShader);
    D3DDeviceContext->VSSetShader(_D3DVertexShader, nullptr, 0);

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

    COM_RELEASE(_D3DVertexBuffer);
    COM_RELEASE(_D3DIndexBuffer);
    COM_RELEASE(_D3DConstantBuffer);

    _Mesh->Release();
    MEM_DELETE(_Mesh);
}

void MeshRendererObject::SetPosition(float x, float y, float z) noexcept
{
    _Position = { x, y, z };
}

void MeshRendererObject::SetRotationDegree(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(
        XMConvertToRadians(pitch),
        XMConvertToRadians(yaw),
        XMConvertToRadians(roll)
    );
}

void MeshRendererObject::SetRotationRadian(float roll, float pitch, float yaw)
{
    _Rotation = XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);
}

void MeshRendererObject::SetScale(float x, float y, float z) noexcept
{
    _Scale = { x, y, z };
}

const DirectX::XMMATRIX* MeshRendererObject::GetWorldMatrix() const noexcept
{
    return &_matWorld;
}

ID3D11InputLayout* MeshRendererObject::GetInputLayout() const noexcept
{
    return _D3DInputLayout;
}

void MeshRendererObject::InitVertexShader(ID3D11Device* D3DDevice)
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

void MeshRendererObject::InitPixelShader(ID3D11Device* D3DDevice)
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

void MeshRendererObject::InitBuffer(ID3D11Device* D3DDevice)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC descVertexBuffer = {};
    descVertexBuffer.Usage               = D3D11_USAGE_DEFAULT;
    descVertexBuffer.ByteWidth           = _Mesh->GetVertexSize() * _Mesh->GetNumVertex();
    descVertexBuffer.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    descVertexBuffer.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descVertexBuffer.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descVertexBuffer.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA descVertexSubResData = {};
    descVertexSubResData.pSysMem          = _Mesh->GetVertices();
    descVertexSubResData.SysMemPitch      = 0;
    descVertexSubResData.SysMemSlicePitch = 0;

    hr = D3DDevice->CreateBuffer(&descVertexBuffer, &descVertexSubResData, &_D3DVertexBuffer);
    assert(SUCCEEDED(hr));

    D3D11_BUFFER_DESC descIndexBuffer = {};
    descIndexBuffer.Usage               = D3D11_USAGE_DEFAULT;
    descIndexBuffer.ByteWidth           = _Mesh->GetIndexSize() * _Mesh->GetNumIndices();
    descIndexBuffer.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    descIndexBuffer.CPUAccessFlags      = (D3D11_CPU_ACCESS_FLAG)0;
    descIndexBuffer.MiscFlags           = (D3D11_RESOURCE_MISC_FLAG)0;
    descIndexBuffer.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA descIndexSubResDataIndex = {};
    descIndexSubResDataIndex.pSysMem          = _Mesh->GetIndices();
    descIndexSubResDataIndex.SysMemPitch      = 0;
    descIndexSubResDataIndex.SysMemSlicePitch = 0;

    hr = D3DDevice->CreateBuffer(&descIndexBuffer, &descIndexSubResDataIndex, &_D3DIndexBuffer);
    assert(SUCCEEDED(hr));
}

void MeshRendererObject::InitRasterizerState(ID3D11Device* D3DDevice)
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
