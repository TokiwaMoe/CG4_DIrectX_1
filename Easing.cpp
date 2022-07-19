#include "Easing.h"

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

void Easing::Initialize()
{
	time = 0;
}

//bool Easing::lerp(XMFLOAT3 start, XMFLOAT3 end, bool flag)
//{
//	float time = 0;
//	time += 60.0f / 3600;
//	position.x = start.x * (1.0f - time) + end.x * time;
//	position.z = start.x * (1.0f - time) + end.x * time;
//	return position;
//}
//

XMFLOAT3 Easing::easeIn(XMFLOAT3 start, XMFLOAT3 end, float flame)
{
	XMFLOAT3 c = { end.x - start.x, end.y - start.y, end.z - start.z };
	time = flame / maxflame;
	float v = easeInCubic(time);
	position.x = c.x * v + start.x;
	position.z = c.z * v + start.z;
	return position;
}

float Easing::easeInCubic(float x)
{
	return x * x * x;
}

XMFLOAT3 Easing::easeOut(XMFLOAT3 start, XMFLOAT3 end, float flame)
{
	
	XMFLOAT3 c = { end.x - start.x, end.y - start.y, end.z - start.z };
	time = flame / maxflame;
	time -= 0.1;
	float v = easeOutCubic(time);
	position.x = +c.x * v + start.x;
	position.z = +c.z * v + start.z;
	return position;
}

float Easing::easeOutCubic(float x)
{
	return x * x * x + 0.1;
}

XMFLOAT3 Easing::easeInOut(XMFLOAT3 start, XMFLOAT3 end, float flame)
{
	XMFLOAT3 c = { end.x - start.x, end.y - start.y, end.z - start.z };
	time = flame / (maxflame / 2.0f);
	time -= 0.2;
	float v = easeInOutCubic(time);

	if (time < 1)
	{
		position.x = c.x / 2.0f * v + start.x;
		position.z = c.z / 2.0f * v + start.z;
		return position;
	}

	position.x = c.x / 2.0f * (v + 0.2) + start.x;
	position.z = c.z / 2.0f * (v + 0.2) + start.z;
	return position;
}

float Easing::easeInOutCubic(float x)
{
	return x * x * x;
}
