#pragma once
#include <d3dx9.h>
//std library는 include에 .h가 붙지 않는다.
#include <d3dx9.h>
#include <string>
#include <vector>

class Frame;
class Texture;

class Sprite
{
private:
	LPDIRECT3DDEVICE9 _device3d;
	ID3DXSprite* _spriteDX;
	
	Texture* _texture;
	////Texture 생성을 위한 변수
	//IDirect3DTexture9* _textureDX;
	//// 이미지 파일로부터 이미지 정보를 얻어온다.
	//D3DXIMAGE_INFO _texInfo;

	std::wstring _filePath;
	
	//Frame* _frame;
	std::vector<Frame*> _frameList;
	int _currentFrame;

	float _x;
	float _y;

public:
	Sprite(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
	~Sprite();

	// LPCWSTR은 포인터(주소)로 넘어옴
	// std:wstring은 값으로 넘어옴
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);

	void SetPosition(float x, float y);

	//Animation
private:
	float _frameDuration;
};