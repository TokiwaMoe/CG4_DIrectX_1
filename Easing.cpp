#include "easing.h"

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

const XMFLOAT3 Easing::lerp(XMFLOAT3 start, XMFLOAT3 end)
{
	float time = 0;
	time += 60.0f / 3600;
	position.x = start.x * (1.0f - time) + end.x * time;
	position.z = start.x * (1.0f - time) + end.x * time;
	return position;
}

const XMFLOAT3 Easing::easeIn(XMFLOAT3 start, XMFLOAT3 end)
{
	float time = 0;
	time += 60.0f / 3600;
	position.x = start.x * (1.0f - time * time) + end.x * time * time;
	position.z = start.z * (1.0f - time * time) + end.z * time * time;
	return position;
}

const XMFLOAT3 Easing::easeOut(XMFLOAT3 start, XMFLOAT3 end)
{
	float time = 0;
	time += 60.0f / 3600;
	position.x = start.x * (1.0f - time * (2 - time)) + end.x * time * (2 - time);
	position.z = start.z * (1.0f - time * (2 - time)) + end.z * time * (2 - time);
	return position;
}

const XMFLOAT3 Easing::easeInOut(XMFLOAT3 start, XMFLOAT3 end)
{
	float time = 0;
	time += 60.0f / 3600;
	position.x = start.x * (1.0f - time * time * (3 - 2 * time)) + end.x * time * time * (3 - 2 * time);
	position.z = start.z * (1.0f - time * time * (3 - 2 * time)) + end.z * time * time * (3 - 2 * time);
	return position;
}
