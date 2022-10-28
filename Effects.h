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
	void Initialize(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, Camera *camera, Effekseer::EffectRef _effect);

	void Update(ID3D12GraphicsCommandList *cmdList, Camera *camera, Effekseer::Handle handle);

	void Play(Effekseer::Handle handle, Effekseer::EffectRef _effect);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetCamera(Camera* camera);

	DirectX::XMFLOAT3 SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }
	DirectX::XMFLOAT3 SetRotation(DirectX::XMFLOAT3 rotation) { this->rotation = rotation; }
	DirectX::XMFLOAT3 SetScale(DirectX::XMFLOAT3 scale) { this->scale = scale; }
	float SetSpeed(float speed) { this->speed = speed; }
	float SetAngle(float angle) { this->angle = angle; }

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
	//Effekseer::EffectRef _effect = nullptr;
	//Effekseer::EffectRef _effect1 = nullptr;
	////�G�t�F�N�g�n���h��(�Đ����̃G�t�F�N�g�ɑΉ�)
	//Effekseer::Handle _efkHandle;
	//Effekseer::Handle _efkHandle1;

protected:// �����o�ϐ�
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 0,0,0 };
	float speed = 0;
	float angle = 0;

	
};
