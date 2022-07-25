#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"Easing.h"
class Player {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	Player();
	~Player();
	void Initialize();

	void Update(Camera* camera);

	void Move(Camera* camera);

	void defense();

	void defenseKey();

	void defenseMove(XMFLOAT3 FinalPos);

	void Jump();

	void HomingBullet();

	void Draw();


public:
	Object3dModel* playerModel = nullptr;
	Object3d* objPlayer = nullptr;
	Object3dModel* bulletModel = nullptr;
	Object3d* objBullet = nullptr;
	Easing* easing = nullptr;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// �v���C���[���W
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 oldPos = { 0,0,0 };
	XMFLOAT3 endPos = { 0,0,0 };
	XMFLOAT3 storagePos = { 0,0,0 };
	//�e���W
	XMFLOAT3 bulletPos = { 0,0,0 };

	enum direction
	{
		Previous,
		Back,
		Right,
		Left
	};

private:
	//�W�����v�֘A�̕ϐ�
	bool jumpFlag = false;
	const float gravity = 9.8;
	bool gravityFlag = false;

	//����֘A�̕ϐ�
	bool defenceFlag = false;
	
public:
	int defence_direction;
	float time = 0;
	XMFLOAT3 distance;

	//�e�ϐ�
	bool bulletFlag = false;
	const float bulletSpeed = 0.5;
	float bulletTime = 0;
	const float m = 1;	//����
	const float PI = 3.141592;
	const float angle = 30;
	XMVECTOR lengthVec;

};