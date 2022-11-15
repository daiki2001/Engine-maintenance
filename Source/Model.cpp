#include "./Header/Model.h"
#include "./Header/DirectXInit.h"
#include "./Header/FbxLoader.h"
#include "./ShaderMgr/ShaderManager.h"

/*シェーダ用*/
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

/*DirectXTexのライブラリ情報*/
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_d.lib")
#else // _DEBUG
#pragma comment(lib, "DirectXTex.lib")
#endif // !_DEBUG

/*FBXのライブラリ情報*/
#ifdef _DEBUG
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")
#else // _DEBUG
#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")
#pragma comment(lib, "zlib-mt.lib")
#endif // !_DEBUG

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
	ComPtr<ID3DBlob> vsBlob;    //頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    //ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト

	static bool isInit = false;

	if (isInit == true)
	{
		return S_OK;
	}

	isInit = true;

	// 各種シェーダーのコンパイルと読み込み
	defaultShader = shaderMgr->CreateShader(StringToWString(shadersDirectory + "FBXVS.hlsl").c_str(),
											StringToWString(shadersDirectory + "FBXPS.hlsl").c_str());
	colorShader = shaderMgr->CreateShader(shaderMgr->GetShader(defaultShader).GetVertex(),
										  StringToWString(shadersDirectory + "color.hlsl").c_str());

	// 頂点レイアウト
	inputLayout = shaderMgr->CreateInputLayout();
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	shaderMgr->GetInputLayout(inputLayout).PushInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);

	defaultGPipeline = shaderMgr->CreateGPipeline(defaultShader, inputLayout);
	colorGPipeline = shaderMgr->CreateGPipeline(colorShader, inputLayout);

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	// CBV（スキニング用）
	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	rootSignature = shaderMgr->CreateRootSignature(defaultGPipeline, _countof(rootparams), rootparams);

	for (size_t i = 0; i < 5; i++)
	{
		auto& defaultGpipeline = shaderMgr->GetGraphicsPipeline(defaultGPipeline, static_cast<ShaderManager::BlendMode>(i));
		auto& colorGpipeline = shaderMgr->GetGraphicsPipeline(colorGPipeline, static_cast<ShaderManager::BlendMode>(i));

		// ブレンドステートの設定
		defaultGpipeline.BlendState.RenderTarget[1] = defaultGpipeline.BlendState.RenderTarget[0];
		colorGpipeline.BlendState = colorGpipeline.BlendState;

		// レンダーターゲットの設定
		defaultGpipeline.NumRenderTargets = 2;    // 描画対象は1つ
		defaultGpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
		defaultGpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
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

int Model::Update(const DirectX::XMFLOAT4& color)
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
		for (size_t i = 0; i < bones.size(); i++)
		{
			// 今の姿勢行列を取得
			FbxAMatrix fbxCurrentPose =
				bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
			// Matrix4型に変換
			Matrix4 matCurrentPose = FbxLoader::ConvertMatrixFromFbx(fbxCurrentPose);
			// 合成してスキニング行列に
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

#pragma region オブジェクト
	// プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());
#pragma endregion //オブジェクト

#pragma region モデル
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファビューの設定
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		descHeapSRV->GetGPUDescriptorHandleForHeapStart()
	);

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
#pragma endregion //モデル
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
	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices.size());
	// 頂点バッファの生成
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(hr))
	{
		return hr;
	}

	// 頂点バッファへのデータ転送
	VertexPosNormalUvSkin* vertMap = nullptr;
	hr = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(hr))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		// マップを解除
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion //CreateVertexBuffer

#pragma region CreateIndexBuffer
	// 頂点インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// インデックスバッファの生成
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(hr))
	{
		return hr;
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	hr = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(hr))
	{
		std::copy(indices.begin(), indices.end(), indexMap);
		// マップを解除
		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
#pragma endregion //CreateIndexBuffer

#pragma region CreateTexBuffer
	// テクスチャ画像データ
	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);
	assert(img);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		static_cast<UINT>(metadata.height),
		static_cast<UINT16>(metadata.arraySize),
		static_cast<UINT16>(metadata.mipLevels)
	);

	// テクスチャバッファの生成
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

	// テクスチャバッファにデータ転送
	hr = texBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		static_cast<UINT>(img->rowPitch),
		static_cast<UINT>(img->slicePitch)
	);
#pragma endregion //CreateTexBuffer

#pragma region CreateSRV
	// SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	hr = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	if (FAILED(hr))
	{
		return hr;
	}

	// シェーダーリソースビューの作成
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
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF), //リソース設定
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
		for (size_t i = 0; i < bones.size(); i++)
		{
			constMap->viewProj = Math::Identity();
			constMap->world = Math::Identity();
			constMap->cameraPos = Math::Vector3();
			constMap->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		constBuff->Unmap(0, nullptr);
	}

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xFF) & ~0xFF), //リソース設定
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
	// 0番のアニメーションを取得
	FbxAnimStack* animStack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	// アニメーションの名前取得
	const char* animStackName = animStack->GetName();
	// アニメーションの時間情報
	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);

	// 開始時間取得
	startTime = takeInfo->mLocalTimeSpan.GetStart();
	// 終了時間取得
	endTime = takeInfo->mLocalTimeSpan.GetStop();
	// 開始時間に合わせる
	currentTime = startTime;
	// 再生中状態にする
	isPlay = true;
}
