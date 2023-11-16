#include "Cube.hlsli"

// Cube_PS.hlsl
// ÏñËØ×ÅÉ«Æ÷
float4 main(VertexOut pIn) : SV_Target
{
    return g_UseCustomColor ? g_Color : pIn.color;
    //return g_Color;

}
