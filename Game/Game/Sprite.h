#pragma once
#include <d3dx9.h>
//std library�� include�� .h�� ���� �ʴ´�.
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
	////Texture ������ ���� ����
	//IDirect3DTexture9* _textureDX;
	//// �̹��� ���Ϸκ��� �̹��� ������ ���´�.
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

	// LPCWSTR�� ������(�ּ�)�� �Ѿ��
	// std:wstring�� ������ �Ѿ��
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