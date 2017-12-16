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
	LPDIRECT3DDEVICE9 _device3d;
	ID3DXSprite* _spriteDX;
	
	Texture* _texture;
	////Texture ������ ���� ����
	//IDirect3DTexture9* _textureDX;
	//// �̹��� ���Ϸκ��� �̹��� ������ ���´�.
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

	// LPCWSTR�� ������(�ּ�)�� �Ѿ��
	// std:wstring�� ������ �Ѿ��
	void Init(std::wstring textureFilename, std::wstring scriptFilename);
	void Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX);

	void SetPosition(float x, float y);

	//Animation
private:
	float _frameDuration;
};