#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<vector>
#include<string>
#include <wrl.h>

class Model
{
public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	struct Matrial
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;

		std::string textureFilename;

		Matrial() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}

	};




public:
	static Model* LoadFromobj();


	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);


	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	static void SetDevice(ID3D12Device* device) { Model::device = device; }


private:
	//デバイス（借りてくる）
	static ID3D12Device* device;

	// 頂点データ配列
	//static VertexPosNormalUv vertices[vertexCount];
	 std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	//static unsigned short indices[planeCount * 3];
	 std::vector<unsigned short> indices;

	 Matrial material;
	 // テクスチャバッファ
	 Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	 // デスクリプタヒープ
	 Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	 // シェーダリソースビューのハンドル(CPU)
	 CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	 // シェーダリソースビューのハンドル(CPU)
	 CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	 // デスクリプタサイズ
	 UINT descriptorHandleIncrementSize;

	 void LoadFromOBJInternal();
	

	
};

//private: // エイリアス
//// Microsoft::WRL::を省略
//	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//	// DirectX::を省略
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMMATRIX = DirectX::XMMATRIX;
//


//public:
//
//
//private://メンバ変数
//	// 頂点バッファ
//	 ComPtr<ID3D12Resource> vertBuff;
//	// 頂点バッファビュー
//	 D3D12_VERTEX_BUFFER_VIEW vbView;
//	// インデックスバッファ
//	 ComPtr<ID3D12Resource> indexBuff;
//	// インデックスバッファビュー
//	D3D12_INDEX_BUFFER_VIEW ibView;

//
//
//	

