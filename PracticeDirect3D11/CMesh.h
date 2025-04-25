// PracticeD3D11

#pragma once

class CMesh
{
public:
    CMesh() = default;
    virtual ~CMesh() = default;

    virtual void Initialize() = 0;
    virtual void Release() noexcept = 0;

    uint32 GetVertexSize() const noexcept
    {
        return _VertexSize;
    }

    uint32 GetNumDescVertex() const noexcept
    {
        return _NumDescVertex;
    }

    const D3D11_INPUT_ELEMENT_DESC* GetDescVertex() const noexcept
    {
        return _DescVertex;
    }

    uint32 GetNumVertex() const noexcept
    {
        return _NumVertex;
    }

    const void* GetVertices() const noexcept
    {
        return _Vertices;
    }

    uint32 GetIndexSize() const noexcept
    {
        return _IndexSize;
    }

    uint32 GetNumIndices() const noexcept
    {
        return _NumIndices;
    }

    const void* GetIndices() const noexcept
    {
        return _Indices;
    }

protected:
    uint32 _NumDescVertex = 0;
    D3D11_INPUT_ELEMENT_DESC* _DescVertex = nullptr;

    uint32 _NumVertex = 0;
    uint32 _VertexSize = 0;
    void* _Vertices = nullptr;

    uint32 _NumIndices = 0;
    uint32 _IndexSize = 0;
    uint32* _Indices = nullptr;
};
