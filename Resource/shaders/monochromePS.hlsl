#include "monochrome.hlsli"

Texture2D<float4> tex0 : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	float4 col = tex0.Sample(smp, input.uv);
	float3 l = (col.r + col.g + col.b) / 3;

	return float4(l, 1);
}