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
    //_NumDescVertex = 5;
    //_DescVertex = new D3D11_INPUT_ELEMENT_DESC[_NumDescVertex]
    //{
    //    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //    { "TEXCOORD1", 1, DXGI_FORMAT_R32G32_FLOAT,       0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //};

    _NumVertex = 3;
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
        0, 1, 2, //삼각형0
        0, 2, 3, //삼각형1
        0, 3, 4, //삼각형2
        0, 4, 5, //삼각형3
        0, 5, 6, //삼각형4
        0, 6, 7, //삼각형5
        0, 7, 8, //삼각형6
        0, 8, 1, //삼각형7
    };
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
