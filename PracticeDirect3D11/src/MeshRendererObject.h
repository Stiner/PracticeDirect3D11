// PracticeD3D11

#pragma once

class Mesh;
class Material;

class MeshRendererObject
{
public:
    MeshRendererObject();
    virtual ~MeshRendererObject() = default;

    void Initialize(ID3D11Device* D3DDevice, Mesh* SourceMesh, Material* SourceMaterial);

protected:
    void CreateVertexShader(ID3D11Device* D3DDevice);
    void CreateInputLayout(ID3D11Device* D3DDevice, ID3DBlob* VertexShaderBlob);
    void CreatePixelShader(ID3D11Device* D3DDevice);
    void CreateRasterizerState(ID3D11Device* D3DDevice);
    void CreateConstantBuffer(ID3D11Device* D3DDevice);
    void CreateVertexIndexBuffer(ID3D11Device* D3DDevice);

public:
    void Update(float DeltaTime);
    void UpdateMatrix();
    void Draw(ID3D11DeviceContext* D3DDeviceContext) const;

    void Release();

    void SetPosition(float x, float y, float z) noexcept;
    void SetRotation(float pitch, float yaw, float roll);
    void SetScale(float x, float y, float z) noexcept;

    const DirectX::XMMATRIX* GetWorldMatrix() const noexcept;

    ID3D11InputLayout* GetInputLayout() const noexcept;

protected:
    DirectX::XMVECTOR _Position;
    DirectX::XMVECTOR _Rotation; // Quaternion
    DirectX::XMVECTOR _Scale;

    bool _IsDirty = false;

    DirectX::XMMATRIX _MatWorld;

    Mesh* _Mesh = nullptr;
    Material* _Material = nullptr;

    ID3D11InputLayout*     _D3DInputLayout     = nullptr;
    ID3D11VertexShader*    _D3DVertexShader    = nullptr;
    ID3D11PixelShader*     _D3DPixelShader     = nullptr;
    ID3D11RasterizerState* _D3DRasterizerState = nullptr;

    ID3D11Buffer* _D3DBufferVertex   = nullptr;
    ID3D11Buffer* _D3DBufferIndex    = nullptr;
    ID3D11Buffer* _D3DBufferMatWorld = nullptr;
};
