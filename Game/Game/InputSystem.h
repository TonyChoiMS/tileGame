#pragma once

class InputSystem
{
private:
	static InputSystem* _instance;
public:
	static InputSystem* GetInstance();
private:
	InputSystem();
	~InputSystem() {}
public:
	enum ekeyState
	{
		KEY_DOWN,
		KEY_UP
	};
private:
	bool _isMouseDown;
	int _mouseX;
	int _mouseY;
public:
	void MouseDown(int x, int y);
	void MouseUp();
	bool IsMouseDown();
	int GetMouseX();
	int GetMouseY();
private:
	unsigned int _keyState[256];
public:
	void KeyDown(unsigned int keyCode);
	void KeyUp(unsigned int keyCode);
	bool IsKeyDown(unsigned int keyCode);
};