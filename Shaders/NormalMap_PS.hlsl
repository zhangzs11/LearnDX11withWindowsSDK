#include "Basic.hlsli"

float4 PS(VertexPosHWNormalTangentTex pIn) : SV_Target
{
    float4 texColor = g_DiffuseMap.Sample(g_Sam, pIn.tex);
    clip(texColor.a - 0.1f);
    
    pIn.normalW = normalize(pIn.normalW);
    pIn.tangentW.xyz = normalize(pIn.tangentW.xyz);
    
    float3 toEyeW = normalize(g_EyePosW - pIn.posW);
    float distToEye = distance(g_EyePosW, pIn.posW);
    
    //normal map
    float3 normalMapSample = g_NormalMap.Sample(g_Sam, pIn.tex).rgb;
    float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pIn.normalW, pIn.tangentW);
    
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;

    [unroll]
    for (i = 0; i < 5; ++i)
    {
        ComputeDirectionalLight(g_Material, g_DirLight[i], bumpedNormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
        
    [unroll]
    for (i = 0; i < 5; ++i)
    {
        ComputePointLight(g_Material, g_PointLight[i], pIn.posW, bumpedNormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }

    [unroll]
    for (i = 0; i < 5; ++i)
    {
        ComputeSpotLight(g_Material, g_SpotLight[i], pIn.posW, bumpedNormalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
  
    
    float4 litColor = texColor * (ambient + diffuse) + spec;
    //Reflection
    if (g_ReflectionEnabled)
    {
        float3 incident = -toEyeW;
        float3 reflectionVector = reflect(incident, pIn.normalW);
        float4 reflectionColor = g_TexCube.Sample(g_Sam, reflectionVector);
        
        litColor += g_Material.reflect * reflectionColor;
    }
    //Refraction
    if (g_RefractionEnabled)
    {
        float3 incident = -toEyeW;
        float3 refractionVector = refract(incident, pIn.normalW, g_Eta);
        float4 refractionColor = g_TexCube.Sample(g_Sam, refractionVector);
        
        litColor += g_Material.reflect * refractionColor;
    }
    
    litColor.a = texColor.a * g_Material.diffuse.a;
    return litColor;
}