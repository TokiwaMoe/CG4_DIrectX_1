#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Object3dModel {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: //静的メンバ関数
	//OBJファイルから3dモデルを読み込み
	static Object3dModel* LoadFromOBJ(const std::string& Object3dModelname);
	//setter
	static void SetDevice(ID3D12Device* device) { Object3dModel::dev = device; }
	void CreateBuffers();
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
	
private:
	bool InitializeDescriptorHeap();
	//マテリアル読み込み
	void loadMaterial(const std::string& directoryPath, const std::string& filename);
	//テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	
private: //非公開のメンバ関数
	//OBJファイルから3Dモデルを読み込む(非公開)
	void LoadFromOBJInternal(const std::string &Object3dModelname);

public: //サブクラス
	//頂点データ構造体
	struct VertexPosNormalUv {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	//マテリアル
	struct Material {
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;
		XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;
		XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};

private: //メンバ変数
	//デバイス
	static ID3D12Device* dev;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;
	// シェーダリソースビューのハンドル(CPU)
	 CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	 CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタヒープ
	 ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	 ComPtr<ID3D12Resource> texbuff;
	// 頂点データ配列
	 std::vector<VertexPosNormalUv> vertices;
	//マテリアル
	 Material material;
	//頂点インデックス配列
	std::vector<unsigned short> indices;
	// 頂点バッファビュー
	 D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	 D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点バッファ
	 ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	 ComPtr<ID3D12Resource> indexBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
};
