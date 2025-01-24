#include "DebugObject.hlsli"

//�}�e���A��
struct Material
{
    float4 color;
};

//�}�e���A��
ConstantBuffer<Material> gMaterial : register(b0);

//�o��
struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutput input)
{
    //�o��
    PixelShaderOutPut output;

    //�}�e���A���̐F��ݒ肷��
    output.color = gMaterial.color;

    return output;
}