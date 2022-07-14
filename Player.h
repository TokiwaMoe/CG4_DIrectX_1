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

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 1,0,0 };

private:
	//ジャンプ関連の変数
	bool jumpFlag = false;
	const float gravity = 9.8;
	bool gravityFlag = false;
	//回避関連の変数
	bool defenceFlag = false;
	

};