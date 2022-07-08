#include "WhiteNoize.hlsli"

Texture2D<float4> tex0 : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{
	float2 smpPoint = input.uv;
	float4 col = tex0.Sample(smp, smpPoint);
	float noise = frac(sin(dot(input.uv * time, float2(8.7819, 3.255))) * 437.645);
	col.rgb += float3(noise, noise, noise);
	return col;
}