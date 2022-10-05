﻿#pragma once
#include "BlockType.h"
#include "../Player.h"
#include <vector>
#include "./Header/EngineGeneral.h"

class BlockManager
{
public: //エイリアス
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum TypeId
	{
		NONE,
		WALL,
		SWITCH,
		GOAL,
		MAX
	};

	class Block
	{
	public: //メンバ変数
		Vector3 pos; //座標
		bool isSwitch;
	private:
		TypeId typeId;

	public: //メンバ関数
		Block(const TypeId& typeId);
		~Block() = default;

		const TypeId GetTypeId() const { return typeId; }
	};

private: //静的メンバ変数
	static Player* player;

private: //メンバ変数
	std::vector<BlockType> blockType;
	std::vector<Block> block;
	bool isOpen;
	bool isGoal;

public: //メンバ関数
	BlockManager();
	~BlockManager();

	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
	// リセット処理
	void Reset();

	// ブロックの生成処理
	int CreateBlock(const BlockManager::TypeId& typeId);
	// ブロックの切り替え処理
	void ChengeBlock(const int& index, const BlockManager::TypeId& typeId);
	// ブロックの削除処理
	void DeleteBlock(const int& index);
	// 全ブロックの削除処理
	void AllDeleteBlock();

	Block& GetBlock(const int& index) { return block[index]; };
	const bool GetDoor() const { return isOpen; }
	const bool GetGoal() const { return isGoal; }
private:
	// プレイヤーの押し戻し処理
	void PlayerPushBack() const;
	// スイッチが押された時の処理
	void SwitchPush(const size_t& blockNo);

	/// <summary>
	/// プレイヤーの周囲のブロックを取得する
	/// </summary>
	/// <param name="radius"> 0を指定するとプレイヤーがいる場所のブロックだけ渡す </param>
	/// <param name="surroundingBlockType"> 要素番号をまとめた配列 </param>
	/// <returns> プレイヤーが踏んでいるブロックの要素番号 </returns>
	int GetSurroundingBlock(const int& radius, BlockManager::TypeId* surroundingBlockType);
};
