﻿#pragma once

#include "fbxsdk.h"
#include"FbxModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include<string>

class FbxLoader
{
private://エイリアス
	//std::を省略
	using string = std::string;

public://定数
	//モデル格納ルートパス
	static const string baseDirectory;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

public:
	///<summary>
	///初期化
	///</summary>
	///<param name"device">D3D12デバイス</param>
	void Initiallize(ID3D12Device* device);

	///<summary>
	///ファイルからFBXモデル読み込み
	///</summary>
	///<param name="modelName">モデル名</param>
	void LoadMadelFromFile(const string& modelName);

	///<summary>
	///再帰的にノード構成を解析
	///</summary>
	///<param name="model">読み込み先モデルオブジェクト</param>
	///<param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	///<summary>
	///メッシュ読み取り
	///</summary>
	///<param name="model">読み込み先モデルオブジェクト</param>
	///<param name="fbxNode">解析対象のノード</param>
	void ParseMesh(Model* model, FbxNode* fbxNode);

	///<summary>
	///後始末
	///</summary>
	void Finalize();

private:
	//頂点座標読み取り
	void ParseMeshVertices(Model* model, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	//マテリアル読み込み
	void ParseMaterial(Model* model, FbxNode* fbxNode);
	//テクスチャ読み込み
	void LoadTexture(Model* model, const std::string& fullpath);
	
private:
	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;
};