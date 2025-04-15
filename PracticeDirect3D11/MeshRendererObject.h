// PracticeD3D11

#pragma once

class Mesh;

class MeshRendererObject
{
public:
    MeshRendererObject();
    virtual ~MeshRendererObject() = default;

    void Initialize(ID3D11Device* D3DDevice);

    void Update(float DeltaTime);
    void Draw(ID3D11DeviceContext* D3DDeviceContext) const;

    void Release();

    void SetPosition(float x, float y, float z) noexcept;
    void SetRotationDegree(float roll, float pitch, float yaw);
    void SetRotationRadian(float roll, float pitch, float yaw);
    void SetScale(float x, float y, float z) noexcept;

    const DirectX::XMMATRIX* GetWorldMatrix() const noexcept;

    ID3D11InputLayout* GetInputLayout() const noexcept;

protected:
    virtual void InitVertexShader(ID3D11Device* D3DDevice);
    virtual void InitInputLayout(ID3D11Device* D3DDevice, ID3DBlob* VertexShaderBlob);
    virtual void InitPixelShader(ID3D11Device* D3DDevice);
    virtual void InitBuffer(ID3D11Device* D3DDevice);
    virtual void InitRasterizerState(ID3D11Device* D3DDevice);

protected:
    DirectX::XMVECTOR _Position;
    DirectX::XMVECTOR _Rotation;
    DirectX::XMVECTOR _Scale;

    DirectX::XMMATRIX _matWorld;

    Mesh* _Mesh = nullptr;

    ID3D11InputLayout* _D3DInputLayout = nullptr;
    ID3D11VertexShader* _D3DVertexShader = nullptr;
    ID3D11PixelShader* _D3DPixelShader = nullptr;
    ID3D11RasterizerState* _D3DRasterizerState = nullptr;

    ID3D11Buffer* _D3DVertexBuffer = nullptr;
    ID3D11Buffer* _D3DIndexBuffer = nullptr;
    ID3D11Buffer* _D3DConstantBuffer = nullptr;
};
