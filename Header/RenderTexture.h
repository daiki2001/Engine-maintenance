#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

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
	HRESULT CreateRenderTexture(vector<ComPtr<ID3D12Resource>>* const texBuff,
								ComPtr<ID3D12DescriptorHeap>* descHeapSRV, const UINT& texCount = 1);
	// レンダーターゲットビューの生成
	HRESULT CreateRTV(ComPtr<ID3D12DescriptorHeap>* descHeapRTV,
					  const vector<ComPtr<ID3D12Resource>>& texBuff);
};
