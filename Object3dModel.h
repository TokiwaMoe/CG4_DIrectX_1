#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Object3dModel {
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: //�ÓI�����o�֐�
	//OBJ�t�@�C������3d���f����ǂݍ���
	static Object3dModel* LoadFromOBJ(const std::string& Object3dModelname);
	//setter
	static void SetDevice(ID3D12Device* device) { Object3dModel::dev = device; }
	void CreateBuffers();
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
	
private:
	bool InitializeDescriptorHeap();
	//�}�e���A���ǂݍ���
	void loadMaterial(const std::string& directoryPath, const std::string& filename);
	//�e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	
private: //����J�̃����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���(����J)
	void LoadFromOBJInternal(const std::string &Object3dModelname);

public: //�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUv {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	//�}�e���A��
	struct Material {
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;
		XMFLOAT3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
	};

private: //�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	 CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	 CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �f�X�N���v�^�q�[�v
	 ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	 ComPtr<ID3D12Resource> texbuff;
	// ���_�f�[�^�z��
	 std::vector<VertexPosNormalUv> vertices;
	//�}�e���A��
	 Material material;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// ���_�o�b�t�@�r���[
	 D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	 D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�o�b�t�@
	 ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	 ComPtr<ID3D12Resource> indexBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
};
