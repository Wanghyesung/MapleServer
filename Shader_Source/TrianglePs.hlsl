struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

Texture2D smileTexture : register(t0);
//SamplerState������ ������ �ȼ� ũ�⸦ ���� �� �ۿ� ���� �ؽ����� �� �ȼ� ���̸� ��� ó���� ������ �����ϴ� State�� ���̴�. (UV��ǥ�� ��ȯ)
SamplerState samplerState : register(s0);

//���� Ÿ������
float4 main(VSOut In) : SV_TARGET
{
    //return In.Color;
    float4 color = (float) 0.f;
    //������ UV��ǥ�� �ؽ��� �����ؼ� ����
 
    color = smileTexture.Sample(samplerState, In.UV);
  
    return color;
}


