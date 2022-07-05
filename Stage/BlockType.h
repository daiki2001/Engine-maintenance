#pragma once
#include "./Header/DrawPolygon.h"
#include <string>

#ifdef _DEBUG
#include <DirectXMath.h>
#endif // _DEBUG

class BlockType
{
public: //�萔
	static const int WIDTH;
	static const int HEIGHT;
	static const std::wstring blockResourcesDir;

private: //�����o�ϐ�
	DrawPolygon* const draw;

	int typeId;
	int graph;

public: //�����o�֐�
	BlockType(const int& typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// ��������
	virtual int Create(const wchar_t* filename);
	// �`�揈��
	void Draw(const int& posX, const int& posY);
#ifdef _DEBUG
	void Draw(const int& posX, const int& posY, const DirectX::XMFLOAT4& color);
#endif // _DEBUG

	// ID�̎擾
	const int GetId() const { return typeId; }
};
