#include "Shaders.h"
#include "./Header/Error.h"

// シェーダーコンパイル用
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

Shaders::ComPtr<ID3DBlob> Shaders::Shader::errorBlob = nullptr;

Shaders::Shader::Shader() :
    shaderBlob(nullptr),
    fileName{}
{
}

int Shaders::Shader::CompileShader(LPCWSTR pFileName, ShaderType shaderType)
{
    if (pFileName == nullptr)
    {
        return Engine::ErrorLog("シェーダーファイル名がありません", true);
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
        return Engine::ErrorLog("シェーダータイプがありません", true);
        break;
    }

    HRESULT hr = D3DCompileFromFile(
        pFileName, //シェーダファイル名
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,				 //インクルード可能にする
        "main",											 //エントリーポイント名
        pTarget,										 //シェーダーモデル指定
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
        0,
        &shaderBlob,
        &errorBlob);
    if (FAILED(hr))
    {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    errstr.begin());
        errstr += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    return 0;
}
