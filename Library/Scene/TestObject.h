#pragma once
#include "./Header/DirectDrawing.h"
#include "./Header/DrawPolygon.h"

class TestObject final : private OBJData
{
public: // �V���O���g����
	static TestObject* Get();
private:
	TestObject();
	TestObject(const TestObject&) = delete;
	~TestObject() override;
	TestObject operator=(const TestObject&) = delete;

public: // �萔
	static const float sphereRadius; //�I�u�W�F�N�g�̑傫��

public: // �����o�֐�
	// ������
	void Init() override;
	// �X�V
	void Update() override;
	// �`��
	void Draw();

	// �Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo& info) override;

	int CreateModel(const char* filePath = nullptr, const char* directoryPath = nullptr);
	inline void SetModel(const int& objectData) { object = objectData; }
	inline void SetDraw(DrawPolygon* draw) { this->draw = draw; }

private: // �����o�ϐ�
	DrawPolygon* draw; //�`��p�I�u�W�F�N�g

	int object; //�I�u�W�F�N�g
	bool isObj;
};
