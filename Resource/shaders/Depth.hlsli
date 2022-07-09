cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
	float time;
};

struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float4 worldpos : POSITION;
	float2 uv : TEXCOORD;	//uv�l
};
