#pragma once
#include "./Header/DrawPolygon.h"

class Player final
{
public: //�V���O���g����
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //�T�u�N���X
	enum Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

private: //�����o�ϐ�
	DrawPolygon* draw;

	int posX, posY;      //���W
	Direction direction; //�����Ă������
	int graph;           //�摜

public: //�����o�֐�
	// ����������
	void Init(DrawPolygon* const draw);
	// �X�V����
	void Update();
	// �`�揈��
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
};
