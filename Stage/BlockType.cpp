#include "BlockType.h"

#include "./Header/Error.h"

const int BlockType::WIDTH = 32;
const int BlockType::HEIGHT = 32;
const std::wstring BlockType::blockResourcesDir = L"./Resources/Game/Block/";

BlockType::BlockType(const int& typeId, DrawPolygon* const draw) :
	draw(draw),
	typeId(typeId),
	graph(Engine::FUNCTION_ERROR)
{
}

BlockType::~BlockType()
{
}

int BlockType::Create(const wchar_t* filename)
{
	graph = draw->LoadTextrue((blockResourcesDir + filename).c_str());
	return graph;
}

void BlockType::Draw(const int& posX, const int& posY)
{
	draw->DrawTextrue(posX, posY, WIDTH, HEIGHT, 0, graph, DirectX::XMFLOAT2(0.0f, 0.0f));
}