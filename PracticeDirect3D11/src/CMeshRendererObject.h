// PracticeD3D11

#pragma once

#include "CSceneObject.h"

class CMesh;
class CMaterial;

class CMeshRendererObject : public CSceneObject
{
public:
    CMeshRendererObject();
    virtual ~CMeshRendererObject() = default;

    void Initialize(CMesh* SourceMesh, CMaterial* SourceMaterial);

    virtual void Update(float DeltaTime) override;

    virtual void Draw() override;

    virtual void Release() override;

protected:
    void CreateVertexShader();
    void CreateInputLayout(ID3DBlob* VertexShaderBlob);
    void CreatePixelShader();
    void CreateRasterizerState();
    void CreateVertexIndexBuffer();

    virtual void UpdateMatrix() override;

protected:
    CMesh* _Mesh = nullptr;
    CMaterial* _Material = nullptr;

    ID3D11InputLayout*     _D3DInputLayout     = nullptr;
    ID3D11VertexShader*    _D3DVertexShader    = nullptr;
    ID3D11PixelShader*     _D3DPixelShader     = nullptr;
    ID3D11RasterizerState* _D3DRasterizerState = nullptr;

    ID3D11Buffer* _D3DBufferVertex   = nullptr;
    ID3D11Buffer* _D3DBufferIndex    = nullptr;
};
