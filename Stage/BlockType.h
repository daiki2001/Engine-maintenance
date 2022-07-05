#pragma once
#include "./Header/DrawPolygon.h"
#include <string>

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

	// ID�̎擾
	const int GetId() const { return typeId; }
};
