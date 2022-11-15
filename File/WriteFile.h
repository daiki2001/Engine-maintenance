#pragma once
#include <stdio.h>

/*関数*/
namespace Engine
{
namespace File
{
// ファイル書き込み用のファイルを開く
int WriteFileOpen(FILE** fileHandle, const char* filePath, int flag = 0);
// ファイル書き込み
int WriteCSV(FILE* fileHandle, int numArray[], int size);
} //File
} //Engine
