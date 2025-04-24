#include "PCH.h"
#include "MeshCubeTex.h"

void MeshCubeTex::Initialize()
{
    _NumDescVertex = 2;
    _DescVertex = new D3D11_INPUT_ELEMENT_DESC[_NumDescVertex]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    _NumVertex = 8;
    _VertexSize = sizeof(Vertex);
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
        { {-1.0f, -1.0f, -1.0f}, { 0.0f, 1.0f } }, // 0
        { {-1.0f,  1.0f, -1.0f}, { 0.0f, 0.0f } }, // 1
        { { 1.0f, -1.0f, -1.0f}, { 1.0f, 1.0f } }, // 2
        { { 1.0f,  1.0f, -1.0f}, { 1.0f, 0.0f } }, // 3
        { {-1.0f, -1.0f,  1.0f}, { 1.0f, 1.0f } }, // 4
        { {-1.0f,  1.0f,  1.0f}, { 1.0f, 0.0f } }, // 5
        { { 1.0f, -1.0f,  1.0f}, { 0.0f, 1.0f } }, // 6
        { { 1.0f,  1.0f,  1.0f}, { 0.0f, 0.0f } }, // 7
    };

    _NumIndices = 36;
    _IndexSize = sizeof(uint32);
    _Indices = new uint32[_NumIndices]
    {
        // FrontCounterClockwise = false

        4, 6, 7, 7, 5, 4, // forward
        0, 1, 3, 3, 2, 0, // back
        0, 4, 5, 5, 1, 0, // left
        2, 3, 7, 7, 6, 2, // right
        1, 5, 7, 7, 3, 1, // up
        0, 2, 6, 6, 4, 0, // bottom
    };
}

void MeshCubeTex::Release() noexcept
{
    _VertexSize = 0;

    MEM_DELETE(_DescVertex);
    _NumDescVertex = 0;

    MEM_DELETE(_Vertices);
    _NumVertex = 0;

    MEM_DELETE(_Indices);
    _NumIndices = 0;
}
