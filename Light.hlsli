#include "LightHelper.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
    matrix g_World;
    matrix g_View;
    matrix g_Proj;
    matrix g_WorldInvTranspose;
}

cbuffer PSConstantBuffer : register(b1)
{
    DirectionalLight g_DirLight;
    PointLight g_PointLight;
    SpotLight g_SpotLight;
    Material g_Material;
    float3 g_EyePosW;
    float g_Pad;
}



struct VertexIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float4 color : COLOR;
};