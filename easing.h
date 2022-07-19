#pragma once
#include <DirectXMath.h>
#include"DirectXCommon.h"

class Easing
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	void Initialize();
	XMFLOAT3 ease(XMFLOAT3 start, XMFLOAT3 end, float time);
	float Cubic_in(float x);
	float Cubic_out(float x);
	float Cubic_inout(float x);
	float Sine_in(float x);
	float Sine_out(float x);
	float Sine_inout(float x);
	float Quad_in(float x);
	float Quad_out(float x);
	float Quad_inout(float x);
	float Quart_in(float x);
	float Quart_out(float x);
	float Quart_inout(float x);
	float Quint_in(float x);
	float Quint_out(float x);
	float Quint_inout(float x);
	float Expo(float x);
	float Circ(float x);
	float Back(float x);
	float Elastic(float x);
	float Bounce(float x);

public:
	XMFLOAT3 position = { 0,0,0 };
	float time = 0;
	const float maxflame = 1.0f;
	const float PI = 3.141592;
	XMFLOAT3 difference = { 0,0,0 };
};