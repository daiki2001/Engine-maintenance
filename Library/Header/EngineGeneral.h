#pragma once
#include <string>

/*�萔*/
namespace Engine
{
namespace General
{
static const std::string libraryDirectory = "./lib/";             //���C�u�����f�B���N�g��
static const std::string defaultTextureFileName = "white1x1.png"; //�f�t�H���g�摜
} //General
} //Engine

/*�֐�*/
namespace Engine
{
namespace General
{
// �f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
std::string ExtractDirectory(const std::string& path);
// �f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
std::string ExtractFileName(const std::string& path);
} //General
} //Engine

using namespace Engine;