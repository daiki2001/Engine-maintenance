#include "ShaderManager.h"
#include <string>
#include <d3dx12.h>
#include "./Header/DirectXInit.h"

#include "./Header/Error.h"

ShaderManager::InputLayout::InputLayout() :
	inputLayout{}
{
}

void ShaderManager::InputLayout::PushInputLayout(const LPCSTR& SemanticName, DXGI_FORMAT Format)
{
	inputLayout.push_back({
		SemanticName,                               //セマンティック名
		0,                                          //同じセマンティック名が複数あるときに使うインデックス
		Format,                                     //要素数とビット数を表す
		0,                                          //入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,               //データのオフセット値
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, //入力データ種別
		0										    //一度に描画するインスタンス数
						  });
}

ShaderManager::PipelineState::PipelineState() :
	pipelinestate{},
	rootsignature(nullptr)
{
}

ShaderManager::BlendMode ShaderManager::blendMode = ShaderManager::BlendMode::NOBLEND;

ShaderManager::ShaderManager() :
	shaders{},
	inputLayouts{},
	gPipelines{},
	rootSignature{},
	pipelineState{}
{
}

ShaderManager::~ShaderManager()
{
}

ShaderManager* ShaderManager::Get()
{
	static ShaderManager instance = {};
	return &instance;
}

int ShaderManager::CreateShader(const LPCWSTR& vsFileName, const LPCWSTR& psFileName)
{
	shaders.push_back({});

	shaders.back().CompileVertexShader(vsFileName);
	shaders.back().CompilePixleShader(psFileName);

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateShader(const Shaders::Shader& veatex, const LPCWSTR& psFileName)
{
	shaders.push_back({});

	shaders.back().CompileVertexShader(veatex.GetFileName().c_str());
	shaders.back().CompilePixleShader(psFileName);

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateShader(const LPCWSTR& vsFileName, const Shaders::Shader& pixle)
{
	shaders.push_back({});

	shaders.back().CompileVertexShader(vsFileName);
	shaders.back().CompilePixleShader(pixle.GetFileName().c_str());

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateShader(const Shaders::Shader& veatex, const Shaders::Shader& pixle)
{
	shaders.push_back({});

	shaders.back().CompileVertexShader(veatex.GetFileName().c_str());
	shaders.back().CompilePixleShader(pixle.GetFileName().c_str());

	return static_cast<int>(shaders.size() - 1);
}

int ShaderManager::CreateGPipeline(int shaderIndex, int inputLayoutIndex)
{
	if (shaderIndex < 0 || inputLayoutIndex < 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	for (size_t i = 0; i < sizeof(gPipelines) / sizeof(gPipelines[0]); i++)
	{
		gPipelines[i].push_back({});
		auto& gpipeline = gPipelines[i].back();

		// 頂点シェーダ、ピクセルシェーダをパイプラインに設定
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaders[shaderIndex].GetVertex().GetShaderBlob());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaders[shaderIndex].GetPixle().GetShaderBlob());
		// サンプルマスク
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
		// ラスタライザステート
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //背面カリングしない
		// ブレンドデスクの設定
		auto& blendDesc = gpipeline.BlendState.RenderTarget[0];
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //標準設定

		switch (i)
		{
		case BlendMode::NOBLEND:
			/*ノーブレンド用の設定*/
			blendDesc.BlendEnable = false; //ブレンドを無効にする
			break;
		case BlendMode::ALPHA:
			/*αブレンド用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;          //加算
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;      //ソースのアルファ値
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; //1.0f - ソースのアルファ値
			goto BaseBlendState;
		case BlendMode::ADD:
			/*加算合成用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD; //加算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;   //ソースの値を 100% 使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;  //デストの値を 100% 使う
			goto BaseBlendState;
		case BlendMode::SUB:
			/*減算合成用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; //デストからソースを減算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;            //ソースの値を 100% 使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;           //デストの値を 100% 使う
			goto BaseBlendState;
		case BlendMode::INV:
			/*色反転用の設定*/
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;          //加算
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; //1.0f - デストカラーの値
			blendDesc.DestBlend = D3D12_BLEND_ZERO;          //使わない
			goto BaseBlendState;
		default:
		BaseBlendState:
			/*共通設定*/
			blendDesc.BlendEnable = true;                //ブレンドを有効にする
			blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //加算
			blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;   //ソースの値を 100% 使う
			blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; //デストの値を   0% 使う
			break;
		}
		// デプスステンシルステート
		gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		// 深度バッファのフォーマット
		gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		// 頂点レイアウトの設定
		gpipeline.InputLayout = inputLayouts[inputLayoutIndex].GetInputLayout();
		// 図形の形状を三角形に設定
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		// その他の設定
		gpipeline.NumRenderTargets = 1; //描画対象は1つ
		gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0~255指定のRGBA
		gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング
	}

	return static_cast<int>(gPipelines[0].size() - 1);
}

int ShaderManager::CreateRootSignature(int gPipelineIndex, const UINT& numParameters, const D3D12_ROOT_PARAMETER* _pParameters)
{
	if (gPipelineIndex < 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	HRESULT hr = S_FALSE;
	ComPtr<ID3DBlob> rootSigBlob = {};
	ComPtr<ID3DBlob> errorBlob = {};
	auto dev = DirectXInit::GetDevice();

	rootSignature.push_back(nullptr);
	auto& rootSig = rootSignature.back();

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(
		numParameters,
		_pParameters,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);

	hr = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob
	);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSig.ReleaseAndGetAddressOf())
	);
	if (FAILED(hr))
	{
		return hr;
	}

	// パイプラインにルートシグネチャをセット
	for (size_t i = 0; i < 5; i++)
	{
		gPipelines[i].at(gPipelineIndex).pRootSignature = rootSig.Get();
	}

	return static_cast<int>(rootSignature.size() - 1);
}

int ShaderManager::CreatePipelineState(int gPipelineIndex)
{
	if (gPipelineIndex < 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	HRESULT hr = S_FALSE;
	auto dev = DirectXInit::GetDevice();

	for (size_t i = 0; i < sizeof(PipelineState::pipelinestate) / sizeof(PipelineState::pipelinestate[0]); i++)
	{
		pipelineState[i].push_back(nullptr);
		auto& pipeline = pipelineState[i].back();

		hr = dev->CreateGraphicsPipelineState(&gPipelines[i].at(gPipelineIndex), IID_PPV_ARGS(&pipeline));
	}

	return static_cast<int>(pipelineState[0].size() - 1);
}

int ShaderManager::ChangePipelineState(ID3D12GraphicsCommandList* cmdList, int rootSignatureIndex, int pipelineStateIndex)
{
	if (cmdList == nullptr || rootSignatureIndex < 0 || pipelineStateIndex < 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature[rootSignatureIndex].Get());
	// パイプラインの設定
	cmdList->SetPipelineState(pipelineState[blendMode].at(pipelineStateIndex).Get());

	return 0;
}

int ShaderManager::ChangePipelineState(ID3D12GraphicsCommandList* cmdList, ID3D12RootSignature* rootSignature, int pipelineStateIndex)
{
	if (cmdList == nullptr || rootSignature == nullptr || pipelineStateIndex < 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature);
	// パイプラインの設定
	cmdList->SetPipelineState(pipelineState[blendMode].at(pipelineStateIndex).Get());

	return 0;
}
