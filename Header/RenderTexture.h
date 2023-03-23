#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <string>
#include <vector>

// テクスチャ一枚分のデータをまとめた構造体
struct Textrue
{
	std::wstring name;
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff; //テクスチャバッファ

	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle; //CPU側のハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle; //GPU側のハンドル
};

class RenderTexture final
{
private: //エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template<class T> using vector = std::vector<T>;

public: //シングルトン化
	static RenderTexture* Get();
private:
	RenderTexture();
	RenderTexture(const RenderTexture&) = delete;
	~RenderTexture();
	RenderTexture operator=(const RenderTexture&) = delete;

public: //静的メンバ変数
	static float clearColor[4];

public: //メンバ関数
	// レンダーテクスチャの生成
	HRESULT CreateRenderTexture(vector<Textrue>* const texBuff,
								ComPtr<ID3D12DescriptorHeap>* descHeapSRV, const UINT& texCount = 1);
	// レンダーターゲットビューの生成
	HRESULT CreateRTV(ComPtr<ID3D12DescriptorHeap>* descHeapRTV,
					  const vector<Textrue>& texBuff);
};
