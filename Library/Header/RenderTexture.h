#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

class RenderTexture final
{
private: //�G�C���A�X
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�V���O���g����
	static RenderTexture* Get();
private:
	RenderTexture();
	RenderTexture(const RenderTexture&) = delete;
	~RenderTexture();
	RenderTexture operator=(const RenderTexture&) = delete;

public: //�ÓI�����o�ϐ�
	static float clearColor[4];

public: //�����o�֐�
	// �����_�[�e�N�X�`���̐���
	HRESULT CreateRenderTexture(std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>* const texBuff,
								Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>* descHeapSRV, const UINT& texCount = 1);
	// �����_�[�^�[�Q�b�g�r���[�̐���
	HRESULT CreateRTV(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>* descHeapRTV,
					  const std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff);
};
