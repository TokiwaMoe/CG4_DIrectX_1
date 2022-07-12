#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"Input.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
class Play {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Play();
	~Play();
	void Initialize(Input* input);

	void Update(Camera* camera);

	void Move();

	void Draw();

	XMFLOAT3& GetPosition() { return position; }


public:
	Object3dModel* playerModel = nullptr;
	Object3d* player = nullptr;
	Input* input = nullptr;

	float speed = 0.5f;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 1,0,0 };

};