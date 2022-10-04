﻿#include "./Header/LoadFile.h"
#include "./Header/Error.h"

namespace Engine
{
namespace Load
{
int LoadMapChip(const char* filePath, int* mapArray, const size_t& mapSize, int LoadStopNumber)
{
	if (filePath == nullptr)
	{
		return Engine::FUNCTION_ERROR;
	}
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };
	int index = 0;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	bool isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		bool end = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}

			if ((index >= mapSize) || (mapArray[index] == LoadStopNumber))
			{
				// ヒットしたら、読み込みを強制的に終了する。
				end = true;
				break;
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

int LoadMapChip(FILE* fileHandle, int* mapArray, const size_t& mapSize, int LoadStopNumber)
{
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	char string[256] = { 0 };
	int index = 0;

	bool isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		bool end = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}

			if ((index >= mapSize) || (mapArray[index] == LoadStopNumber))
			{
				// ヒットしたら、読み込みを強制的に終了する。
				end = true;
				break;
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}
} //Load
} //Engine
