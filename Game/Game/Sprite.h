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
<<<<<<< HEAD
	LPDIRECT3DDEVICE9 _device3d;
	ID3DXSprite* _spriteDX;
	
	Texture* _texture;
	////Texture 생성을 위한 변수
	//IDirect3DTexture9* _textureDX;
	//// 이미지 파일로부터 이미지 정보를 얻어온다.
	//D3DXIMAGE_INFO _texInfo;
=======
	Texture* _texture;
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30

	std::wstring _filePath;
	
	//Frame* _frame;
	std::vector<Frame*> _frameList;
	int _currentFrame;

	float _x;
	float _y;

public:
	Sprite();
	~Sprite();

	// LPCWSTR은 포인터(주소)로 넘어옴
	// std:wstring은 값으로 넘어옴
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
<<<<<<< HEAD
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);
=======
	void Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float x, float y);
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30

	//Animation
private:
	float _frameDuration;
};