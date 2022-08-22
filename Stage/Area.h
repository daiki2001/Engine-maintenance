#pragma once
#include "./Header/DrawPolygon.h"
#include "BlockManager.h"
#include "./Header/EngineGeneral.h"

class Area
{
private: //�G�C���A�X
	using Vector3 = Math::Vector3;

private: //�ÓI�����o�ϐ�
	static DrawPolygon* draw;
	static BlockManager* block_mgr;
	static int wall_obj; //�O�ǂ̃I�u�W�F�N�g
	static int door_obj; //�h�A�̃I�u�W�F�N�g

public: //�ÓI�����o�֐�
	static const bool IsGoal() { return block_mgr->GetGoal(); }

	static BlockManager* GetBlockManager() { return block_mgr; }

public: //�����o�֐�
	Area();
	~Area();

	// ����������
	void Init(DrawPolygon* const draw);
	// �X�V����
	void Update();
	// �`�揈��
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
	// ���Z�b�g����
	void Reset();

	// �X�e�[�W�ǂݍ���
	void LoadArea(const char* filePath = nullptr);
};
