#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	const float window_width = 1280;
	const float window_height = 720;
	float u = 1.0f / window_width;
	float v = 1.0f / window_height;
	float4 color = { 0, 0, 0, 0 };

	for (int y = 0; y < 11; y++) {
		for (int x = 0; x < 11; x++)
			color += tex.Sample(smp, input.uv + float2(u * x, v * y));
	}

	/*color += tex.Sample(smp, input.uv + float2(-u, -v));
	color += tex.Sample(smp, input.uv + float2(0, -v));
	color += tex.Sample(smp, input.uv + float2(+u, -v));

	color += tex.Sample(smp, input.uv + float2(-u, 0));
	color += tex.Sample(smp, input.uv + float2(0, 0));
	color += tex.Sample(smp, input.uv + float2(+u, 0));

	color += tex.Sample(smp, input.uv + float2(-u, +v));
	color += tex.Sample(smp, input.uv + float2(0, +v));
	color += tex.Sample(smp, input.uv + float2(+u, +v));*/

	//u�l���炵
	//float2 uv = { 0.4f, 0 };
	//float4 texcolor = tex.Sample(smp, input.uv + uv);
	//float3 color = (1, 1, 1);

	return float4(color.rbg / float(11 * 11), 1);
	//�F���]
	//return float4(color - texcolor.rgb, 1);
	//���x�ύX
	//return float4(texcolor.rbg * 2.0f, 1);
}