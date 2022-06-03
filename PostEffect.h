#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
private:
    // Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    ///<summary>
    ///コンストラクタ
    ///</summary>
    PostEffect();

    ///<summary>
    ///初期化
    ///</summary>
    void Initialize();

    ///<summary>
    ///描画コマンドの発行
    ///</summary>
    ///<param name="cmdList">コマンドリスト</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

protected:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};

