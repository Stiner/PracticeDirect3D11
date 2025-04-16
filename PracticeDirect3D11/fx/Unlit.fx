//--------------------------------------------------------------------------------------
// Vertex Shader
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

cbuffer cbPerObject
{
    float4x4 gMatWorldViewProj;
};

VertexOutput VS_main(VertexInput i)
{
    VertexOutput o;
    // 동차 절단 공간으로 변환
    o.Position = mul(float4(i.Position, 1.0f), gMatWorldViewProj);
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
