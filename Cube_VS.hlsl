#include "Cube.hlsli"

// Cube_VS.hlsl
VertexOut main(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.posL, 1.0f), g_World); // 'mul' is used for matrix multiplication. The operator '*' requires the operands to be
    vOut.posH = mul(vOut.posH, g_View); // two matrices with equal number of rows and columns, resulting in
    vOut.posH = mul(vOut.posH, g_Proj); // Cij = Aij * Bij
    vOut.color = vIn.color; // Here the alpha channel value defaults to 1.0
    return vOut;
}
