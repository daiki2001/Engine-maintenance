#include "Shaders.h"
#include "./Header/Error.h"

// �V�F�[�_�[�R���p�C���p
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

Shaders::ComPtr<ID3DBlob> Shaders::Shader::errorBlob = nullptr;

Shaders::Shader::Shader() :
	shaderBlob(nullptr),
	fileName{}
{
}

int Shaders::Shader::CompileShader(const LPCWSTR& pFileName, const ShaderType& shaderType)
{
	if (pFileName == nullptr)
	{
		return Engine::ErrorLog("�V�F�[�_�[�t�@�C����������܂���");
	}

	fileName = pFileName;

	static LPCSTR pTarget = nullptr;

	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		pTarget = "vs_5_0";
		break;
	case ShaderType::PIXLE_SHADER:
		pTarget = "ps_5_0";
		break;
	case ShaderType::DOMAIN_SHADER:
		pTarget = "ds_5_0";
		break;
	case ShaderType::HULL_SHADER:
		pTarget = "hs_5_0";
		break;
	case ShaderType::GEOMETRY_SHADER:
		pTarget = "gs_5_0";
		break;
	case ShaderType::COMPUTE_SHADER:
		pTarget = "cs_5_0";
		break;
	default:
		shaderBlob = nullptr;
		errorBlob = nullptr;
		return Engine::ErrorLog("�V�F�[�_�[�^�C�v������܂���");
		break;
	}

	HRESULT hr = D3DCompileFromFile(
		pFileName, //�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				 //�C���N���[�h�\�ɂ���
		"main",											 //�G���g���[�|�C���g��
		pTarget,										 //�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0,
		&shaderBlob,
		&errorBlob);
	if (FAILED(hr))
	{
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

	return 0;
}
