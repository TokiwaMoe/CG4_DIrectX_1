cbuffer cbuff0 : register(b0)
{
	/*float time;*/
	float4 color;
};

struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 uv : TEXCOORD;	//uv�l
};
