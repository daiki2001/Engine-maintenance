#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Shaders.h"
#include <vector>

class ShaderManager final
{
public: //シングルトン化
	static ShaderManager* Get();
private:
	ShaderManager();
	ShaderManager(const ShaderManager&) = delete;
	~ShaderManager();
	ShaderManager operator=(const ShaderManager&) = delete;

private: //エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template<class T> using vector = std::vector<T>;

public: //サブクラス
	class InputLayout
	{
	public: //メンバ変数
		vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	public: //メンバ関数
		InputLayout();
		~InputLayout() = default;

		D3D12_INPUT_LAYOUT_DESC GetInputLayout() const { return { inputLayout.data(), static_cast<UINT>(inputLayout.size()) }; }
		void PushInputLayout(const LPCSTR& SemanticName, DXGI_FORMAT Format);
		void PopInputLayout() { inputLayout.pop_back(); }
	};

	enum BlendMode
	{
		NOBLEND, //ノーブレンド
		ALPHA,	 //αブレンド
		ADD,	 //加算合成
		SUB,	 //減算合成
		INV		 //色反転合成
	};

	struct PipelineState
	{
	public: //メンバ変数
		ComPtr<ID3D12PipelineState> pipelinestate[5]; //パイプラインの状態
		ComPtr<ID3D12RootSignature> rootsignature;    //ルートシグネチャ

	public: //メンバ関数
		// コンストラクタ
		PipelineState();
	};

public: //静的メンバ変数
	static BlendMode blendMode;

private: //メンバ変数
	vector<Shaders> shaders;          //シェーダー群
	vector<InputLayout> inputLayouts; //インプットレイアウト

	vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC> gPipelines[5]; //グラフィックパイプライン
	vector<ComPtr<ID3D12RootSignature>> rootSignature;        //ルートシグネチャ
	vector<ComPtr<ID3D12PipelineState>> pipelineState[5];     //パイプラインの状態

public: //メンバ関数
	// コンパイル済シェーダーデータの生成
	int CreateShader(const LPCWSTR& vsFileName, const LPCWSTR& psFileName);
	int CreateShader(const Shaders::Shader& veatex, const LPCWSTR& psFileName);
	int CreateShader(const LPCWSTR& vsFileName, const Shaders::Shader& pixle);
	int CreateShader(const Shaders::Shader& veatex, const Shaders::Shader& pixle);

	// インプットレイアウトの生成
	int CreateInputLayout() { inputLayouts.push_back({}); return static_cast<int>(inputLayouts.size() - 1); }
	// グラフィックパイプラインの生成
	int CreateGPipeline(int shaderIndex, int inputLayoutIndex);
	// ルートシグネチャの生成
	int CreateRootSignature(int gPipelineIndex, const UINT& numParameters, const D3D12_ROOT_PARAMETER* _pParameters);
	// パイプラインステートの生成
	int CreatePipelineState(int gPipelineIndex);

	int ChangePipelineState(ID3D12GraphicsCommandList* cmdList, int rootSignatureIndex, int pipelineStateIndex);
	int ChangePipelineState(ID3D12GraphicsCommandList* cmdList, ID3D12RootSignature* rootSignature, int pipelineStateIndex);

	Shaders& GetShader(int index) { return shaders.at(index); }
	InputLayout& GetInputLayout(int index) { return inputLayouts.at(index); }
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetGraphicsPipeline(int index)
	{ return gPipelines[blendMode].at(index); }
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetGraphicsPipeline(int index, BlendMode blendMode)
	{ return gPipelines[blendMode].at(index); }
	ID3D12RootSignature* GetRootSignature(int index) const { return rootSignature.at(index).Get(); }
	ID3D12PipelineState* GetPipelineState(int index) const
	{ return pipelineState[blendMode].at(index).Get(); }
	ID3D12PipelineState* GetPipelineState(int index, BlendMode blendMode) const
	{ return pipelineState[blendMode].at(index).Get(); }
};
