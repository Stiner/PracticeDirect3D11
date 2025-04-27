// PracticeD3D11

#pragma once

#include "CMesh.h"

class CMeshCube final : public CMesh
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

    virtual void Initialize() final;
    virtual void Release() noexcept final;
};
