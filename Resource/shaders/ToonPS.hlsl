#include "Toon.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
	//float light_diffuse = saturate(dot(-light, input.normal));
	//float3 shade_color;
	//shade_color = m_ambient; // �A���r�G���g��
	//shade_color += m_diffuse * light_diffuse;	// �f�B�t���[�Y��
	//float4 texcolor = tex.Sample(smp, input.uv);
	//return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	/*float4 shadecolor;
	const float shininess = 4.0f;
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 dotlightnormal = dot(lightv, input.normal);
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;*/

	//�f�B�t���[�Y
	float diffuseIntensity = saturate(dot(normalize(input.normal), lightv));
	float light = smoothstep(0.5, 0.6, diffuseIntensity);
	float4 diffuse = { texcolor.rgb * lightcolor * light, 1 };
	//�A���r�G���g
	float dark = 1 - smoothstep(0.5, 0.55, diffuseIntensity);
	float4 ambient = { texcolor.rgb * lightcolor * 0.3 * dark, 1 };
	//�X�y�L�����[
	float3 eyeDir = normalize(cameraPos.xyz - input.worldpos.xyz);
	float3 halfV = normalize(lightv + eyeDir);
	float intensity = saturate(dot(normalize(input.normal), halfV));

	float4 color = float4(lightcolor.rgb, 1);
	float reflection = pow(intensity, 100);
	float specularLight = smoothstep(0.7, 0.8, reflection);
	float4 specular = color * specularLight;

	float4 abs = ambient + diffuse + specular;

	return abs;

}