#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include<cassert>
#include<Effekseer.h>
#include <DirectXMath.h>
#include<EffekseerRendererDX12.h>
#include"DirectXCommon.h"
#include"Camera.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "EffekseerRendererDX12.lib")
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "LLGI.lib")

class Effects {
public:
	Effects();
	~Effects();
	void Initialize(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, Camera *camera);

	void Update(ID3D12GraphicsCommandList *cmdList, Camera *camera);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetCamera(Camera* camera);
public:
	//エフェクトレンダラー
	EffekseerRenderer::RendererRef _efkRenderer = nullptr;
	//エフェクトマネージャー
	Effekseer::ManagerRef _efkManager = nullptr;

	//-----DX12やVulkan,metalなどコマンドリストを使うライブラリに対応するもの-----
	//メモリプール
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> _efkMemoryPool = nullptr;
	//コマンドリスト(DirectX12やVulkanへの対応のため)
	Effekseer::RefPtr<EffekseerRenderer::CommandList> _efkCmdList = nullptr;

	//-----エフェクト再生に必要なもの-----
	//エフェクト本体(エフェクトファイルに対応)
	Effekseer::EffectRef _effect = nullptr;
	Effekseer::EffectRef _effect1 = nullptr;
	//エフェクトハンドル(再生中のエフェクトに対応)
	Effekseer::Handle _efkHandle;
	Effekseer::Handle _efkHandle1;
	
};
