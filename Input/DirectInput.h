#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定

#include <dinput.h>
#include <vector>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// CheckHitAll で調べる入力タイプ
#define CHECKINPUT_KEY   (0x01) //キー入力を調べる
#define CHECKINPUT_PAD   (0x02) //パッド入力を調べる
#define CHECKINPUT_MOUSE (0x04) //マウスボタン入力を調べる
#define CHECKINPUT_ALL   (CHECKINPUT_KEY | CHECKINPUT_PAD | CHECKINPUT_MOUSE) //すべての入力を調べる

class DirectInput final
{
public:
	// インスタンスの取得
	static DirectInput* GetInstance();
private: // シングルトン化
	// コンストラクタをprivateにする
	DirectInput() {};
	// コピーコンストラクタを無効にする
	DirectInput(const DirectInput& obj) = delete;
	// デストラクタをprivateにする
	~DirectInput() {}
	// 代入演算子を無効にする
	const DirectInput& operator=(const DirectInput& obj) = delete;

public: // サブクラス
	enum GamepadInputType
	{
		// ボタン
		PAD_BUTTON_A = 0,           //Aボタン
		PAD_BUTTON_B = 1,           //Bボタン
		PAD_BUTTON_X = 2,           //Xボタン
		PAD_BUTTON_Y = 3,           //Yボタン
		PAD_BUTTON_LB = 4,          //LBボタン
		PAD_BUTTON_RB = 5,          //RBボタン
		PAD_BUTTON_BACK = 6,        //BACKボタン
		PAD_BUTTON_START = 7,       //STARTボタン
		PAD_BUTTON_LSTICK_PUSH = 8, //Lスティック押し込み
		PAD_BUTTON_RSTICK_PUSH = 9, //Rスティック押し込み
		// 十字キー
		PAD_POV_UP = 32,    //上入力
		PAD_POV_UP_RIGHT,   //右上入力
		PAD_POV_RIGHT,      //右入力
		PAD_POV_DOWN_RIGHT, //右下入力
		PAD_POV_DOWN,       //下入力
		PAD_POV_DOWN_LEFT,  //左下入力
		PAD_POV_LEFT,       //左入力
		PAD_POV_UP_LEFT,    //左上入力
		// 左右トリガー
		PAD_TRIGGER_LEFT,  //左トリガー
		PAD_TRIGGER_RIGHT, //右トリガー
		// 左スティック
		PAD_LSTICK_UP,    //左スティックの上入力
		PAD_LSTICK_RIGHT, //左スティックの右入力
		PAD_LSTICK_DOWN,  //左スティックの下入力
		PAD_LSTICK_LEFT,  //左スティックの左入力
		// 右スティック
		PAD_RSTICK_UP,    //右スティックの上入力
		PAD_RSTICK_RIGHT, //右スティックの右入力
		PAD_RSTICK_DOWN,  //右スティックの下入力
		PAD_RSTICK_LEFT,  //右スティックの左入力
	};

public: // 定数
	static const LONG DEADZONE; //スティック入力の無効範囲

private: // 静的メンバ変数
	// DirectInputインターフェース
	static IDirectInput8* dinput;
	// キーボードデバイス
	static IDirectInputDevice8* devkeyborad;
	// ゲームパッドデバイス
	static std::vector<IDirectInputDevice8*> devgamepad;

	// キーボードの入力情報
	static BYTE key[256];
	// ゲームパッドの入力情報
	static std::vector<DIJOYSTATE> gamepads;

	static std::vector<DIDEVICEINSTANCE> parameter;

public: // 静的メンバ関数
	// 初期化
	static int Init();
	// 更新
	static int Update();
	// デバイスの解放
	static void Release();

	// 特定のキーボードの入力状態の取得
	static bool CheckHitKey(int keyCode);
	// 特定のゲームパッドの入力状態の取得
	static bool CheckHitGamepad(GamepadInputType inputType, const DIJOYSTATE& gamepad);
	// 特定のゲームパッドのアナログスティックの入力状態の取得
	static bool CheckHitGamepadAnalogStick(GamepadInputType inputType, const DIJOYSTATE& gamepad);
	// 全ての入力デバイスの状態の取得
	static int CheckHitAll(int CheckType = CHECKINPUT_ALL);
	// キーボードデバイスの入力状態の取得
	static int GetHitKeyStateAll(char* KeyStateArray);
	// 特定のゲームパッドデバイスの生の入力状態の取得
	static int GetGamepadStateAll(DIJOYSTATE* gamepadState, const size_t& gamepadNo = 0);
private:
	// デバイスを列挙する時のコールバック関数
	static BOOL CALLBACK EnumGamepadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	// キーボードデバイスの更新
	static int KeyInputUpdate();
	// ゲームパッドデバイスの更新
	static int GamepadInputUpdate(const size_t& gamepadNo = 0);
	// ゲームパッドデバイスの生成
	static HRESULT CreateGamePadDevice();
	// プロパティの設定
	static HRESULT SetUpGamePadProperty(const size_t& gamepadNo = 0);
};
