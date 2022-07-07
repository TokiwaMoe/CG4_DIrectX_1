#include "negapozi.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 col = tex0.Sample(smp, input.uv);

	return float4(1 - col.r, 1 - col.g, 1 - col.b, 1);
}