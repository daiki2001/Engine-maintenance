#pragma once
#include <stdio.h>

/*関数*/
namespace Engine
{
namespace File
{
/// <summary>
/// マップチップのファイル読み込み
/// </summary>
/// <param name="filePath"> CSVファイルのパス </param>
/// <param name="mapArray"> 読み込んだマップチップの保存先 </param>
/// <param name="mapSize"> mapArrayのサイズ </param>
/// <param name="isNumberStop"> 特定の数字で止めるかどうか </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0で成功、0以外で失敗 </returns>
int LoadMapChip(const char* filePath, int* mapArray, int mapSize, bool isNumberStop = false, int LoadStopNumber = -1);

/// <summary>
/// マップチップのファイル読み込み
/// </summary>
/// <param name="fileHandle"> ファイルハンドル </param>
/// <param name="mapArray"> 読み込んだマップチップの保存先 </param>
/// <param name="mapSize"> mapArrayのサイズ </param>
/// <param name="isNumberStop"> 特定の数字で止めるかどうか </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0で成功、0以外で失敗 </returns>
int LoadMapChip(FILE* fileHandle, int* mapArray, int mapSize, bool isNumberStop = false, int LoadStopNumber = -1);
} //File
} //Engine
