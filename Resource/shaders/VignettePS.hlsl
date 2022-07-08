#include "Vignette.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{
	float2 smpPoint = input.uv;
	float4 col = tex0.Sample(smp, smpPoint);
	float vignette = length(float2(0.5, 0.5) - input.uv);
	vignette = clamp(vignette - 0.2, 0, 1); //xがmin以下ならmin, max以上ならmax
	col.gb -= vignette;
	return col;
}