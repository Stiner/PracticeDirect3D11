//--------------------------------------------------------------------------------------
// Unlit.fx
//--------------------------------------------------------------------------------------
struct VertexInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct VertexOutput
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
cbuffer cbPerFrame : register(b0)
{
    // 뷰, 투영 행렬
    float4x4 gMatViewProj;
};

cbuffer cbPerObject : register(b1)
{
    // 월드 행렬
    float4x4 gMatWorld;
};

VertexOutput VS_main(VertexInput i)
{
    VertexOutput o;
    // 동차 절단 공간으로 변환
    // HLSL에서의 벡터는 행 우선의 1x4 행렬로 표현되고, 열 우선인 4x4 행렬과 곱할 때는 변환행렬 앞에 있어야 함.
    o.Position = mul(float4(i.Position, 1.0f), gMatWorld);
    o.Position = mul(o.Position, gMatViewProj);

    // 정점 색은 그대로 픽셀 쉐이더로 전달
    o.Color = i.Color;

    return o;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_main(VertexOutput i) : SV_Target
{
    return i.Color;
}
