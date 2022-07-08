#include "noize.hlsli"

Texture2D<float4> tex0 : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	/*float speed = 0.2;
	float width = 0.03;
	float power = 0.5;
	float frame = 0;
	frame += 0.1;
	float sbTime = frac(frame * speed);
	float seTime = sbTime + width;
	float uv = float2(
		input.uv.x + sin(smoothstep(sbTime, seTime, input.uv.y) * 2 * 3.141592) * power,
		input.uv.y
		);*/
	float time2 = time;
	
	float2 smpPoint = input.uv;
	float4 Tex = tex0.Sample(smp, smpPoint);
	//�G�t�F�N�g��������Ȃ���������邽�߂̌v�Z
	float sinv = sin(input.uv.y * 2 + time2 * -0.1);
	float steped = step(0.99, sinv * sinv);
	//������
	Tex.rgb -= (1 - steped) * abs(sin(input.uv.y * 50.0 + time2 * 1.0)) * 0.05;
	Tex.rgb -= (1 - steped) * abs(sin(input.uv.y * 100.0 + time2 * 2.0)) * 0.08;
	return Tex;
}