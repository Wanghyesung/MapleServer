#include "globals.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

//Texture2D albedoTexture : register(t0);
////SamplerState������ ������ �ȼ� ũ�⸦ ���� �� �ۿ� ���� �ؽ����� �� �ȼ� ���̸� ��� ó���� ������ �����ϴ� State�� ���̴�. (UV��ǥ�� ��ȯ)
//SamplerState pointSampler : register(s0);
//SamplerState anisotropicSampler : register(s1);

//���� Ÿ������
float4 main(VSOut In) : SV_TARGET
{
    //return In.Color;
    float4 color = (float4) 0.f;
    //������ UV��ǥ�� �ؽ��� �����ؼ� ����
    //cos(100.0f);
    
    //dot()
    float fRatio = Size.x / 100.f;
    if (In.UV.x > fRatio)
        discard;
    
    color = albedoTexture.Sample(pointSampler, In.UV);
    
    if (color.a <= 0.0f)
        discard;
    
    return color;
}