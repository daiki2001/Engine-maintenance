#include "./Header/EngineGeneral.h"

namespace Engine
{
namespace General
{
std::string ExtractDirectory(const std::string& path)
{
	using string = std::string;

	size_t pos1;
	// ��؂蕶��'\\'���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(0, pos1 + 1);
	}
	// ��؂蕶��'/'���o�Ă����ԍŌ�̕���������
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
	// ��؂蕶��'\\'���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	// ��؂蕶��'/'���o�Ă����ԍŌ�̕���������
	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
} //General
} //Engine