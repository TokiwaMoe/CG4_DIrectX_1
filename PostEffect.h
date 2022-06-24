#pragma once
#include "Sprite.h"
#include "Input.h"

class PostEffect :
    public Sprite
{
private:
    // Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    ///<summary>
    ///�R���X�g���N�^
    ///</summary>
    PostEffect(Input* input);

    ///<summary>
    ///������
    ///</summary>
    void Initialize();

    ///<summary>
    ///�`��R�}���h�̔��s
    ///</summary>
    ///<param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    void CreateGraphicsPipelineState();

protected:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff[2];
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    //�O���t�B�b�N�p�C�v���C��
    ComPtr<ID3D12PipelineState> pipelineState;
    //���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature;

private:
    //��ʃN���A�J���[
    static const float clearColor[4];

    Input* input = nullptr;
};
