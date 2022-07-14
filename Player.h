#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
class Player {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Player();
	~Player();
	void Initialize();

	void Update(Camera* camera);

	void Move(Camera* camera);

	void defense();

	void Jump();

	void Draw();


public:
	Object3dModel* playerModel = nullptr;
	Object3d* objPlayer = nullptr;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 1,0,0 };

private:
	//�W�����v�֘A�̕ϐ�
	bool jumpFlag = false;
	const float gravity = 9.8;
	bool gravityFlag = false;
	const float m = 1;	//����
	const float k = 2;	//��C��R
	const float PI = 3.141592;
	const float angle = 60;
	
	const float speed = 0.5;
	//����֘A�̕ϐ�
	bool defenceFlag = false;

public:
	float time = 0;
	

};