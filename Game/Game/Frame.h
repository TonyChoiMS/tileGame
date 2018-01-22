#pragma once

#include <d3dx9.h>

class Texture;

class Frame
{
private:
	Texture* _texture;

	//Texture 생성을 위한 변수
	RECT _textureRect;
	D3DCOLOR _textureColor;

	float _width;
	float _height;
	float _rotate;

	float _x;
	float _y;

public:
	Frame();
	~Frame();

	void Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float x, float y);

private:
	float _frameTime;

public:
	float GetFrameTime() { return _frameTime; }
};