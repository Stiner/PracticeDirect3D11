#pragma once

#include "Mesh.h"

class MeshCubeTex : public Mesh
{
public:
    struct Vertex
    {
        XMFLOAT3 Position;
        XMFLOAT2 TexCoord0;
    };

public:
    MeshCubeTex() = default;
    virtual ~MeshCubeTex() = default;

    void Initialize();
    void Release() noexcept;
};
