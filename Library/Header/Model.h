#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <fbxsdk.h>
#include <string>
#include <vector>
#include <DirectXTex.h>
#include "./Math/EngineMath.h"
#include "./Camera.h"

struct Node
{
	std::string name;                      //���O
	Engine::Math::Vector3 position;        //���[�J�����W
	Engine::Math::Matrix4 rotation;        //���[�J����]�s��
	Engine::Math::Vector3 scaling;         //���[�J���X�P�[��
	Engine::Math::Matrix4 transform;       //���[�J���ϊ��s��
	Engine::Math::Matrix4 globalTransform; //�O���[�o���ϊ��s��
	Node* parent;                          //�e�m�[�h

	Node();
};

class Model
{
private: // �G�C���A�X
	using Vector3 = Engine::Math::Vector3;
	using Matrix4 = Engine::Math::Matrix4;
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template<class T> using vector = std::vector<T>;

public: // �t�����h�N���X
	friend class FbxLoader;

public: // �萔
	static const int MAX_BONE_INDICES = 4;
	static const int MAX_BONE = 32;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		Vector3 pos;          //xyz���W
		Vector3 normal;       //�@���x�N�g��
		DirectX::XMFLOAT2 uv; //uv���W

		UINT boneIndex[MAX_BONE_INDICES];   //�{�[�� �ԍ�
		float boneWeight[MAX_BONE_INDICES]; //�{�[�� �d��
	};
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		Matrix4 viewProj;  //�r���[�v���W�F�N�V�����s��
		Matrix4 world;     //���[���h�s��
		Vector3 cameraPos; //�J�������W
	};
	// �{�[���\����
	struct Bone
	{
		std::string name = {};                          //���O
		Matrix4 invInitPose = Engine::Math::Identity(); //�����p���̋t�s��
		FbxCluster* fbxCluster = nullptr;               //�N���X�^�[
	};
	// �萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		Matrix4 bones[MAX_BONE]; //�{�[���̃X�L�j���O�s��
	};

private: // �ÓI�����o�ϐ�
	static ID3D12Device* dev;

	static ComPtr<ID3D12RootSignature> rootSignature; //���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> pipelineState; //�p�C�v���C���X�e�[�g

public: // �ÓI�����o�֐�
	static HRESULT CreateGraphicsPipeline();

public: // �����o�ϐ�
	Vector3 pos;   //���[�J�����W
	Matrix4 rota;  //��]�s��
	Vector3 scale; //�X�P�[��
	Matrix4 world; //���[���h�s��

	FbxTime frameTime;   //1�t���[���̎���
	FbxTime startTime;   //�A�j���[�V�����J�n����
	FbxTime endTime;     //�A�j���[�V�����I������
	FbxTime currentTime; //���ݎ���
	bool isPlay;         //�A�j���[�V�����Đ���
private:
	std::string name;      //���f����
	std::string directory; //���f��������f�B���N�g���p�X
	vector<Node> nodes;    //�m�[�h�z��

	Node* meshNode;                         //���b�V�������m�[�h
	vector<VertexPosNormalUvSkin> vertices; //���_�f�[�^�z��
	vector<unsigned short> indices;         //���_�C���f�b�N�X�z��

	ComPtr<ID3D12Resource> vertBuff;          //���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;         //�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> texBuff;           //�e�N�X�`���o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView;          //���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;           //�C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuff;         //�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffSkin;     //�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12DescriptorHeap> descHeapSRV; //SRV�p�f�X�N���v�^�q�[�v

	Vector3 ambient;                  //�A���r�G���g�W��
	Vector3 diffuse;                  //�f�B�t���[�Y�W��
	DirectX::TexMetadata metadata;    //�e�N�X�`�����^�f�[�^
	DirectX::ScratchImage scratchImg; //�X�N���b�`�C���[�W

	vector<Bone> bones; //�{�[���z��

	FbxScene* fbxScene; //FBX�V�[��

public: // �����o�֐�
	Model();
	~Model();

	// ������
	void Init();
	// �X�V����
	int Update();
	// �`��
	void Draw();
	// �I������
	void Finalize();

	// �e��o�b�t�@�̐���
	HRESULT CreateBuffers();
	HRESULT CreateConstBuffer();

	// �A�j���[�V�����J�n
	void PlayAnimation();

	const Matrix4& GetModelTransform() { return meshNode->globalTransform; }
	vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }
};
