#include "object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(t0);

ConstantBuffer<DirectionalLight> gDirectionLight : register(b1);

ConstantBuffer<Camera> gCamera : register(b2);

struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

float SchkicjFresnel(float u, float f0, float f90)
{
    return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

PixelShaderOutPut main(VertexShaderOutput input)
{
    PixelShaderOutPut output;
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    if (textureColor.a == 0.0)
    {
        discard;
    }
    
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    if (gMaterial.enableLighting != 0)
    {
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 halfVector = normalize(-gDirectionLight.direction + toEye);
        
        float NdotL = dot(normalize(input.normal), -gDirectionLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess);
        
        float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gDirectionLight.color.rgb * cos * gDirectionLight.intensity;

        float3 specular =
        gDirectionLight.color.rgb * gDirectionLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
 
    return output;
}