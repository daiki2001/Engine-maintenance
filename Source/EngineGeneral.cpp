#include "./Header/EngineGeneral.h"

namespace Engine
{
std::wstring StringToWString(const std::string& s)
{
	// バッファサイズの取得
	int bufSize = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[bufSize];
	// string → wstring
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, cpUCS2, bufSize);

	// wstringの生成
	std::wstring o(cpUCS2, cpUCS2 + bufSize - 1);
	// バッファの破棄
	delete[] cpUCS2;

	return o;
}

std::string WStringToString(const std::wstring& s)
{
	// バッファサイズの取得
	int bufSize = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// バッファの取得
	char* cpMultiByte = new char[bufSize];
	// wstring → string
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, cpMultiByte, bufSize, nullptr, nullptr);

	// stringの生成
	std::string o(cpMultiByte, cpMultiByte + bufSize - 1);
	// バッファの破棄
	delete[] cpMultiByte;

	return o;
}

std::string ExtractDirectory(const std::string& path)
{
	using string = std::string;

	size_t pos1;
	// 区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(0, pos1 + 1);
	}
	// 区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(0, pos1 + 1);
	}

	return "./";
}

std::string ExtractFileName(const std::string& path)
{
	using string = std::string;

	size_t pos1;
	// 区切り文字'\\'が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// 区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
} //Engine