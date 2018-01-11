#pragma once

#include <d3dx9.h>

class Texture;

class Frame
{
private:
<<<<<<< HEAD
	ID3DXSprite* _spriteDX;
//	IDirect3DTexture9* _textureDX;
=======
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	Texture* _texture;

	//Texture 생성을 위한 변수
	RECT _textureRect;
	D3DCOLOR _textureColor;

	float _width;
	float _height;

<<<<<<< HEAD
=======
	float _x;
	float _y;

>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
public:
	Frame();
	~Frame();

<<<<<<< HEAD
	void Init(ID3DXSprite* spriteDX, Texture* texture, int x, int y, int width, int height, float frameTime);
	void Render();
	void Release();
	//void Reset(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX);
	void Reset(ID3DXSprite* spriteDX);
=======
	void Init(Texture* texture, int x, int y, int width, int height, float frameTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float x, float y);
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30

private:
	float _frameTime;

public:
	float GetFrameTime() { return _frameTime; }
};