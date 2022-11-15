#include "./Header/Input.h"

DirectInput* Input::input = {};
char Input::keys[256] = {};
char Input::oldKeys[256] = {};
DIJOYSTATE Input::gamepad = {};
DIJOYSTATE Input::oldGamepad = {};

Input* Input::Get()
{
	static Input instance = {};
	return &instance;
}

void Input::Init()
{
	input = DirectInput::GetInstance();
	DirectInput::Init();
}

void Input::Update()
{
	input->Update();

	for (size_t i = 0; i < 256; i++)
	{
		oldKeys[i] = keys[i];
	}
	oldGamepad = gamepad;

	input->GetHitKeyStateAll(keys);
	input->GetGamepadStateAll(&gamepad);
}

bool Input::IsKey(int keyCode)
{
	return keys[keyCode];
}

bool Input::IsKeyTrigger(int keyCode)
{
	return keys[keyCode] && !oldKeys[keyCode];
}

bool Input::IsKeyReturn(int keyCode)
{
	return !keys[keyCode] && oldKeys[keyCode];
}

bool Input::IsGamepad(DirectInput::GamepadInputType gamepadType)
{
	return input->CheckHitGamepad(gamepadType, gamepad);
}

bool Input::IsGamepadTrigger(DirectInput::GamepadInputType gamepadType)
{
	return input->CheckHitGamepad(gamepadType, gamepad) &&
		!input->CheckHitGamepad(gamepadType, oldGamepad);
}

bool Input::IsGamepadReturn(DirectInput::GamepadInputType gamepadType)
{
	return !input->CheckHitGamepad(gamepadType, gamepad) &&
		input->CheckHitGamepad(gamepadType, oldGamepad);
}
