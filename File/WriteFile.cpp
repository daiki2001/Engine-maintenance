#include "WriteFile.h"
#include <string.h>
#include <Windows.h>

namespace Engine
{
namespace File
{
int WriteFileOpen(FILE** fileHandle, const char* filePath, int flag)
{
	char mode[3] = "w";

	if (flag & 0x1)
	{
		strcat_s(mode, 3, "b");
	}
	else
	{
		strcat_s(mode, 3, "t");
	}

	errno_t err = fopen_s(fileHandle, filePath, mode);
	if (err != 0)
	{
		OutputDebugStringA("ファイルが開けません。\n");
		return err;
	}

	return 0;
}

int WriteCSV(FILE* fileHandle, int numArray[], const size_t& size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (i > 0)
		{
			fprintf_s(fileHandle, ", %d", numArray[i]);
		}
		else
		{
			fprintf_s(fileHandle, "%d", numArray[i]);
		}
	}

	return fprintf_s(fileHandle, "\n");
}
} //File
} //Engine
