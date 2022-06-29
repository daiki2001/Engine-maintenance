#include "./Header/Model.h"
#include "./Header/DirectXInit.h"
#include "./Header/FbxLoader.h"

#include "./Header/Error.h"

/*�V�F�[�_�p*/
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

Node::Node() :
	name{},
	position{ 0.0f, 0.0f, 0.0f },
	rotation{ Engine::Math::Identity() },
	scaling{ 1.0f, 1.0f, 1.0f },
	transform{ Engine::Math::Identity() },
	globalTransform{ Engine::Math::Identity() },
	parent{ nullptr }
{
}

ID3D12Device* Model::dev = DirectXInit::GetDevice();
Microsoft::WRL::ComPtr<ID3D12RootSignature> Model::rootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Model::pipelineState;

Model::Model() :
	meshNode(nullptr),
	ambient{ 1.0f, 1.0f, 1.0f },
	diffuse{ 1.0f, 1.0f, 1.0f },
	metadata{},
	scratchImg{},
	pos(0.0f, 0.0f, 0.0f),
	rota(Engine::Math::Identity()),
	scale(1.0f, 1.0f, 1.0f),
	world(Engine::Math::Identity()),
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

	using namespace Microsoft::WRL;

	isInit = true;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	hr = D3DCompileFromFile(
		L"./lib/Shaders/FBXVS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(hr)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	hr = D3DCompileFromFile(
		L"./lib/Shaders/FBXPS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(hr)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
					errorBlob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �e�����󂯂�{�[���ԍ�(4��)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �{�[���̃X�L���E�F�C�g(4��)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// �����_�[�^�[�Q�b�g�̐ݒ�
	gpipeline.NumRenderTargets = 2;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

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

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
							   D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	hr = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
								  rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(hr))
	{
		return hr;
	}

	gpipeline.pRootSignature = rootSignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	hr = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
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

int Model::Update()
{
	using namespace Engine::Math;

	if (isPlay)
	{
		currentTime += frameTime;

		if (currentTime >= endTime)
		{
			currentTime = startTime;
		}
	}

	world = Identity();
	world *= Engine::Math::scale(scale);
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
	// �p�C�v���C���̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
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
			constMap->viewProj = Engine::Math::Identity();
			constMap->world = Engine::Math::Identity();
			constMap->cameraPos = Engine::Math::Vector3();
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
			constMapSkin->bones[i] = Engine::Math::Identity();
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
