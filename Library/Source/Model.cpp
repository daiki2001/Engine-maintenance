#include "./Header/Model.h"
#include "./Header/DirectXInit.h"
#include "./Header/FbxLoader.h"
#include "./ShaderMgr/ShaderManager.h"

/*�V�F�[�_�p*/
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace
{
ShaderManager* shaderMgr = ShaderManager::Get();
}

Node::Node() :
	name{},
	position{ 0.0f, 0.0f, 0.0f },
	rotation{ Math::Identity() },
	scaling{ 1.0f, 1.0f, 1.0f },
	transform{ Math::Identity() },
	globalTransform{ Math::Identity() },
	parent{ nullptr }
{
}

ID3D12Device* Model::dev = DirectXInit::GetDevice();
int Model::defaultShader = FUNCTION_ERROR;
int Model::colorShader = FUNCTION_ERROR;
int Model::inputLayout = FUNCTION_ERROR;
int Model::defaultGPipeline = FUNCTION_ERROR;
int Model::colorGPipeline = FUNCTION_ERROR;
int Model::rootSignature;
int Model::defaultPipelineState;
int Model::colorPipelineState;

Model::Model() :
	meshNode(nullptr),
	ambient{ 1.0f, 1.0f, 1.0f },
	diffuse{ 1.0f, 1.0f, 1.0f },
	metadata{},
	scratchImg{},
	pos(0.0f, 0.0f, 0.0f),
	rota(Math::Identity()),
	scale(1.0f, 1.0f, 1.0f),
	world(Math::Identity()),
	frameTime{},
	startTime{},
	endTime{},
	currentTime{},
	isPlay(false)
{
	Init();
}

Model::~Model()
{
	Finalize();
}

HRESULT Model::CreateGraphicsPipeline()
{
	HRESULT hr = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;    //���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    //�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g

	static bool isInit = false;

	if (isInit == true)
	{
		return S_OK;
	}

	isInit = true;

	// �e��V�F�[�_�[�̃R���p�C���Ɠǂݍ���
	defaultShader = shaderMgr->CreateShader(L"./lib/Shaders/FBXVS.hlsl", L"./lib/Shaders/FBXPS.hlsl");
	colorShader = shaderMgr->CreateShader(shaderMgr->GetShader(defaultShader).GetVertex(), L"./lib/Shaders/color.hlsl");

	// ���_���C�A�E�g
	inputLayout = shaderMgr->CreateInputLayout();
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);

	defaultGPipeline = shaderMgr->CreateGPipeline(defaultShader, inputLayout);
	colorGPipeline = shaderMgr->CreateGPipeline(colorShader, inputLayout);

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	// CBV�i�X�L�j���O�p�j
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	rootSignature = shaderMgr->CreateRootSignature(defaultGPipeline, _countof(rootparams), rootparams);

	for (size_t i = 0; i < 5; i++)
	{
		auto& defaultGpipeline = shaderMgr->GetGraphicsPipeline(defaultGPipeline, static_cast<ShaderManager::BlendMode>(i));
		auto& colorGpipeline = shaderMgr->GetGraphicsPipeline(colorGPipeline, static_cast<ShaderManager::BlendMode>(i));

		// �u�����h�X�e�[�g�̐ݒ�
		defaultGpipeline.BlendState.RenderTarget[1] = defaultGpipeline.BlendState.RenderTarget[0];
		colorGpipeline.BlendState = colorGpipeline.BlendState;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		defaultGpipeline.NumRenderTargets = 2;    // �`��Ώۂ�1��
		defaultGpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
		defaultGpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
		colorGpipeline.NumRenderTargets = defaultGpipeline.NumRenderTargets;
		colorGpipeline.RTVFormats[0] = defaultGpipeline.RTVFormats[0];
		colorGpipeline.RTVFormats[1] = defaultGpipeline.RTVFormats[1];

		colorGpipeline.pRootSignature = shaderMgr->GetRootSignature(rootSignature);
	}

	defaultPipelineState = shaderMgr->CreatePipelineState(defaultGPipeline);
	colorPipelineState = shaderMgr->CreatePipelineState(colorGPipeline);

	return S_OK;
}

void Model::ChangeDefaultShader()
{
	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		rootSignature,
		defaultPipelineState);
}

void Model::ChangeColorShader()
{
	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		rootSignature,
		colorPipelineState);
}

