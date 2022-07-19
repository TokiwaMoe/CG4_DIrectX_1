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
	XMFLOAT3 lerp(XMFLOAT3 start, XMFLOAT3 end, float time);
	XMFLOAT3 easeIn(XMFLOAT3 start, XMFLOAT3 end, float time);
	float easeInCubic(float x);
	XMFLOAT3 easeOut(XMFLOAT3 start, XMFLOAT3 end, float time);
	float easeOutCubic(float x);
	XMFLOAT3 easeInOut(XMFLOAT3 start, XMFLOAT3 end, float time);
	float easeInOutCubic(float x);
public:
	XMFLOAT3 position = { 0,0,0 };
	float time = 0;
	const float maxflame = 1.0f;
	const float PI = 3.141592;
};
