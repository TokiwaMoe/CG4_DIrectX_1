#include"Collision.h"

using namespace DirectX;
using namespace std;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) { return false; }

	//�[����_���v�Z
	if (inter)
	{
		//���ʏ�̍ŋߐړ_���A�[����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}

bool Collision::ClosestPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	//point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		//p0���ŋߖT
		*closest = triangle.p0;
		return closest;
	}

	//point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d4.m128_f32[0])
	{
		//p1���ŋߖT
		*closest = triangle.p1;
		return closest;
	}

	//point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - 
		d3.m128_f32[0] * d2.m128_f32[0];

	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f &&
		d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return closest;
	}

	//point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);

	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		//p0���ŋߖT
		*closest = triangle.p2;
		return closest;
	}

	//point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] -
		d1.m128_f32[0] * d6.m128_f32[0];

	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f &&
		d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return closest;
	}

	//point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
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
	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPoint2Triangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius) { return false; }
	//�[����_���v�Z
	if (inter)
	{
		//�O�p�`��̍ŋߌ�_p0���[����_�Ƃ���
		*inter = p;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f;	//�덷�z���p�̔����Ȓl
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return false; }
	//�n�_�ƒ��_�̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0) { return false; }
	//��������������
	if (distance) { *distance = t; }
	//��_���v�Z
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	//�O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;	//�덷�z���p�̔����Ȓl
	XMVECTOR m;
	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }
	//��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }
	//��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }
	//�����Ȃ̂ŁA�������Ă���
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
	//lay�̎n�_��sphere�̊O���ɂ���A(c>0)�Alay��sphere���痣��Ă��������������Ă���ꍇ(b<0)�A������Ȃ�
	if (c < 0.0f && b < 0.0f) { return false; }
	float discr = b * b - c;
	//���̔��莮�̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) { return false; }

	//���C�͋��ƌ������Ă���
	//��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	//t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
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
	//�e�x�N�g���̊m��
	 XMVECTOR NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	 XMVECTOR NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	 XMVECTOR NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	 XMVECTOR NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	 XMVECTOR NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	 XMVECTOR NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	 XMVECTOR Interval = obb1.GetPos_W() - obb2.GetPos_W();

	 //������ : Ae1
	 float rA = XMVectorGetX(XMVector3Length(Ae1));
	 float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	 float L = fabs(XMVectorGetX(XMVector3Dot(Interval, NAe1)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : Ae2
	 rA = XMVectorGetX(XMVector3Length(Ae2));
	 rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NAe2)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : Ae3
	 rA = XMVectorGetX(XMVector3Length(Ae3));
	 rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NAe3)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : Be1
	 rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	 rB = XMVectorGetX(XMVector3Length(Be1));
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NBe1)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : Be2
	 rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	 rB = XMVectorGetX(XMVector3Length(Be2));
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NBe2)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : Be3
	 rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	 rB = XMVectorGetX(XMVector3Length(Be3));
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, NBe3)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C11
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

	 //������ : C12
	 Cross = XMVector3Cross(NAe1, NBe2);
	 rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C13
	 Cross = XMVector3Cross(NAe1, NBe3);
	 rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be2, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C21
	 Cross = XMVector3Cross(NAe2, NBe1);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be2, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C22
	 Cross = XMVector3Cross(NAe2, NBe2);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C23
	 Cross = XMVector3Cross(NAe2, NBe3);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be2, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C31
	 Cross = XMVector3Cross(NAe3, NBe1);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be2, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C32
	 Cross = XMVector3Cross(NAe3, NBe2);
	 rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2, empty);
	 rB = LenSegOnSeparateAxis(Cross, Be1, Be3, empty);
	 L = fabs(XMVectorGetX(XMVector3Dot(Interval, Cross)));
	 if (L > rA + rB)
	 {
		 return false;
	 }

	 //������ : C33
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
	//3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	//������Sep�͕W��������Ă��邱��
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
