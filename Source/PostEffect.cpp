#include "./Header/PostEffect.h"
#include "./Header/DirectXInit.h"
#include "./Header/DirectDrawing.h"
#include "./ShaderMgr/ShaderManager.h"

namespace
{
ShaderManager* shaderMgr = ShaderManager::Get();
}

/*シェーダ用*/
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

PostEffect::PostEffect() :
	position(0.0f, 0.0f, 0.0f),
	angle(0.0f),
	matWorld(Math::Identity()),
	color(1.0f, 1.0f, 1.0f, 1.0f),
	shader(FUNCTION_ERROR),
	inputLayout(FUNCTION_ERROR),
	gPipeline(FUNCTION_ERROR),
	rootSignature(nullptr),
	pipelineState(FUNCTION_ERROR),
	vertBuff{},
	vbView{},
	constBuff{},
	texBuff{},
	descHeapSRV{},
	descHeapRTV{},
	depthBuff{},
	descHeapDSV{}
{
}

int PostEffect::Init()
{
	using namespace Engine;

	HRESULT hr = S_FALSE;
	DirectXInit* w = DirectXInit::GetInstance();
	auto dev = DirectXInit::GetDevice();
	auto renderTex = RenderTexture::Get();

	hr = CreateGraphicsPipelineState();
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	hr = CreateVertexBuffer();
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	// 定数バッファの生成
	hr = CreateConstantBuffer();
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	hr = renderTex->CreateRenderTexture(&texBuff, &descHeapSRV, 2);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	hr = renderTex->CreateRTV(&descHeapRTV, texBuff);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	// リソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		w->windowWidth,
		w->windowHeight,
		1,
		0,
		1,
		0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	// リソース生成
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff)
	);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	// DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescHeapDesc = {};
	dsvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDescHeapDesc.NumDescriptors = 1;
	// DSV用デスクリプタヒープ作成
	hr = dev->CreateDescriptorHeap(&dsvDescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	// デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart()
	);

	return 0;
}

int PostEffect::Draw()
{
	using namespace DirectX;
	using namespace Engine;

	static bool isInit = false;
	static auto dev = DirectXInit::GetDevice();
	static auto cmdList = DirectXInit::GetCommandList();
	static auto renderTex = RenderTexture::Get();

	if (isInit == false)
	{
		isInit = true;
	}

#pragma region GraphicsCommand

	SpriteConstBufferData* constMap = nullptr;
	HRESULT hr = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(hr))
	{
		constMap->color = color;
		constMap->mat = Math::Identity();
		constBuff->Unmap(0, nullptr);
	}

	// プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(1, texBuff[0].gpuDescHandle);
	//cmdList->SetGraphicsRootDescriptorTable(2, texBuff[1].gpuDescHandle);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);

#pragma endregion

	return hr;
}

int PostEffect::PreDraw()
{
	static DirectXInit* w = DirectXInit::GetInstance();
	static auto dev = DirectXInit::GetDevice();
	static auto cmdList = DirectXInit::GetCommandList();
	static auto renderTex = RenderTexture::Get();

	for (size_t i = 0; i < texBuff.size(); i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			texBuff[i].texbuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[2] = {};
	for (int i = 0; i < static_cast<int>(texBuff.size()); i++)
	{
		// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(),
			i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}
	// 深度ステンシルビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットをセット
	cmdList->OMSetRenderTargets(_countof(rtvH), rtvH, false, &dsvH);

	// ビューポート領域
	CD3DX12_VIEWPORT viewports[2] = {};
	// シザー矩形
	CD3DX12_RECT scissorRects[2] = {};

	for (size_t i = 0; i < texBuff.size(); i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(w->windowWidth),
			static_cast<float>(w->windowHeight)
		);

		scissorRects[i] = CD3DX12_RECT(
			0,
			0,
			w->windowWidth,
			w->windowHeight
		);
	}

	// ビューポート領域の設定
	cmdList->RSSetViewports(_countof(viewports), viewports);

	// シザー矩形の設定
	cmdList->RSSetScissorRects(_countof(scissorRects), scissorRects);

	// 画面クリア
	for (size_t i = 0; i < texBuff.size(); i++)
	{
		cmdList->ClearRenderTargetView(rtvH[i], RenderTexture::clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	return 0;
}

int PostEffect::PostDraw()
{
	static auto cmdList = DirectXInit::GetCommandList();
	static auto renderTex = RenderTexture::Get();

	for (size_t i = 0; i < texBuff.size(); i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			texBuff[i].texbuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		));
	}

	return 0;
}

