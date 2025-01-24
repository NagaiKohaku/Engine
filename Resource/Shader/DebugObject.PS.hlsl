#include "DebugObject.hlsli"

//マテリアル
struct Material
{
    float4 color;
};

//マテリアル
ConstantBuffer<Material> gMaterial : register(b0);

//出力
struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力
    PixelShaderOutPut output;

    //マテリアルの色を設定する
    output.color = gMaterial.color;

    return output;
}