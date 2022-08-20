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
	XMFLOAT3 lerp(XMFLOAT3 start, XMFLOAT3 end, float flame);
	XMFLOAT3 ease(XMFLOAT3 start, XMFLOAT3 end, float flame);
	XMFLOAT3 easeOut(XMFLOAT3 start, XMFLOAT3 end, float flame);
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
	float Expo_in(float x);
	float Expo_out(float x);
	float Expo_inout(float x);
	float Circ_in(float x);
	float Circ_out(float x);
	float Circ_inout(float x);
	float Back_in(float x);
	float Back_out(float x);
	float Back_inout(float x);
	float Elastic_in(float x);
	float Elastic_out(float x);
	float Elastic_inout(float x);
	float Bounce_in(float x);
	float Bounce_out(float x);
	float Bounce_inout(float x);

public:
	XMFLOAT3 position = { 0,0,0 };
	float time = 0;
	const float maxflame = 1.0f;
	const float PI = 3.141592;
	XMFLOAT3 difference = { 0,0,0 };
};
