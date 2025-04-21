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

    /*/----
    // Quad
    _NumVertex = 4;
    _Vertices = new Vertex[_NumVertex]
    {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), Utility::Colors::White },
        { XMFLOAT3(-1.0f,  1.0f, -1.0f), Utility::Colors::Black },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f), Utility::Colors::Red   },
        { XMFLOAT3( 1.0f,  1.0f, -1.0f), Utility::Colors::Green },
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
        //                                  
        //      (+y)  (+z)       5-------7  
        //        | /          / |     / |  
        // (-x)---+---(+x)   1-------3   |  
        //      / |          |   4---|---6  
        // (-z)  (-y)        | /     | /    
        //                   0-------2      
        //                                  
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), static_cast<XMFLOAT4>(DirectX::Colors::White  ) }, // 0
        { XMFLOAT3(-1.0f,  1.0f, -1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Black  ) }, // 1
        { XMFLOAT3( 1.0f, -1.0f, -1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Red    ) }, // 2
        { XMFLOAT3( 1.0f,  1.0f, -1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Green  ) }, // 3
        { XMFLOAT3(-1.0f, -1.0f,  1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Blue   ) }, // 4
        { XMFLOAT3(-1.0f,  1.0f,  1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Yellow ) }, // 5
        { XMFLOAT3( 1.0f, -1.0f,  1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Cyan   ) }, // 6
        { XMFLOAT3( 1.0f,  1.0f,  1.0f), static_cast<XMFLOAT4>(DirectX::Colors::Magenta) }, // 7
    };

    _NumIndices = 36;
    _Indices = new uint32[_NumIndices]
    {
        // FrontCounterClockwise = false

        4, 6, 7, 7, 5, 4, // front

        0, 1, 3, 3, 2, 0, // back

        0, 4, 5, 5, 1, 0, // left

        2, 3, 7, 7, 6, 2, // right

        1, 5, 7, 7, 3, 1, // up

        0, 2, 6, 6, 4, 0, // bottom
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
