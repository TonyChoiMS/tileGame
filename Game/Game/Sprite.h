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

	// LPCWSTR은 포인터(주소)로 넘어옴
	// std:wstring은 값으로 넘어옴
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