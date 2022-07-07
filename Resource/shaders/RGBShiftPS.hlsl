#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float shift = 0.005;
    float r = tex0.Sample(smp, input.uv + float2(-shift, 0)).r;
    float g = tex0.Sample(smp, input.uv + float2(0, 0)).g;
    float b = tex0.Sample(smp, input.uv + float2(shift, 0)).b;
    float4 col = float4(r, g, b, 1);

    return col;
}