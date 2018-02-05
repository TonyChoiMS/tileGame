#pragma once
#include <d3dx9.h>
#include <string>
#include <vector>


class Frame;
class Texture;

class Sprite
{
private:
	//LPDIRECT3DDEVICE9 _device3d;
	//ID3DXSprite* _spriteDX;

	Texture* _texture;
	/*
	// Texture 생성을 위한 변수들
	IDirect3DTexture9* _textureDX;
	Texture* _texture;
	D3DXIMAGE_INFO _texInfo;
	*/
	float _x;
	float _y;

	std::wstring _filePath;

	//Frame* _frame;
	std::vector<Frame*> _frameList;
private:
	int _currentFrame;
	float _frameDurateion;
public:
	//Sprite(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);

	Sprite();
	~Sprite();

	// 인자에 default 값을 지정 한다.
	void Init(std::wstring textureFilename, std::wstring scriptFilename, float rotate = 0.0f);
	//void Init(std::wstring textureFilename, std::wstring scriptFilename, float rotate);
	void Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime);
	void Render();
	void Release();
	//void Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
	void Reset();
	void Update(float deltaTime);
	void SetPosition(float x, float y);


};