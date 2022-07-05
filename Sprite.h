#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite {
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	struct ConstBufferData {
		XMFLOAT4 color;
		XMMATRIX mat;
	};

public:
	//静的初期化
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);
	//テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);
	//描画前
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//描画後
	static void PostDraw();
	//生成
	static Sprite* Create(UINT texNumber, XMFLOAT2 position,
		XMFLOAT4 color = { 1,1,1,1 }, XMFLOAT2 anchorpoint = { 0.0f,0.0f }, bool isFlipX = false, bool isFlipY = false);
	//座標取得
	const XMFLOAT2& GetPosition() { return position; }

protected:
	//テクスチャの最大枚数
	static const int srvCount = 512;
	//頂点数
	static const int vertNum = 4;
	//デバイス
	static ID3D12Device* dev;
	//デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	// 射影行列
	static XMMATRIX matProjection;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[srvCount];

public:
	Sprite(UINT texnumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	void Initialize();

	void Update();

	void SetPosition(XMFLOAT2 position);

	void SetRotation(float rotation);

	void SetSize(XMFLOAT2 size);

	void SetTexBase(XMFLOAT2 texBase);

	void SetAnchorPoint(XMFLOAT2 anchorpoint);

	void SetIsFlipX(bool isFlipX);

	void SetIsFlipY(bool isFlipY);

	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);

	void SetColor(XMFLOAT4 color);

	void Draw();

protected:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// テクスチャ番号
	UINT texNumber = 0;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{};
	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	//左上
	XMFLOAT2 leftTop = { 0,0 };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texBase = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 100.0f, 100.0f };

private: // メンバ関数
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();
};