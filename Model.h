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
public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W
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
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);


	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	static void SetDevice(ID3D12Device* device) { Model::device = device; }


private:
	//�f�o�C�X�i�؂�Ă���j
	static ID3D12Device* device;

	// ���_�f�[�^�z��
	//static VertexPosNormalUv vertices[vertexCount];
	 std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	//static unsigned short indices[planeCount * 3];
	 std::vector<unsigned short> indices;

	 Matrial material;
	 // �e�N�X�`���o�b�t�@
	 Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	 // �f�X�N���v�^�q�[�v
	 Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	 // �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	 CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	 // �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	 CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	 // �f�X�N���v�^�T�C�Y
	 UINT descriptorHandleIncrementSize;

	 void LoadFromOBJInternal();
	

	
};

//private: // �G�C���A�X
//// Microsoft::WRL::���ȗ�
//	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//	// DirectX::���ȗ�
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMMATRIX = DirectX::XMMATRIX;
//


//public:
//
//
//private://�����o�ϐ�
//	// ���_�o�b�t�@
//	 ComPtr<ID3D12Resource> vertBuff;
//	// ���_�o�b�t�@�r���[
//	 D3D12_VERTEX_BUFFER_VIEW vbView;
//	// �C���f�b�N�X�o�b�t�@
//	 ComPtr<ID3D12Resource> indexBuff;
//	// �C���f�b�N�X�o�b�t�@�r���[
//	D3D12_INDEX_BUFFER_VIEW ibView;

//
//
//	

