#include "Model.h"
#include<vector>
#include<string>
#include <DirectXMath.h>
#include <fstream>
#include<sstream>
#include<cassert>
#include <DirectXTex.h>





using namespace DirectX;
ID3D12Device* Model::device = nullptr;
Model* Model::LoadFromobj()
{
    Model* model = new Model();

	model->LoadFromOBJInternal();

	//�o�b�t�@����







	return model;

	

 
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	std::ifstream file;

	file.open(directoryPath + filename);

	if (file.fail())
	{
		assert(0);
	}
	std::string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);
		std::string key;
		getline(line_stream, key, ' ');
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}


		if (key == "newmtl")
		{
			line_stream >> material.name;
		}
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;

			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	file.close();
}

bool Model::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	std::string filepath = directoryPath + filename;

	wchar_t wfilepath[128];

	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));


	// WIC�e�N�X�`���̃��[�h
	//result = LoadFromWICFile(L"Resources/tex1.png", WIC_FLAGS_NONE, &metadata, scratchImg);
	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);

	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr, 
		IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // ���f�[�^���o
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);
	return true;
}

void Model::LoadFromOBJInternal()
{
	const std::string modelname = "triangle_mat";
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/" + modelname + "/";



	std::ifstream file;

	file.open(directoryPath + filename);

	if (file.fail()) {
		assert(0);
	}

	std::vector<XMFLOAT3>positions;
	std::vector<XMFLOAT3>normals;
	std::vector<XMFLOAT2>texcoords;

	std::string line;

	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		std::string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			std::string filename;
			line_stream >> filename;

			LoadMaterial(directoryPath, filename);
		}

		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.emplace_back(position);

			/*VertexPosNormalUv vertex{};
			vertex.pos = position;
			vertices.emplace_back(vertex);*/
		}
		if (key == "f") {
			std::string index_string;

			while (getline(line_stream, index_string, ' ')) {
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;

				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back((unsigned short)indices.size());

				//indices.emplace_back(indexPosition - 1);
			}
		}
		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;

			texcoords.emplace_back(texcoord);

		}
		if (key == "vn")
		{
			XMFLOAT3 normal{};

			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}
	}
	file.close();
}


