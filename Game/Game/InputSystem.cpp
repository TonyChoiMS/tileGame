#include "InputSystem.h"

InputSystem* InputSystem::_instance = 0;

InputSystem* InputSystem::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new InputSystem();
	}
	return _instance;
}
InputSystem::InputSystem()
{
	for (int i = 0; i < 256; i++)
	{
		_keyState[i] = ekeyState::KEY_UP;
	}
}

void InputSystem::MouseDown(int x, int y)
{
	_isMouseDown = true;
	_mouseX = x;
	_mouseY = y;
}

void InputSystem::MouseUp()
{
	_isMouseDown = false;
}

bool InputSystem::IsMouseDown()
{
	return _isMouseDown;
}
int InputSystem::GetMouseX()
{
	return _mouseX;
}
int InputSystem::GetMouseY()
{
	return _mouseY;
}

void InputSystem::KeyDown(unsigned int keyCode)
{
	_keyState[keyCode] = ekeyState::KEY_DOWN;
}
void InputSystem::KeyUp(unsigned int keyCode)
{
	_keyState[keyCode] = ekeyState::KEY_UP;

}

bool InputSystem::IsKeyDown(unsigned int keyCode)
{
	if (ekeyState::KEY_DOWN == _keyState[keyCode])
	{
		return true;
	}
	return false;
}