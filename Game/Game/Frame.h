#pragma once
#include <d3dx9.h>

class Texture;

class Frame
{

private:
	//ID3DXSprite* _spriteDX;
	//IDirect3DTexture9* _textureDX;

	Texture* _texture;

	RECT _textureRect;
	D3DCOLOR _textureColor;

private:
	float _frameTime;
	float _width;
	float _height;
	float _x;
	float _y;
	float _rotate;
public:
	Frame();
	~Frame();

	//void Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX, int x, int y, int width, int height, float frameTime);
	//void Init(ID3DXSprite* spriteDX, Texture* texture, int x, int y, int width, int height, float frameTime);
	void Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameTime);
	void Render();
	void Relase();
	//void Reset(ID3DXSprite* spriteDX);
	void Reset();
	void SetPosition(float x, float y);

	float GetFrameTime();

};