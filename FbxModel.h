#pragma once
#include<DirectXMath.h>
#include<DirectXTex.h>
#include<string>
#include<vector>
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<fbxsdk.h>

//�m�[�h
struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
private://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template<class T> using vector = std::vector<T>;
public:
	//�t�����h�N���X
	friend class FbxLoader;

public://�萔
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

public://�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];//�{�[���@�ԍ�
		float boneWeight[MAX_BONE_INDICES];//�{�[���@�d��
	};

	//�{�[���N���X
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name) { this->name = name; }
	};
public:
	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};

public://�����o�֐�
	//�f�X�g���N�^
	~FbxModel();
	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	//gettter
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }

private://�����o�ϐ�
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//�{�[���z��
	std::vector<Bone> bones;

protected:
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
};