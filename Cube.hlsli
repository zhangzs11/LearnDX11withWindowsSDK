// Cube.hlsli
cbuffer ConstantBuffer : register(b0)
{
    matrix g_World; // The 'matrix' can be replaced by 'float4x4'. Without 'row_major', the matrix is by default a column-major matrix,
    matrix g_View;  // 'row_major' can be added before to denote a row-major matrix.
    matrix g_Proj;  // This tutorial will use the default column-major matrix going forward, but the matrix needs to be transposed beforehand in C++ code.
    vector g_Color;
    uint g_UseCustomColor;
}

struct VertexIn
{
    float3 posL : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};