void Model::Init()
{
	dev = DirectXInit::GetDevice();

	if (FAILED(CreateGraphicsPipeline()))
	{
		assert(0);
	}

	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

int Model::Update(DirectX::XMFLOAT4 color)
{
	using namespace Math;

	if (isPlay)
	{
		currentTime += frameTime;

		if (currentTime >= endTime)
		{
			currentTime = startTime;
		}
	}

	world = Identity();
	world *= Math::scale(scale);
	world *= rota;
	world *= translate(pos);

	Matrix4 matViewPorj =
		Camera::GetMatView() *
		Camera::matProjection[Camera::Projection::PERSPECTIVE];
	const Matrix4& modelTrans = GetModelTransform();
	const Vector3& cameraPos = Camera::pos;

	HRESULT hr = S_FALSE;
	ConstBufferData* constMap = nullptr;
	hr = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(hr))
	{
		constMap->viewProj = matViewPorj;
		constMap->world = modelTrans * world;
		constMap->color = color;
		constMap->cameraPos = cameraPos;
		constBuff->Unmap(0, nullptr);
	}

	ConstBufferDataSkin* constMapSkin = nullptr;
	hr = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < bones.size(); i++)
		{
			// ���̎p���s����擾
			FbxAMatrix fbxCurrentPose =
				bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
			// Matrix4�^�ɕϊ�
			Matrix4 matCurrentPose = FbxLoader::ConvertMatrixFromFbx(fbxCurrentPose);
			// �������ăX�L�j���O�s���
			constMapSkin->bones[i] =
				modelTrans *
				bones[i].invInitPose *
				matCurrentPose *
				Inverse(modelTrans);
		}
		constBuffSkin->Unmap(0, nullptr);
	}

	return 0;
}

void Model::Draw()
{
	static ID3D12GraphicsCommandList* cmdList = DirectXInit::GetCommandList();

#pragma region �I�u�W�F�N�g
	// �v���~�e�B�u�`��̐ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
#pragma endregion //�I�u�W�F�N�g

#pragma region ���f��
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		descHeapSRV->GetGPUDescriptorHandleForHeapStart()
	);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
#pragma endregion //���f��

}

void Model::Finalize()
{
	if (fbxScene != nullptr)
	{
		fbxScene->Destroy();
		fbxScene = nullptr;
	}
}

HRESULT Model::CreateBuffers()
{
	using namespace Engine;
	HRESULT hr = S_FALSE;

#pragma region CreateVertexBuffer
	// ���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices.size());
	// ���_�o�b�t�@�̐���
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(hr))
	{
		return hr;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUvSkin* vertMap = nullptr;
	hr = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(hr))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		// �}�b�v������
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion //CreateVertexBuffer

#pragma region CreateIndexBuffer
	// ���_�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// �C���f�b�N�X�o�b�t�@�̐���
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(hr))
	{
		return hr;
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	hr = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(hr))
	{
		std::copy(indices.begin(), indices.end(), indexMap);
		// �}�b�v������
		indexBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
#pragma endregion //CreateIndexBuffer

#pragma region CreateTexBuffer
	// �e�N�X�`���摜�f�[�^
	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);
	assert(img);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// �e�N�X�`���o�b�t�@�̐���
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
								 D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
	if (FAILED(hr))
	{
		return hr;
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	hr = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		static_cast<UINT>(img->rowPitch),
		static_cast<UINT>(img->slicePitch)
	);
#pragma endregion //CreateTexBuffer

#pragma region CreateSRV
	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	hr = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	if (FAILED(hr))
	{
		return hr;
	}

	// �V�F�[�_�[���\�[�X�r���[�̍쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(
		texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);
#pragma endregion //CreateSRV

	return S_OK;
}

HRESULT Model::CreateConstBuffer()
{
	HRESULT hr = S_FALSE;

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF), //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(hr))
	{
		return hr;
	}
	ConstBufferData* constMap = nullptr;
	hr = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < bones.size(); i++)
		{
			constMap->viewProj = Math::Identity();
			constMap->world = Math::Identity();
			constMap->cameraPos = Math::Vector3();
			constMap->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		constBuff->Unmap(0, nullptr);
	}

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xFF) & ~0xFF), //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));
	if (FAILED(hr))
	{
		return hr;
	}
	ConstBufferDataSkin* constMapSkin = nullptr;
	hr = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < MAX_BONE; i++)
		{
			constMapSkin->bones[i] = Math::Identity();
		}
		constBuffSkin->Unmap(0, nullptr);
	}

	return S_OK;
}

void Model::PlayAnimation()
{
	// 0�Ԃ̃A�j���[�V�������擾
	FbxAnimStack* animStack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	// �A�j���[�V�����̖��O�擾
	const char* animStackName = animStack->GetName();
	// �A�j���[�V�����̎��ԏ��
	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);

	// �J�n���Ԏ擾
	startTime = takeInfo->mLocalTimeSpan.GetStart();
	// �I�����Ԏ擾
	endTime = takeInfo->mLocalTimeSpan.GetStop();
	// �J�n���Ԃɍ��킹��
	currentTime = startTime;
	// �Đ�����Ԃɂ���
	isPlay = true;
}
