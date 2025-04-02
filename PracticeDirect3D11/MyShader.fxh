//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
struct VertexInput
{
    float3 Position : POSITION;
    //float3 Normal : NORMAL;
    float4 Color : COLOR;
    //float2 TexCoord0 : TEXCOORD0;
    //float2 TexCorrd1 : TEXCOORD1;
};

struct VertexOutput
{
    //float3 Normal;
    float4 Color;
};

VertexOutput VS(VertexInput i) : SV_Position
{
    VertexOutput o;
    o.Color = i.Color;

    return o;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VertexOutput i : SV_Position) : SV_Target
{
    return i.Color;
}
