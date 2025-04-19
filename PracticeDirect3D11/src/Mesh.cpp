#include "PCH.h"
#include "Mesh.h"

#include "Utility.h"

void Mesh::Load()
{
    _NumDescVertex = 2;
    _DescVertex = new D3D11_INPUT_ELEMENT_DESC[_NumDescVertex]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //*/----
    // Quad
    _NumVertex = 4;
    _Vertices = new Vertex[_NumVertex]
    {
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), Utility::Colors::White   },
        { XMFLOAT3(-1.0f,  1.0f, 0.0f), Utility::Colors::Black   },
        { XMFLOAT3( 1.0f, -1.0f, 0.0f), Utility::Colors::Red     },
        { XMFLOAT3( 1.0f,  1.0f, 0.0f), Utility::Colors::Green   },
    };

    _NumIndices = 6;
    _Indices = new uint32[_NumIndices]
    {
        0, 1, 2, 2, 1, 3,
    };
    /*/
    // Cube
    _NumVertex = 8;
    _Vertices = new Vertex[_NumVertex]
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), Utility::Colors::White   },
        { XMFLOAT3(-1.0f,  1.0f, -1.0f), Utility::Colors::Black   },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f), Utility::Colors::Red     },
        { XMFLOAT3( 1.0f,  1.0f, -1.0f), Utility::Colors::Green   },
        { XMFLOAT3(-1.0f, -1.0f,  1.0f), Utility::Colors::Blue    },
        { XMFLOAT3(-1.0f,  1.0f,  1.0f), Utility::Colors::Yellow  },
        { XMFLOAT3( 1.0f, -1.0f,  1.0f), Utility::Colors::Cyan    },
        { XMFLOAT3( 1.0f,  1.0f,  1.0f), Utility::Colors::Magenta }
    };

    _NumIndices = 36;
    _Indices = new uint32[_NumIndices]
    {
        0, 1, 2, 2, 1, 3,
        4, 5, 6, 6, 5, 7,
        0, 1, 4, 4, 1, 5,
        2, 3, 6, 6, 3, 7,
        0, 2, 4, 4, 2, 6,
        1, 3, 5, 5, 3, 7
    };
    //*/
}

void Mesh::Release() noexcept
{
    MEM_DELETE(_DescVertex);
    _NumDescVertex = 0;

    MEM_DELETE(_Vertices);
    _NumVertex = 0;

    MEM_DELETE(_Indices);
    _NumIndices = 0;
}
