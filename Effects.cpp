#include "Effects.h"
Effects::Effects(){}
Effects::~Effects(){}

void Effects::Initialize(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, Camera *camera)
{
	assert(device);
	assert(cmdQueue);

	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	_efkRenderer = EffekseerRendererDX12::Create(
		device,	//DirectX12�̃f�o�C�X
		cmdQueue,	//DirectX12�̃R�}���h�L���[
		2,	//�o�b�N�o�b�t�@�[�̐�
		bbFormats,	//�����_�[�^�[�Q�b�g�t�H�[�}�b�g
		1,	//�����_�[�^�[�Q�b�g��
		DXGI_FORMAT_UNKNOWN,	//�f�v�X�t�H�[�}�b�g
		false,	//�f�v�X�����邩
		1000);	//�ő�p�[�e�B�N���̐�

	

	//�}�l�[�W���[������
	_efkManager = Effekseer::Manager::Create(1000);	//�ő�C���X�^���X��
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
	//_effect = Effekseer::Effect::Create(
	//	_efkManager,
	//	(const EFK_CHAR*)L"effectsTest/10/SimpleLaser.efk",	//�G�t�F�N�g�t�@�C��
	//	1.0f,	//�X�P�[�����O�l
	//	(const EFK_CHAR*)L"effectsTest/10"	//�G�t�F�N�g��t�H���_�[(�e�N�X�`����T���̂ɕK�v)
	//);

	_effect1 = Effekseer::Effect::Create(
		_efkManager,
		(const EFK_CHAR*)L"effectsTest/10/firework.efk",	//�G�t�F�N�g�t�@�C��
		1.0f,	//�X�P�[�����O�l
		(const EFK_CHAR*)L"effectsTest/Textures"	//�G�t�F�N�g��t�H���_�[(�e�N�X�`����T���̂ɕK�v)
	);

	
}

void Effects::Play()
{
	_efkHandle1 = _efkManager->Play(_effect1, 3, 0, 0);
}

void Effects::Update(ID3D12GraphicsCommandList *cmdList, Camera *camera)
{
	//�G�t�F�N�g�̍Đ�
	//_efkHandle = _efkManager->Play(_effect1, 0, 0, 0);
	_efkManager->SetScale(_efkHandle, 1, 1, 1);
	_efkManager->SetRotation(_efkHandle1, { 0,0,0 }, 180);
	
	//�J�����ݒ�
	SetCamera(camera);
}

void Effects::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//�G�t�F�N�g�`��
	_efkManager->Update();	//�}�l�[�W���[�̍X�V(���ԍX�V)
	_efkMemoryPool->NewFrame();	//�`�悷�ׂ������_�[�^�[�Q�b�g��I��
	EffekseerRendererDX12::BeginCommandList(_efkCmdList, cmdList);
	_efkRenderer->BeginRendering();	//�`��O����
	_efkManager->Draw();	//�G�t�F�N�g�`��
	_efkRenderer->EndRendering();	//�`��㏈��

	EffekseerRendererDX12::EndCommandList(_efkCmdList);
}

void Effects::SetCamera(Camera* camera)
{
	Effekseer::Matrix44 fkViewMat;
	Effekseer::Matrix44 fkProjMat;
	auto view = camera->GetViewMatrix();//�J�����̃Z�b�g
	auto proj = camera->GetProjectionMatrix();//�ˉe�A�������e����
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			fkViewMat.Values[i][j] = view.r[i].m128_f32[j];
			fkProjMat.Values[i][j] = proj.r[i].m128_f32[j];
		}
	}
	_efkRenderer->SetCameraMatrix(fkViewMat);
	_efkRenderer->SetProjectionMatrix(fkProjMat);
}
