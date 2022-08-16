#pragma once
#include <windows.h>
#include <string>
#include <cassert>

/*定数*/
namespace Engine
{
static constexpr int FUNCTION_ERROR = -1;
} // Engine

/*関数*/
namespace Engine
{
// エラーログを出力ウィンドウに出力し、終了する
static int Error(const std::string& str, const bool& flag = true, const char* file = __FILE__, const int& line = __LINE__)
{
	if (flag == false)
	{
		return 0;
	}

	char logMessage[1024];

	sprintf_s(logMessage, "%s(line:%d)\n", file, line);

	OutputDebugStringA(str.c_str());
	OutputDebugStringA(logMessage);

#ifdef _DEBUG
	assert(0);
	exit(1);
#endif // _DEBUG

	return FUNCTION_ERROR;
}
#define ErrorLog(str, flag) Error(str, flag, __FILE__, __LINE__)
} // Engine