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
	position.y = difference.y * v + start.y;
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
	return -(cos(PI * x) - 1) / 2;
}

float Easing::Quad_in(float x)
{
	return x * x;
}

float Easing::Quad_out(float x)
{
	return 1 - (1 - x) * (1 - x);
}

float Easing::Quad_inout(float x)
{
	return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}

float Easing::Quart_in(float x)
{
	return x * x * x * x;
}

float Easing::Quart_out(float x)
{
	return 1 - pow(1 - x, 4);
}

float Easing::Quart_inout(float x)
{
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

float Easing::Quint_in(float x)
{
	return x * x * x * x * x;
}

float Easing::Quint_out(float x)
{
	return 1 - pow(1 - x, 5);
}

float Easing::Quint_inout(float x)
{
	return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}

float Easing::Expo_in(float x)
{
	return x == 0 ? 0 : pow(2, 10 * x - 10);
}

float Easing::Expo_out(float x)
{
	return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

float Easing::Expo_inout(float x)
{
	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5 ? pow(2, 20 * x - 10) / 2
		: (2 - pow(2, -20 * x + 10)) / 2;
}

float Easing::Circ_in(float x)
{
	return 1 - sqrt(1 - pow(x, 2));
}

float Easing::Circ_out(float x)
{
	return sqrt(1 - pow(x - 1, 2));
}

float Easing::Circ_inout(float x)
{
	return x < 0.5
		? (1 - sqrt(1 - pow(2 * x, 2))) / 2
		: (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
}

float Easing::Back_in(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

float Easing::Back_out(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

float Easing::Back_inout(float x)
{
	const float c1 = 1.70158;
	const float c2 = c1 * 1.525;

	return x < 0.5
		? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float Easing::Elastic_in(float x)
{
	const float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

float Easing::Elastic_out(float x)
{
	const float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}

float Easing::Elastic_inout(float x)
{
	const float c5 = (2 * PI) / 4.5;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
		: (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
}

float Easing::Bounce_in(float x)
{
	return 1 - Bounce_out(1 - x);
}

float Easing::Bounce_out(float x)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

float Easing::Bounce_inout(float x)
{
	return x < 0.5
		? (1 - Bounce_out(1 - 2 * x)) / 2
		: (1 + Bounce_out(2 * x - 1)) / 2;
}
