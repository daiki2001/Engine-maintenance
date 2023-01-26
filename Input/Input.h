#pragma once
#include "./Input/DirectInput.h"

class Input final
{
public: // シングルトン化
	static Input* Get();
private:
	// コンストラクタをprivateにする
	Input() {}
	// コピーコンストラクタの削除
	Input(const Input& obj) = delete;
	// デストラクタをprivateにする
	~Input() {}
	// 代入演算子の削除
	const Input& operator=(const Input& obj) = delete;

private: // 静的メンバ変数
	// 入力デバイス
	static DirectInput* input;
	// 最新のキーボード情報用
	static char keys[256];
	// 1ループ(フレーム)前のキーボード情報
	static char oldKeys[256];
	// 最新のゲームパッド情報用
	static DIJOYSTATE gamepad;
	// 1ループ(フレーム)前のゲームパッド情報
	static DIJOYSTATE oldGamepad;

	// コントロールキーが押されているかどうか
	static bool isControl;
	// 1ループ(フレーム)前にコントロールキーが押されていたかどうか
	static bool isOldControl;

public: // 静的メンバ関数
	// 初期化
	static void Init();
	// 更新
	static void Update();
	// キーが押されているかどうか
	static bool IsKey(int key);
	// キーが押されたかどうか
	static bool IsKeyTrigger(int key);
	// キーから離れたかどうか
	static bool IsKeyReturn(int key);
	// キーが押されているかどうか
	static bool IsGamepad(DirectInput::GamepadInputType gamepadType);
	// キーが押されたかどうか
	static bool IsGamepadTrigger(DirectInput::GamepadInputType gamepadType);
	// キーから離れたかどうか
	static bool IsGamepadReturn(DirectInput::GamepadInputType gamepadType);

	// コントロールキーが押されているかどうか
	static bool IsControlKey() { return IsKey(DIK_LCONTROL) || IsKey(DIK_RCONTROL); }
	// コントロールキーが押されたかどうか
	static bool IsControlKeyTrigger() { return IsKeyTrigger(DIK_LCONTROL) || IsKeyTrigger(DIK_RCONTROL); }
	// コントロールキーから離れたかどうか
	static bool IsControlKeyReturn() { return IsKeyReturn(DIK_LCONTROL) || IsKeyReturn(DIK_RCONTROL); }
};
