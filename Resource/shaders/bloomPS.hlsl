#include "bloom.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー


float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}/*

float4 main(VSOutput input) : SV_TARGET
{
	float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
	float4 col = float4(0, 0, 0, 0);

	for (float py = _Sigma; py <= _Sigma * 2; py += _StepWidth)
	{
		for (float px = _Sigma; px <= _Sigma * 2; px += _StepWidth)
		{
			float2 pickUV = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pickUV, _Sigma);
			col += tex0.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	col.rgb = col.rgb / totalWeight;
	return col;
}*/

float4 main(VSOutput input) : SV_TARGET
{
	float4 col = tex0.Sample(smp, input.uv);
	float grayScale = col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
	float extract = smoothstep(0.6, 0.9, grayScale);
	return float4(col.rgb * extract, 1);
}