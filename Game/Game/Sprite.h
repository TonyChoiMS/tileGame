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
	Texture* _texture;

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
	void Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime);
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float x, float y);

	//Animation
private:
	float _frameDuration;
};