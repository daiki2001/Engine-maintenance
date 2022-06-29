#pragma once
#include "./Math/EngineMath.h"
#include <vector>

class Camera final
{
public: // �V���O���g����
	Camera* Get();
private:
	Camera();
	Camera(const Camera&) = delete;
	~Camera() {}
	Camera operator=(const Camera&) = delete;

private: // �G�C���A�X
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	using Vector3 = Engine::Math::Vector3;
	using Matrix4 = Engine::Math::Matrix4;

public: // �T�u�N���X
	enum Projection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

public: // �萔
	static const size_t MAIN_CAMERA;

public: // �ÓI�����o�֐�
	// ������
	static void Init();

	// �L�����N�^�p���s��̐���
	static DirectX::XMMATRIX CreateCamera(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);
	// �L�����N�^�p�������s��̐���
	static DirectX::XMMATRIX CreateCameraFix(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);

	// �J�����̐؂�ւ�
	static int ChangeCamera(const size_t& cameraNo);
	// �J�����̐ݒ�
	static void SetCamera(const Engine::Math::Vector3& cameraPos, const Engine::Math::Vector3& cameraTarget, const Engine::Math::Vector3& upVector);
	// �J�����̎擾
	static Matrix4 GetMatView() { return matView[cameraNo]; };

public: // �ÓI�����o�ϐ�
	static float targetRadius; //�����_����̋���
	static float longitude;    //�o�x
	static float latitude;     //�ܓx

	static Vector3 pos;    //�J�����̈ʒu
	static Vector3 target; //�J�����̒����_
	static Vector3 upVec;  //��x�N�g��

	static Matrix4 matProjection[2];     //�v���W�F�N�V�����s��
	static std::vector<Matrix4> matView; //�r���[�ϊ��s��(�J����)
	static size_t cameraNo;              //�J�����̔ԍ��i�ŏ���MAIN_CAMERA���w���Ă���j
};
