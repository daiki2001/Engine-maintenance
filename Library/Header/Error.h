#pragma once
#include <windows.h>
#include <string>
#include <cassert>

/*�萔*/
namespace Engine
{
static constexpr int FUNCTION_ERROR = -1;
} // Engine

/*�֐�*/
namespace Engine
{
// �G���[���O���o�̓E�B���h�E�ɏo�͂��A�I������
static void ErrorLog(const std::string& str, const bool& flag = true, const char* file = __FILE__, const int& line = __LINE__)
{
	if (flag == false)
	{
		return;
	}

	char logMessage[1024];

	sprintf_s(logMessage, "%s(line:%d)\n", file, line);

	OutputDebugStringA(str.c_str());
	OutputDebugStringA(logMessage);
	assert(0);
	exit(1);

	return;
}
} // Engine