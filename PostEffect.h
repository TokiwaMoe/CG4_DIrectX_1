#pragma once
#include "Sprite.h"
#include "Input.h"
#include "Object3d.h"

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
    PostEffect(Input* input);

    ///<summary>
    ///初期化
    ///</summary>
    void Initialize(LPCWSTR vs, LPCWSTR ps);

    ///<summary>
    ///描画コマンドの発行
    ///</summary>
    ///<param name="cmdList">コマンドリスト</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreateGraphicsPipelineState(LPCWSTR vs, LPCWSTR ps);

protected:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff[2];
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    //ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;
    // デスクリプタヒープ
    static ComPtr<ID3D12DescriptorHeap> descHeap;

public:
    //グラフィックパイプライン
    ComPtr<ID3D12PipelineState> pipelineState;

private:
    //画面クリアカラー
    static const float clearColor[4];

    Input* input = nullptr;

    Object3d* obj3d = nullptr;

    float _Time = 0;
};

