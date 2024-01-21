#include "Skybox.hlsli"

VertexPosHL VS(VertexPos vIn)
{
    VertexPosHL vOut;
    
    // set z = w , make z/w = 1 the skybox keep in the far plane
    float4 posH = mul(float4(vIn.posL, 1.0f), g_WorldViewProj);
    vOut.posH = posH.xyww;
    vOut.posL = vIn.posL;
    return vOut;
}