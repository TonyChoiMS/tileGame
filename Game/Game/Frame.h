#pragma once

#include <d3dx9.h>

class Texture;

class Frame
{
private:
	ID3DXSprite* _spriteDX;
//	IDirect3DTexture9* _textureDX;
	Texture* _texture;

	//Texture 생성을 위한 변수
	RECT _textureRect;
	D3DCOLOR _textureColor;

	float _width;
	float _height;

	float _x;
	float _y;

public:
	Frame();
	~Frame();

	void Init(ID3DXSprite* spriteDX, Texture* texture, int x, int y, int width, int height, float frameTime);
	void Render();
	void Release();
	//void Reset(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX);
	void Reset(ID3DXSprite* spriteDX);

	void SetPosition(float x, float y);

private:
	float _frameTime;

public:
	float GetFrameTime() { return _frameTime; }
};