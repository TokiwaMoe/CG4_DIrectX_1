#include "Easing.h"

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

void Easing::Initialize()
{
	time = 0;
}

XMFLOAT3 Easing::ease(XMFLOAT3 start, XMFLOAT3 end, float flame)
{
	difference = { end.x - start.x, end.y - start.y, end.z - start.z };
	time = flame / maxflame;
	float v = Cubic_inout(time);
	position.x = difference.x * v + start.x;
	position.z = difference.z * v + start.z;
	return position;
}

float Easing::Cubic_in(float x)
{
	return x * x * x;
}

float Easing::Cubic_out(float x)
{
	return 1 - pow(1 - x, 3);
}

float Easing::Cubic_inout(float x)
{
	return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

float Easing::Sine_in(float x)
{
	return 1 - cos((x * PI) / 2);
}

float Easing::Sine_out(float x)
{
	return sin((x * PI) / 2);
}

float Easing::Sine_inout(float x)
{
	return 0.0f;
}

float Easing::Quad_in(float x)
{
	return 0.0f;
}

float Easing::Quad_out(float x)
{
	return 0.0f;
}

float Easing::Quad_inout(float x)
{
	return 0.0f;
}

float Easing::Quart_in(float x)
{
	return 0.0f;
}

float Easing::Quart_out(float x)
{
	return 0.0f;
}

float Easing::Quart_inout(float x)
{
	return 0.0f;
}

float Easing::Quint_in(float x)
{
	return 0.0f;
}

float Easing::Quint_out(float x)
{
	return 0.0f;
}

float Easing::Quint_inout(float x)
{
	return 0.0f;
}

float Easing::Expo(float x)
{
	return 0.0f;
}

float Easing::Circ(float x)
{
	return 0.0f;
}

float Easing::Back(float x)
{
	return 0.0f;
}

float Easing::Elastic(float x)
{
	return 0.0f;
}

float Easing::Bounce(float x)
{
	return 0.0f;
}
