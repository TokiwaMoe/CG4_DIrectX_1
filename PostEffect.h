#pragma once
#include "Sprite.h"
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
    PostEffect();

    ///<summary>
    ///������
    ///</summary>
    void Initialize();

    ///<summary>
    ///�`��R�}���h�̔��s
    ///</summary>
    ///<param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

protected:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};
