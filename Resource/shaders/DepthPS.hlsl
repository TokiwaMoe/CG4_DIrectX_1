#include "Depth.hlsli"

Texture2D<float4> tex0 : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float _Depth = 0.05;
    float _Width = 0.05;

    float4 depth = tex0.Sample(smp, input.uv);
    float4 pintColor = float4(1, 0, 0, 1);
    float4 overColor = float4(0, 0, 1, 1);
    float pint = smoothstep(0, _Width / 2, abs(depth.r - _Depth));

    return (1 - pint) * pintColor + pint * overColor;
}