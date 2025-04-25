// PracticeD3D11

#pragma once

#include "CMesh.h"

class CMeshCube : public CMesh
{
public:
    struct Vertex
    {
        XMFLOAT3 Position;
        XMFLOAT4 Color;
    };

public:
    CMeshCube() = default;
    virtual ~CMeshCube() = default;

    void Initialize();
    void Release() noexcept;
};
