#pragma once
#include <d3d12.h>
#include <wrl.h>

class Shaders
{
private: //�G�C���A�X
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //�T�u�N���X
	enum ShaderType
	{
		VERTEX_SHADER,   //���_�V�F�[�_�[
		PIXLE_SHADER,    //�s�N�Z���V�F�[�_�[
		DOMAIN_SHADER,   //�h���C���V�F�[�_�[
		HULL_SHADER,     //�n���V�F�[�_�[
		GEOMETRY_SHADER, //�W�I���g���V�F�[�_�[
		COMPUTE_SHADER   //�R���s���[�g�V�F�[�_�[
	};

	class Shader
	{
	private: //�ÓI�����o�ϐ�
		static ComPtr<ID3DBlob> errorBlob;

	private: //�����o�ϐ�
		ComPtr<ID3DBlob> shaderBlob;

	public: //�����o�֐�
		Shader();
		~Shader() = default;

		int CompileShader(LPCWSTR pFileName, ShaderType shaderType);
		ID3DBlob* GetShaderBlob() const { return shaderBlob.Get(); }
	};

private: //�����o�ϐ�
	Shader vertexShader = {};   //���_�V�F�[�_�[
	Shader pixleShader = {};    //�s�N�Z���V�F�[�_�[
	Shader domainShader = {};   //�h���C���V�F�[�_�[
	Shader hullShader = {};     //�n���V�F�[�_�[
	Shader geometryShader = {}; //�W�I���g���V�F�[�_�[
	Shader computeShader = {};  //�R���s���[�g�V�F�[�_�[

public: //�����o�֐�
	Shaders() = default;
	~Shaders() = default;

	// ���_�V�F�[�_�[�̃R���p�C��
	int CompileVertexShader(LPCWSTR pFileName)
	{ return vertexShader.CompileShader(pFileName, ShaderType::VERTEX_SHADER); }
	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	int CompilePixleShader(LPCWSTR pFileName)
	{ return pixleShader.CompileShader(pFileName, ShaderType::PIXLE_SHADER); }
	// �h���C���V�F�[�_�[�̃R���p�C��
	int CompileDomainShader(LPCWSTR pFileName)
	{ return domainShader.CompileShader(pFileName, ShaderType::DOMAIN_SHADER); }
	// �n���V�F�[�_�[�̃R���p�C��
	int CompileHullShader(LPCWSTR pFileName)
	{ return hullShader.CompileShader(pFileName, ShaderType::HULL_SHADER); }
	// �W�I���g���V�F�[�_�[�̃R���p�C��
	int CompileGeometryShader(LPCWSTR pFileName)
	{ return geometryShader.CompileShader(pFileName, ShaderType::GEOMETRY_SHADER); }
	// �R���s���[�g�V�F�[�_�[�̃R���p�C��
	int CompileComputeShader(LPCWSTR pFileName)
	{ return computeShader.CompileShader(pFileName, ShaderType::COMPUTE_SHADER); }

	Shader GetVertexShader() { return vertexShader; }
	Shader GetPixleShader() { return pixleShader; }
	Shader GetDomainShader() { return domainShader; }
	Shader GetHullShader() { return hullShader; }
	Shader GetGeometryShader() { return geometryShader; }
	Shader GetComputeShader() { return computeShader; }
};
