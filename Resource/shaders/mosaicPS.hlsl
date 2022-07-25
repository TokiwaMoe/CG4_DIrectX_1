#include "mosaic.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float density = 50;
	float4 col = tex0.Sample(smp, floor(input.uv *  density) / density); //x以下の最大の整数を返す

	return col;
}