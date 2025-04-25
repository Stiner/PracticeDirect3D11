// PracticeD3D11

#pragma once

#include "CMesh.h"

class CMeshCubeTex : public CMesh
{
public:
    struct Vertex
    {
        XMFLOAT3 Position;
        XMFLOAT2 TexCoord0;
    };

public:
    CMeshCubeTex() = default;
    virtual ~CMeshCubeTex() = default;

    void Initialize();
    void Release() noexcept;
};
