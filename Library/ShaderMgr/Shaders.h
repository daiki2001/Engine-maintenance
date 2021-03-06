#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>

class Shaders
{
private: //エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //サブクラス
	enum ShaderType
	{
		VERTEX_SHADER,   //頂点シェーダー
		PIXLE_SHADER,    //ピクセルシェーダー
		DOMAIN_SHADER,   //ドメインシェーダー
		HULL_SHADER,     //ハルシェーダー
		GEOMETRY_SHADER, //ジオメトリシェーダー
		COMPUTE_SHADER   //コンピュートシェーダー
	};

	class Shader
	{
	private: //静的メンバ変数
		static ComPtr<ID3DBlob> errorBlob;

	private: //メンバ変数
		ComPtr<ID3DBlob> shaderBlob;
		std::wstring fileName;

	public: //メンバ関数
		Shader();
		~Shader() = default;

		int CompileShader(LPCWSTR pFileName, ShaderType shaderType);
		std::wstring GetFileName() const { return fileName; }
		ID3DBlob* GetShaderBlob() const { return shaderBlob.Get(); }
	};

private: //メンバ変数
	Shader vertexShader = {};   //頂点シェーダー
	Shader pixleShader = {};    //ピクセルシェーダー
	Shader domainShader = {};   //ドメインシェーダー
	Shader hullShader = {};     //ハルシェーダー
	Shader geometryShader = {}; //ジオメトリシェーダー
	Shader computeShader = {};  //コンピュートシェーダー

public: //メンバ関数
	Shaders() = default;
	~Shaders() = default;

	// 頂点シェーダーのコンパイル
	int CompileVertexShader(LPCWSTR pFileName)
	{ return vertexShader.CompileShader(pFileName, ShaderType::VERTEX_SHADER); }
	// ピクセルシェーダーのコンパイル
	int CompilePixleShader(LPCWSTR pFileName)
	{ return pixleShader.CompileShader(pFileName, ShaderType::PIXLE_SHADER); }
	// ドメインシェーダーのコンパイル
	int CompileDomainShader(LPCWSTR pFileName)
	{ return domainShader.CompileShader(pFileName, ShaderType::DOMAIN_SHADER); }
	// ハルシェーダーのコンパイル
	int CompileHullShader(LPCWSTR pFileName)
	{ return hullShader.CompileShader(pFileName, ShaderType::HULL_SHADER); }
	// ジオメトリシェーダーのコンパイル
	int CompileGeometryShader(LPCWSTR pFileName)
	{ return geometryShader.CompileShader(pFileName, ShaderType::GEOMETRY_SHADER); }
	// コンピュートシェーダーのコンパイル
	int CompileComputeShader(LPCWSTR pFileName)
	{ return computeShader.CompileShader(pFileName, ShaderType::COMPUTE_SHADER); }

	Shader GetVertex() const { return vertexShader; }
	Shader GetPixle() const { return pixleShader; }
	Shader GetDomain() const { return domainShader; }
	Shader GetHull() const { return hullShader; }
	Shader GetGeometry() const { return geometryShader; }
	Shader GetCompute() const { return computeShader; }
};
