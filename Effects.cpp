#include "Effects.h"

void Effects::Initialize(DirectXCommon* dxCommon)
{
	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	_efkRenderer = EffekseerRendererDX12::Create(
		dxCommon->GetDev(),	//DirectX12�̃f�o�C�X
		dxCommon->GetCmdQueue(),	//DirectX12�̃R�}���h�L���[
		2,	//�o�b�N�o�b�t�@�[�̐�
		bbFormats,	//�����_�[�^�[�Q�b�g�t�H�[�}�b�g
		1,	//�����_�[�^�[�Q�b�g��
		DXGI_FORMAT_UNKNOWN,	//�f�v�X�t�H�[�}�b�g
		false,	//�f�v�X�����邩
		10000);	//�ő�p�[�e�B�N���̐�

	//�}�l�[�W���[������
	_efkManager = Effekseer::Manager::Create(10000);	//�ő�C���X�^���X��
	//���W�n������n�ɂ���(�N���C�A���g���̌n�ɍ��킹��)
	_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	//�`��p�C���X�^���X����`��@�\��ݒ�
	_efkManager->SetSpriteRenderer(_efkRenderer->CreateSpriteRenderer());
	_efkManager->SetRibbonRenderer(_efkRenderer->CreateRibbonRenderer());
	_efkManager->SetRingRenderer(_efkRenderer->CreateRingRenderer());
	_efkManager->SetTrackRenderer(_efkRenderer->CreateTrackRenderer());
	_efkManager->SetModelRenderer(_efkRenderer->CreateModelRenderer());

	//�`��p�C���X�^���X����e�N�X�`���̓ǂݍ��݋@�\���ݒ�
	//�Ǝ��g�����\
	_efkManager->SetTextureLoader(_efkRenderer->CreateTextureLoader());
	_efkManager->SetModelLoader(_efkRenderer->CreateModelLoader());

	//DirectX12���L�̏���
	_efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(_efkRenderer->GetGraphicsDevice());
	_efkCmdList = EffekseerRenderer::CreateCommandList(_efkRenderer->GetGraphicsDevice(), _efkMemoryPool);

	_efkRenderer->SetCommandList(_efkCmdList);

	//�G�t�F�N�g�̓ǂݍ���
	_effect = Effekseer::Effect::Create(
		_efkManager,
		(const EFK_CHAR*)L"effectsTest/10/SimpleLaser.efk",	//�G�t�F�N�g�t�@�C��
		1.0f,	//�X�P�[�����O�l
		(const EFK_CHAR*)L"effectsTest/10"	//�G�t�F�N�g��t�H���_�[(�e�N�X�`����T���̂ɕK�v)
	);

	//�G�t�F�N�g�̍Đ�
	_efkHandle = _efkManager->Play(_effect, 0, 0, 0);
}

void Effects::Update()
{

}
