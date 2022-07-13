#pragma once
#include "./Header/DrawPolygon.h"
#include <string>
#include "./Header/EngineGeneral.h"

class BlockType
{
public: //�G�C���A�X
	using Vector3 = Math::Vector3;

public: //�萔
	static const int WIDTH;
	static const int HEIGHT;
	static const std::wstring blockResourcesDir;

private: //�ÓI�����o�ϐ�
	static DrawPolygon* draw;
	static int BlockBox;
	
private: //�����o�ϐ�
	int typeId;
	int graph;

public: //�����o�֐�
	BlockType(const int& typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// ��������
	virtual int Create(const wchar_t* filename);
	// �`�揈��
	void Draw(const Vector3& pos);
#ifdef _DEBUG
	void Draw(const Vector3& pos, const DirectX::XMFLOAT4& color);
#endif // _DEBUG

	// ID�̎擾
	const int GetId() const { return typeId; }
};
