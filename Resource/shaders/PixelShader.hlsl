#include "Header.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex0.Sample(smp, input.uv) * color;
	float4 col = { 0.5f, 0,0,1 };
	return float4(col.rgb, texcolor.a);

}