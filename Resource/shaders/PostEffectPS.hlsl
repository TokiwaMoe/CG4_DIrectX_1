#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);	//1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	//ぼかし
	//const float window_width = 1280;
	//const float window_height = 720;
	//float u = 1.0f / window_width;
	//float v = 1.0f / window_height;
	////float4 color = { 0, 0, 0, 0 };

	//for (int y = 0; y < 11; y++) {
	//	for (int x = 0; x < 11; x++)
	//		colortex0 += tex0.Sample(smp, input.uv + float2(u * x, v * y));
	//}

	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = colortex1;
	}

	return float4(color.rbg, 1);
	

	//return float4(color.rbg / float(11 * 11), 1);

	//u値ずらし
	//float2 uv = { 0.4f, 0 };
	//float4 texcolor = tex.Sample(smp, input.uv + uv);
	//float3 color = (1, 1, 1);

	//色反転
	//return float4(color - texcolor.rgb, 1);
	//明度変更
	//return float4(texcolor.rbg * 2.0f, 1);
}