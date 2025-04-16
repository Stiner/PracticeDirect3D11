#pragma once

#define USE_SIMPLE_VERTEX 1

class Mesh
{
public:
    struct Vertex
    {
        XMFLOAT3 Position;
        XMFLOAT4 Color;
        //XMFLOAT3 Position;
        //XMFLOAT3 Normal;
        //XMFLOAT4 Color;
        //XMFLOAT2 TexCoord0;
        //XMFLOAT2 TexCoord1;
    };

public:
    Mesh() = default;
    virtual ~Mesh() = default;

    void Load();
    void Release() noexcept;

    inline int32 GetVertexSize() const noexcept
    {
        return sizeof(Vertex);
    }

    inline int32 GetNumDescVertex() const noexcept
    {
        return _NumDescVertex;
    }

    inline const D3D11_INPUT_ELEMENT_DESC* GetDescVertex() const noexcept
    {
        return _DescVertex;
    }

    inline int32 GetNumVertex() const noexcept
    {
        return _NumVertex;
    }

    inline const Vertex* GetVertices() const noexcept
    {
        return _Vertices;
    }

    inline int32 GetIndexSize() const noexcept
    {
        return sizeof(uint32);
    }

    inline int32 GetNumIndices() const noexcept
    {
        return _NumIndices;
    }

    inline const uint32* GetIndices() const noexcept
    {
        return _Indices;
    }

protected:
    int32 _NumDescVertex = 0;
    D3D11_INPUT_ELEMENT_DESC* _DescVertex = nullptr;

    int32 _NumVertex = 0;
    Vertex* _Vertices = nullptr;

    int32 _NumIndices = 0;
    uint32* _Indices = nullptr;
};
