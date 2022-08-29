#include"Collision.h"

using namespace DirectX;
using namespace std;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius) { return false; }

	//擬似交点を計算
	if (inter)
	{
		//平面上の最近接点を、擬似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}

bool Collision::ClosestPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	//pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		//p0が最近傍
		*closest = triangle.p0;
		return closest;
	}

	//pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d4.m128_f32[0])
	{
		//p1が最近傍
		*closest = triangle.p1;
		return closest;
	}

	//pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - 
		d3.m128_f32[0] * d2.m128_f32[0];

	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f &&
		d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return closest;
	}

	//pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);

	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		//p0が最近傍
		*closest = triangle.p2;
		return closest;
	}

	//pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] -
		d1.m128_f32[0] * d6.m128_f32[0];

	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f &&
		d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return closest;
	}

	//pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] -
		d5.m128_f32[0] * d4.m128_f32[0];

	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f &&
		(d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) /
			((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return closest;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = XMVector3Dot(v, v);
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius) { return false; }
	//擬似交点を計算
	if (inter)
	{
		//三角形上の最近交点p0を擬似交点とする
		*inter = p;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f;	//誤差吸収用の微小な値
	//面法線とレイの方向ベクトルの内積
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//裏面には当たらない
	if (d1 > -epsilon) { return false; }
	//始点と頂点の距離(平面の法線方向)
	//面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//始点と平面の距離(平面の法線方向)
	float dist = d2 - plane.distance;
	//始点と平面の距離(レイ方向)
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if (t < 0) { return false; }
	//距離を書き込む
	if (distance) { *distance = t; }
	//交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	//三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//レイと平面が当たっていなければ、当たっていない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//レイと平面が当たっていたので、距離と交点が書き込まれた
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;	//誤差吸収用の微小な値
	XMVECTOR m;
	//辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }
	//辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }
	//辺p2_p0について
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }
	//内側なので、当たっている
	if (inter)
	{
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	//layの始点がsphereの外側にあり、(c>0)、layがsphereから離れていく方向を差している場合(b<0)、当たらない
	if (c < 0.0f && b < 0.0f) { return false; }
	float discr = b * b - c;
	//負の判定式はレイが球を外れていることに一致
	if (discr < 0.0f) { return false; }

	//レイは球と交差している
	//交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	//tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0) { t = 0.0f; }
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

bool Collision::CheckSphere2(XMFLOAT3 sphere1, XMFLOAT3 sphere2, float sphere1Radius, float sphere2Radius)
{
		float x = (sphere2.x - sphere1.x) * (sphere2.x - sphere1.x);
		float y = (sphere2.y - sphere1.y) * (sphere2.y - sphere1.y);
		float z = (sphere2.z - sphere1.z) * (sphere2.z - sphere1.z);
		float r = (sphere1Radius + sphere2Radius) * (sphere1Radius + sphere2Radius);

		if (x + y + z <= r)
		{
			return true;
		}
}

bool Collision::ColOBBs(OBB& obb1, OBB& obb2)
{
	//各ベクトルの確保
	 XMVECTOR NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	 XMVECTOR NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	 XMVECTOR NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	 XMVECTOR NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	 XMVECTOR NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	 XMVECTOR NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	 XMVECTOR Interval = obb1.GetPos_W() - obb2.GetPos_W();

	 //分離軸 : Ae1
	 float rA = XMVectorGetX(XMVector3Length(Ae1));
	 float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	 float L = fabs(XMVectorGetX(XMVector3Dot(Interval, NAe1)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : Ae2
	 rA = XMVectorGetX(XMVector3Length(Ae2));
	 rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NAe2)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : Ae3
	 rA = XMVectorGetX(XMVector3Length(Ae3));
	 rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NAe3)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : Be1
	 rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	 rB = XMVectorGetX(XMVector3Length(Be1));
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NBe1)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : Be2
	 rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	 rB = XMVectorGetX(XMVector3Length(Be2));
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NBe2)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : Be3
	 rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	 rB = XMVectorGetX(XMVector3Length(Be3));
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NBe3)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C11
	 XMVECTOR Cross;
	 XMVECTOR empty = { 0,0,0 };
	 Cross = XMVector3Cross(NAe1, NBe1);
	 rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be2, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C12
	 Cross = XMVector3Cross(NAe1, NBe2);
	 rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C13
	 Cross = XMVector3Cross(NAe1, NBe3);
	 rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be2, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C21
	 Cross = XMVector3Cross(NAe2, NBe1);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be2, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C22
	 Cross = XMVector3Cross(NAe2, NBe2);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C23
	 Cross = XMVector3Cross(NAe2, NBe3);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be2, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C31
	 Cross = XMVector3Cross(NAe3, NBe1);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be2, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C32
	 Cross = XMVector3Cross(NAe3, NBe2);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //分離軸 : C33
	 Cross = XMVector3Cross(NAe3, NBe3);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be2, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 return true;
}

float Collision::LenSegOnSeparateAxis(XMVECTOR& Sep, XMVECTOR& e1, XMVECTOR& e2, XMVECTOR& e3)
{
	//3つの内積の絶対値の和で投影線分長を計算
	//分離軸Sepは標準化されていること
	float r1 = fabs(XMVectorGetX(XMVector3Dot(Sep, e1)));
	float r2 = fabs(XMVectorGetX(XMVector3Dot(Sep, e2)));
	float r3;
	if (e3.m128_f32[0] == 0 && e3.m128_f32[1] == 0 && e3.m128_f32[2] == 0)
	{
		r3 = 0;
	}
	else {
		r3 = fabs(XMVectorGetX(XMVector3Dot(Sep, e3)));
	}
	return r1 + r2 + r3;
}
