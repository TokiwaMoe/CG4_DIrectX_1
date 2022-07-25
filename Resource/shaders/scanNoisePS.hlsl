#include "scanNoise.hlsli"

Texture2D<float4> tex0 : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	float speed = 0.2;
	float width = 0.03;
	float power = 0.5;
	float sbTime = frac(time / 10.0f * speed);
	float seTime = sbTime + width;
	float2 uv = float2(
		input.uv.x + sin(smoothstep(sbTime, seTime, input.uv.y) * 2 * 3.141592) * power,
		input.uv.y
		);

	return tex0.Sample(smp, uv);
	
}