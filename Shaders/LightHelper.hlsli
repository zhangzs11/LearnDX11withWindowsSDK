
struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};

struct PointLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
    float range;

    float3 att;
    float pad;
};

struct SpotLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
    float range;

    float3 direction;
    float Spot;

    float3 att;
    float pad;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular; // w = SpecPower
    float4 reflect;
};



void ComputeDirectionalLight(Material mat, DirectionalLight L,
    float3 normal, float3 toEye,
    out float4 ambient,
    out float4 diffuse,
    out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightVec = -L.direction;

    ambient = mat.ambient * L.ambient;

    float diffuseFactor = dot(lightVec, normal);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        spec = specFactor * mat.specular * L.specular;
    }
}


void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightVec = L.position - pos;

    float d = length(lightVec);

    if (d > L.range)
        return;

    lightVec /= d;

    ambient = mat.ambient * L.ambient;

    float diffuseFactor = dot(lightVec, normal);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        spec = specFactor * mat.specular * L.specular;
    }

    float att = 1.0f / dot(L.att, float3(1.0f, d, d * d));

    diffuse *= att;
    spec *= att;
}


void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightVec = L.position - pos;

    float d = length(lightVec);

    if (d > L.range)
        return;

    lightVec /= d;

    ambient = mat.ambient * L.ambient;

    float diffuseFactor = dot(lightVec, normal);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        spec = specFactor * mat.specular * L.specular;
    }

    float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.Spot);
    float att = spot / dot(L.att, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}

float3 NormalSampleToWorldSpace(float3 normalMapSample,
    float3 unitNormalW,
    float4 tangentW)
{
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    float3 N = unitNormalW;
    float3 T = normalize(tangentW.xyz - dot(tangentW.xyz, N) * N); // Schmidt Orthogonalization
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    // Transforming the normal vertors from tangent space to world space
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

static const float SAMP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SAMP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow,
                       Texture2D shadowMap,
                       float4 shadowPosH,
                       float depthBias)
{
    //透视除法
    shadowPosH.xyz /= shadowPosH.w;
    
    //NDC空间的深度值
    float depth = shadowPosH.z - depthBias;
    
    //纹素在纹理坐标下的宽高
    const float dx = SMAP_DX;
    
    float percentLit = 0.0f;//属于阴影的百分比
    
    // samShadow为compareValue <= sampleValue时为1.0f(反之为0.0f), 对相邻四个纹素进行采样比较
    // 并根据采样点位置进行双线性插值
    // float result0 = depth <= s0;  // .s0      .s1          
    // float result1 = depth <= s1;
    // float result2 = depth <= s2;  //     .depth
    // float result3 = depth <= s3;  // .s2      .s3
    // float result = BilinearLerp(result0, result1, result2, result3, a, b);  // a b为算出的插值相对位置 
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow,
            shadowPosH.xy, depth, int2(i % 3 - 1, i / 3 - 1)).r;
    }
    
    return percentLit /= 9.0f;
}

