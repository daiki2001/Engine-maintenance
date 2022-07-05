#pragma once
#include "BlockType.h"
#include <vector>

class BlockManager final
{
public: //�V���O���g����
	static BlockManager* Get();
private:
	BlockManager();
	BlockManager(const BlockManager&) = delete;
	~BlockManager();
	BlockManager operator=(const BlockManager&) = delete;

public: //�T�u�N���X
	enum TypeId
	{
		NONE
	};

	class Block
	{
	public: //�����o�ϐ�
		int posX = 0;
		int posY = 0;
	private:
		TypeId typeId = BlockManager::TypeId::NONE;

	public: //�����o�֐�
		Block() = default;
		~Block() = default;

		const TypeId GetTypeId() const { return typeId; }
		void SetTypeId(const TypeId& typeId) { this->typeId = typeId; }
	};

private: //�����o�ϐ�
	std::vector<BlockType> blockType;
	std::vector<Block> block;

public: //�����o�֐�
	// ����������
	void Init(DrawPolygon* const draw);
	// �X�V����
	void Update();
	// �`�揈��
	void Draw();

	// �u���b�N�̐�������
	int CreateBlock(const BlockManager::TypeId& typeId);

	Block& GetBlock(const int& index) { return block[index]; };
};