HRESULT PostEffect::CreateGraphicsPipelineState()
{
	HRESULT hr = S_FALSE;
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	auto dev = DirectXInit::GetDevice();

	// 各種シェーダーのコンパイルと読み込み
	shader = shaderMgr->CreateShader(StringToWString(shadersDirectory + "PostEffectTestVS.hlsl").c_str(),
									 StringToWString(shadersDirectory + "PostEffectTestPS.hlsl").c_str());

	// 頂点レイアウト
	inputLayout = DirectDrawing::Get2dInputLayout();

	// グラフィックスパイプライン設定
	gPipeline = shaderMgr->CreateGPipeline(shader, inputLayout);

	for (size_t i = 0; i < 5; i++)
	{
		auto& gp = shaderMgr->GetGraphicsPipeline(gPipeline, static_cast<ShaderManager::BlendMode>(i));

		// ラスタライザステート
		gp.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //背面カリングしない

		// デプスステンシルステートの設定
		gp.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		gp.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; //常に上書き
	}

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0 = {};
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0レジスタ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1 = {};
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); //t1レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(
		_countof(rootparams),
		rootparams,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	// バージョン自動判定のシリアライズ
	hr = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob
	);
	if (FAILED(hr))
	{
		assert(0);
		return hr;
	}

	// ルートシグネチャの生成
	hr = dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr))
	{
		assert(0);
		return hr;
	}

	// パイプラインにルートシグネチャをセット
	for (size_t i = 0; i < 5; i++)
	{
		shaderMgr->GetGraphicsPipeline(gPipeline, static_cast<ShaderManager::BlendMode>(i)).pRootSignature =
			rootSignature.Get();
	}

	// グラフィックスパイプラインの生成
	pipelineState = shaderMgr->CreatePipelineState(gPipeline);

	return hr;
}

HRESULT PostEffect::CreateVertexBuffer()
{
	HRESULT hr = S_FALSE;
	auto dev = DirectXInit::GetDevice();

	SpriteVertex vert[] = {
		{{ -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }},
		{{ -1.0f, +1.0f, 0.0f }, { 0.0f, 0.0f }},
		{{ +1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }},
		{{ +1.0f, +1.0f, 0.0f }, { 1.0f, 0.0f }},
	};

	// 頂点バッファの生成
	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vert)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(hr))
	{
		return hr;
	}

	// 頂点バッファへのデータ転送
	SpriteVertex* vertexMap = nullptr;
	hr = vertBuff->Map(0, nullptr, (void**)&vertexMap);
	if (SUCCEEDED(hr))
	{
		memcpy(vertexMap, vert, sizeof(vert));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの生成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vert);
	vbView.StrideInBytes = sizeof(SpriteVertex);

	return hr;
}

HRESULT PostEffect::CreateConstantBuffer()
{
	HRESULT hr = S_FALSE;
	auto dev = DirectXInit::GetDevice();

	// 定数バッファの生成
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

	// 定数バッファにデータ転送
	SpriteConstBufferData* constMap = nullptr;
	hr = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(hr))
	{
		constMap->color = color;
		constMap->mat = Math::Identity();
		constBuff->Unmap(0, nullptr);
	}

	return hr;
}
