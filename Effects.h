#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include<cassert>
#include<Effekseer.h>
#include<EffekseerRendererDX12.h>
#include"DirectXCommon.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "EffekseerRendererDX12.lib")
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "LLGI.lib")

class Effects {
public:
	Effects();
	~Effects();
	void Initialize(ID3D12Device* device, ID3D12CommandQueue* cmdQueue);

	void Update(ID3D12GraphicsCommandList *cmdList);

	void Draw();
public:
	//�G�t�F�N�g�����_���[
	EffekseerRenderer::RendererRef _efkRenderer = nullptr;
	//�G�t�F�N�g�}�l�[�W���[
	Effekseer::ManagerRef _efkManager = nullptr;

	//-----DX12��Vulkan,metal�ȂǃR�}���h���X�g���g�����C�u�����ɑΉ��������-----
	//�������v�[��
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> _efkMemoryPool = nullptr;
	//�R�}���h���X�g(DirectX12��Vulkan�ւ̑Ή��̂���)
	Effekseer::RefPtr<EffekseerRenderer::CommandList> _efkCmdList = nullptr;

	//-----�G�t�F�N�g�Đ��ɕK�v�Ȃ���-----
	//�G�t�F�N�g�{��(�G�t�F�N�g�t�@�C���ɑΉ�)
	Effekseer::EffectRef _effect = nullptr;
	//�G�t�F�N�g�n���h��(�Đ����̃G�t�F�N�g�ɑΉ�)
	Effekseer::Handle _efkHandle;
	
};
