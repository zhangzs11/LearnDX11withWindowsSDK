// Directional Light
struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad; // Padding to make the struct size a multiple of 16 bytes for HLSL compatibility
};

// Point Light
struct PointLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
    float range; // Light radius

    float3 att; // Attenuation coefficients
    float pad; // Padding to make the struct size a multiple of 16 bytes for HLSL compatibility
};

// Spot Light
struct SpotLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
    float range; // Light radius

    float3 direction;
    float Spot; // Spotlight exponent

    float3 att; // Attenuation coefficients
    float pad; // Padding to make the struct size a multiple of 16 bytes for HLSL compatibility
};

// Material properties
struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular; // w component = Specular Power
    float4 reflect;
};

void ComputeDirectionalLight(Material mat, DirectionalLight L, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 spec)
{
    // Initialize outputs
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Calculate the light vector opposite to light direction
    float3 lightVec = -L.direction;
    
    // Compute ambient color
    ambient = mat.ambient * L.ambient;
    
    // Calculate diffuse light intensity
    float diffuseFactor = dot(lightVec, normal);
    
    // Conditional for dynamic branching support
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
    // Initialize outputs
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Calculate vector from point to light source
    float3 lightVec = L.position - pos;
    
    // Distance to the light source
    float d = length(lightVec);
    
    // Check if within light range
    if (d > L.range)
        return;
    
    // Normalize light vector
    lightVec /= d;
    
    // Compute ambient color
    ambient = mat.ambient * L.ambient;
    
    // Calculate diffuse light intensity
    float diffuseFactor = dot(lightVec, normal);
    
    // Conditional for dynamic branching support
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
        
        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        spec = specFactor * mat.specular * L.specular;
    }
    
    // Compute attenuation
    float att = 1.0f / dot(L.att, float3(1.0f, d, d * d));
    
    diffuse *= att;
    spec *= att;
}

void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 spec)
{
    // Initialize outputs
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Calculate vector from point to light source
    float3 lightVec = L.position - pos;
    
    // Distance to the light source
    float d = length(lightVec);
    
    // Check if within light range
    if (d > L.range)
        return;
    
    // Normalize light vector
    lightVec /= d;
    
    // Compute ambient color
    ambient = mat.ambient * L.ambient;
    
    // Calculate diffuse light intensity
    float diffuseFactor = dot(lightVec, normal);
    
    // Conditional for dynamic branching support
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
        
        diffuse = diffuseFactor * mat.diffuse * L.diffuse;
        spec = specFactor * mat.specular * L.specular;
    }
    
    // Compute spotlight effect and attenuation
    float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.Spot);
    float att = spot / dot(L.att, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    spec *= att;
}
