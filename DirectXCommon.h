#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include"WinApp.h"

//DirectX基盤
class DirectXCommon
{
private:
	//WindowsAPI
	WinApp* winApp = nullptr;
public://メンバ関数
	void Initialize(WinApp* winApp);
	void InitializeDevice();
	void InitializeCommon();
	void InitializeSwapChain();
	bool InitializeRenderTargets();
	void InitializeDepthBuffer();
	void InitializeFence();
	void ClearRenderTarget();
	void ClearDepthBuffer();
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//デバイス取得
	ID3D12Device* GetDev() { return dev.Get(); }
	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
	//コマンドキュー取得
	ID3D12CommandQueue* GetCmdQueue() { return cmdQueue.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
};
