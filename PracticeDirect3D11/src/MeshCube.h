#pragma once

#include "Mesh.h"

class MeshCube : public Mesh
{
public:
    struct Vertex
    {
        XMFLOAT3 Position;
        XMFLOAT4 Color;
    };

public:
    MeshCube() = default;
    virtual ~MeshCube() = default;

    void Initialize();
    void Release() noexcept;
};
