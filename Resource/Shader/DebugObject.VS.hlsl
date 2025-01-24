#include "DebugObject.hlsli"

//���W�s��
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
};

//���W�s��
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

//����
struct VertexShaderInput
{
    float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    //�o��
    VertexShaderOutput output;

    //���W
    output.position = mul(input.position, gTransformationMatrix.WVP);

    return output;
}