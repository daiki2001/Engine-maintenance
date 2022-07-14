#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/EngineGeneral.h"

class Player final
{
public: //�V���O���g����
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

private: //�G�C���A�X
	using Vector3 = Math::Vector3;

public: //�T�u�N���X
	enum Direction
	{
		RIGHT = -1,
		UP,
		LEFT,
		DOWN
	};

public: //�����o�ϐ�
	Vector3 pos; //���W
private:
	DrawPolygon* draw;

	Direction direction; //�����Ă������
	int object;          //�v���C���[�̃I�u�W�F�N�g
	int graph;           //�摜

public: //�����o�֐�
	// ����������
	void Init(DrawPolygon* const draw);
	// �X�V����
	void Update(const Input* const input);
	// �`�揈��
	void Draw(const int& offsetX = 0, const int& offsetY = 0);

	const Direction GetDirection() const { return direction; };
private:
	void Move(const Input* const input);
};
