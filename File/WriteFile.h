#pragma once
#include <stdio.h>

/*定数*/
namespace Engine
{
namespace File
{
constexpr int TEXT = 0x0;   //テキスト入出力
constexpr int BINARY = 0x1; //バイナリ入出力
} //File
} //Engine

/*関数*/
namespace Engine
{
namespace File
{
// ファイル書き込み用のファイルを開く
int WriteFileOpen(FILE** fileHandle, const char* filePath, int flag = File::TEXT);
// ファイル書き込み
int WriteCSV(FILE* fileHandle, int numArray[], const size_t& size);
} //File
} //Engine
