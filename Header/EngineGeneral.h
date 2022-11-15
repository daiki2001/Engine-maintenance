#pragma once
#include "./Math/EngineMath.h"
#include "./File/LoadFile.h"
#include "./File/WriteFile.h"
#include "Error.h"

/*定数*/
namespace Engine
{
static const std::string resourcesDirectory = "./Resources/";               //リソースディレクトリ
#ifdef ENGINE_EXPORTS
static const std::string engineResourcesDir = "./Engine/Resources/";
#elif GAME_EXPORTS
static const std::string engineResourcesDir = "./Engine/Resources/";
#else
static const std::string engineResourcesDir = "./Resources/Engine/";
#endif
static const std::string shadersDirectory = engineResourcesDir + "shaders/"; //シェーダーディレクトリ
} //Engine

/*関数*/
namespace Engine
{
std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);

// ディレクトリを含んだファイルパスからディレクトリパスを抽出する
std::string ExtractDirectory(const std::string& path);
// ディレクトリを含んだファイルパスからファイル名を抽出する
std::string ExtractFileName(const std::string& path);

template <class T>
constexpr void Swap(T* a, T* b)
{
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

template <class T>
void ContainerClear(T& std)
{
	std.clear();
	std.shrink_to_fit();
}
} //Engine

using namespace Engine;
