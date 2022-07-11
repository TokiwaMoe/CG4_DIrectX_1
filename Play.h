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
	void Initialize(DirectXCommon* dxCommon, Input* input);

	/// <summary>
	/// ???t???[??????
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// ?`??
	/// </summary>
	void Draw();

	XMFLOAT3& GetPosition() { return position; }


public:
	Object3dModel* playerModel = nullptr;
	Object3d* player = nullptr;

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	float speed = 0.5f;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };

};