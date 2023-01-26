#include "GameInput.h"

GameInput::GameInput() :
	mainUpKey(DIK_W),
	mainDownKey(DIK_S),
	mainLeftKey(DIK_A),
	mainRightKey(DIK_D),
	subUpKey(DIK_UP),
	subDownKey(DIK_DOWN),
	subLeftKey(DIK_LEFT),
	subRightKey(DIK_RIGHT),
	decisionKey(DIK_SPACE),
	mainUp1Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_UP),
	mainDown1Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_DOWN),
	mainLeft1Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_LEFT),
	mainRight1Gamepad(DirectInput::GamepadInputType::PAD_LSTICK_RIGHT),
	mainUp2Gamepad(DirectInput::GamepadInputType::PAD_POV_UP),
	mainDown2Gamepad(DirectInput::GamepadInputType::PAD_POV_DOWN),
	mainLeft2Gamepad(DirectInput::GamepadInputType::PAD_POV_LEFT),
	mainRight2Gamepad(DirectInput::GamepadInputType::PAD_POV_RIGHT),
	subUpGamepad(DirectInput::GamepadInputType::PAD_RSTICK_UP),
	subDownGamepad(DirectInput::GamepadInputType::PAD_RSTICK_DOWN),
	subLeftGamepad(DirectInput::GamepadInputType::PAD_RSTICK_LEFT),
	subRightGamepad(DirectInput::GamepadInputType::PAD_RSTICK_RIGHT),
	decisionGamepad(DirectInput::GamepadInputType::PAD_BUTTON_A)
{
}

GameInput* GameInput::Get()
{
	static GameInput instance = {};
	return &instance;
}

const bool GameInput::MainUp() const
{
	return Input::IsKey(mainUpKey) ||
		Input::IsGamepad(mainUp1Gamepad) ||
		Input::IsGamepad(mainUp2Gamepad);
}

const bool GameInput::MainUpTrigger() const
{
	return Input::IsKeyTrigger(mainUpKey) ||
		Input::IsGamepadTrigger(mainUp1Gamepad) ||
		Input::IsGamepadTrigger(mainUp2Gamepad);
}

const bool GameInput::MainUpReturn() const
{
	return Input::IsKeyReturn(mainUpKey) ||
		Input::IsGamepadReturn(mainUp1Gamepad) ||
		Input::IsGamepadReturn(mainUp2Gamepad);
}

const bool GameInput::MainDown() const
{
	return Input::IsKey(mainDownKey) ||
		Input::IsGamepad(mainDown1Gamepad) ||
		Input::IsGamepad(mainDown2Gamepad);
}

const bool GameInput::MainDownTrigger() const
{
	return Input::IsKeyTrigger(mainDownKey) ||
		Input::IsGamepadTrigger(mainDown1Gamepad) ||
		Input::IsGamepadTrigger(mainDown2Gamepad);
}

const bool GameInput::MainDownReturn() const
{
	return Input::IsKeyReturn(mainDownKey) ||
		Input::IsGamepadReturn(mainDown1Gamepad) ||
		Input::IsGamepadReturn(mainDown2Gamepad);
}

const bool GameInput::MainLeft() const
{
	return Input::IsKey(mainLeftKey) ||
		Input::IsGamepad(mainLeft1Gamepad) ||
		Input::IsGamepad(mainLeft2Gamepad);
}

const bool GameInput::MainLeftTrigger() const
{
	return Input::IsKeyTrigger(mainLeftKey) ||
		Input::IsGamepadTrigger(mainLeft1Gamepad) ||
		Input::IsGamepadTrigger(mainLeft2Gamepad);
}

const bool GameInput::MainLeftReturn() const
{
	return Input::IsKeyReturn(mainLeftKey) ||
		Input::IsGamepadReturn(mainLeft1Gamepad) ||
		Input::IsGamepadReturn(mainLeft2Gamepad);
}

const bool GameInput::MainRight() const
{
	return Input::IsKey(mainRightKey) ||
		Input::IsGamepad(mainRight1Gamepad) ||
		Input::IsGamepad(mainRight2Gamepad);
}

const bool GameInput::MainRightTrigger() const
{
	return Input::IsKeyTrigger(mainRightKey) ||
		Input::IsGamepadTrigger(mainRight1Gamepad) ||
		Input::IsGamepadTrigger(mainRight2Gamepad);
}

const bool GameInput::MainRightReturn() const
{
	return Input::IsKeyReturn(mainRightKey) ||
		Input::IsGamepadReturn(mainRight1Gamepad) ||
		Input::IsGamepadReturn(mainRight2Gamepad);
}

const bool GameInput::SubUp() const
{
	return Input::IsKey(subUpKey) || Input::IsGamepad(subUpGamepad);
}

const bool GameInput::SubUpTrigger() const
{
	return Input::IsKeyTrigger(subUpKey) || Input::IsGamepadTrigger(subUpGamepad);
}

const bool GameInput::SubUpReturn() const
{
	return Input::IsKeyReturn(subUpKey) || Input::IsGamepadReturn(subUpGamepad);
}

const bool GameInput::SubDown() const
{
	return Input::IsKey(subDownKey) || Input::IsGamepad(subDownGamepad);
}

const bool GameInput::SubDownTrigger() const
{
	return Input::IsKeyTrigger(subDownKey) || Input::IsGamepadTrigger(subDownGamepad);
}

const bool GameInput::SubDownReturn() const
{
	return Input::IsKeyReturn(subDownKey) || Input::IsGamepadReturn(subDownGamepad);
}

const bool GameInput::SubLeft() const
{
	return Input::IsKey(subLeftKey) || Input::IsGamepad(subLeftGamepad);
}

const bool GameInput::SubLeftTrigger() const
{
	return Input::IsKeyTrigger(subLeftKey) || Input::IsGamepadTrigger(subLeftGamepad);
}

const bool GameInput::SubLeftReturn() const
{
	return Input::IsKeyReturn(subLeftKey) || Input::IsGamepadReturn(subLeftGamepad);
}

const bool GameInput::SubRight() const
{
	return Input::IsKey(subRightKey) || Input::IsGamepad(subRightGamepad);
}

const bool GameInput::SubRightTrigger() const
{
	return Input::IsKeyTrigger(subRightKey) || Input::IsGamepadTrigger(subRightGamepad);
}

const bool GameInput::SubRightReturn() const
{
	return Input::IsKeyReturn(subRightKey) || Input::IsGamepadReturn(subRightGamepad);
}

const bool GameInput::Decision() const
{
	return Input::IsKey(decisionKey) || Input::IsGamepad(decisionGamepad);
}

const bool GameInput::DecisionTrigger() const
{
	return Input::IsKeyTrigger(decisionKey) || Input::IsGamepadTrigger(decisionGamepad);
}

const bool GameInput::DecisionReturn() const
{
	return Input::IsKeyReturn(decisionKey) || Input::IsGamepadReturn(decisionGamepad);
}
