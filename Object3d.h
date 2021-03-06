#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Object3dModel.h"
#include"CollisionInfo.h"
#include "Camera.h"

class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

	struct Material
	{
		std::string name;	//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.1f,0.1f,0.1f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;	//3D変換行列
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	//定数バッファ用データ構造体B1


private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create();


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	//static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	//static ComPtr<ID3D12DescriptorHeap> descHeap;

	// テクスチャバッファ
	//static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	//static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	//static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	//static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	//static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	/*static VertexPosNormalUv vertices[vertexCount];*/
	static std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	//static unsigned short indices[planeCount * 3];
	static std::vector<unsigned short> indices;
	//マテリアル
	static Material material;
	// カメラ
	static Camera* camera;

private:// 静的メンバ関数

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数

	///<summary>
	///コンストラクト
	///</summary>
	Object3d() = default;

	///<summary>
	///デストラクタ
	///</summary>
	virtual ~Object3d();

	///<summary>
	///初期化
	///</summary>
	/// <returns>成否</returns>
	virtual bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();


	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	virtual void Draw();

	void SetObject3dModel(Object3dModel* Object3dModel) { this->object3dModel = Object3dModel; }

	/// <summary>
	/// ビルボードフラグのセット
	/// </summary>
	/// <param name="isBillboard">ビルボードか</param>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	///<summary>
	///ワールド行列の取得
	///</summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	///<summary>
	///コライダーのセット
	///</summary>
	/// <param name="collision">コライダー</param>
	void SetCollider(BaseCollider* collider);

	///<summary>
	///衝突時コールバック関数
	///</summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

protected:// メンバ変数
//ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0.0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;

	Object3dModel* object3dModel = nullptr;

	//クラス名(デバッグ用)
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;
	// ビルボード
	bool isBillboard = false;
};

