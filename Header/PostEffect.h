#pragma once
#include "RenderTexture.h"
#include "./Header/EngineGeneral.h"
#include <d3d12.h>
#include <wrl.h>
#include <vector>

class PostEffect
{
private: //エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using Vector3 = Math::Vector3;
	using Matrix4 = Math::Matrix4;
	
private: //定数
		
public: //メンバ変数
	Vector3 position;        //座標
	float angle;             //回転角度
	Matrix4 matWorld;        //ワールド行列
	DirectX::XMFLOAT4 color; //色
private:
	int shader;                                //シェーダー
	int inputLayout;                           //インプットレイアウト
	int gPipeline;                             //グラフィックパイプライン
	ComPtr<ID3D12RootSignature> rootSignature; //ルートシグネチャ
	int pipelineState;                         //パイプラインステート

	ComPtr<ID3D12Resource> vertBuff;  //頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;  //頂点バッファビュー
	ComPtr<ID3D12Resource> constBuff; //定数バッファ

	std::vector<Textrue> texBuff; //テクスチャバッファ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;    //SRV用のデスクリプタヒープ

	ComPtr<ID3D12DescriptorHeap> descHeapRTV; //RTV用のデスクリプタヒープ

	ComPtr<ID3D12Resource> depthBuff;         //深度バッファ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV; //DSV用のデスクリプタヒープ

public: //メンバ関数
	// コンストラクタ
	PostEffect();

	// 初期化処理
	int Init();
	// 描画処理
	int Draw();

	// 描画前処理
	int PreDraw();
	// 描画後処理
	int PostDraw();

	int GetShader() { return shader; }
	int GetInputLayout() { return inputLayout; }
	int GetGPipeline() { return gPipeline; }
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	int GetPipelineState() { return pipelineState; }
private:
	// パイプラインの生成
	HRESULT CreateGraphicsPipelineState();
	// 頂点バッファの生成
	HRESULT CreateVertexBuffer();
	// 定数バッファの生成
	HRESULT CreateConstantBuffer();
};
