#include "Sprite.h"
#include "Frame.h"
Sprite::Sprite(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_device3d = device3d;
	_spriteDX = spriteDX;
}

Sprite::~Sprite()
{
	if (NULL != _textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}
	for (int i = 0; i < _frameList.size(); i++)
	{
		delete _frameList[i];
	}
	_frameList.clear();
	/*
	if (NULL != _frame)
	{
		delete _frame;
		_frame = NULL;

	}
	*/
}

void Sprite::Init(std::wstring filePath)
{
	
	_filePath = filePath;
	//std::wstring.c_str() -> std::wstring을 LPCWSTR값으로 바꿔줌
	HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	if (FAILED(hr))
	{
		return;
	}

	hr = D3DXCreateTextureFromFileEx(
		_device3d,			// Direct3D
		filePath.c_str(),			// 파일 경로
		_texInfo.Width,		// 이미지의 가로
		_texInfo.Height,		// 이미지 세로
		1,					// 이미지가 가까워지거나 멀어질 때 퀄리티 설정
		0,
		D3DFMT_UNKNOWN,		// 현재 결정된 윈도우를 따라감
		D3DPOOL_DEFAULT,	// 자원 관리를 기본적인 윈도우 시스템을 따라감
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),		// Color Key - 해당 색은 화면에 출력이 안됨(다른 것으로 대체 될 색)
		&_texInfo,
		NULL,					// Pallete
		&_textureDX);
	if (FAILED(hr))
	{
		return;
	}
	{
		Frame* frame = new Frame();
		frame->Init(_spriteDX, _textureDX, 32, 0, 32, 32);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
		_frameList.push_back(frame);
	}
	{
		Frame* frame = new Frame();
		frame->Init(_spriteDX, _textureDX, 0, 0, 32, 32);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
		_frameList.push_back(frame);
	}
	{
		Frame* frame = new Frame();
		frame->Init(_spriteDX, _textureDX, 64, 0, 32, 32);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
		_frameList.push_back(frame);
	}
	_currentFrame = 2;
}

void Sprite::Update(float deltaTime)
{
	_frameDuration += deltaTime;
	if (_frameList[_currentFrame]->GetFrameTime() <= _frameDuration)
	{
		_frameDuration = 0.0f;
		_currentFrame = (_currentFrame + 1) % _frameList.size();
	}
}

void Sprite::Render()
{
	if (_currentFrame < _frameList.size()) {
		_frameList[_currentFrame]->Render();
	}		
}

void Sprite::Release()
{
	//_frame->Release();
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Release();
	}
	if (_textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}
}

void Sprite::Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_device3d = device3d;
	_spriteDX = spriteDX;

	//Texture 로드
	HRESULT hr = D3DXCreateTextureFromFileEx(
		device3d,			// Direct3D
		_filePath.c_str(),			// 파일 경로
		_texInfo.Width,		// 이미지의 가로
		_texInfo.Height,		// 이미지 세로
		1,					// 이미지가 가까워지거나 멀어질 때 퀄리티 설정
		0,
		D3DFMT_UNKNOWN,		// 현재 결정된 윈도우를 따라감
		D3DPOOL_DEFAULT,	// 자원 관리를 기본적인 윈도우 시스템을 따라감
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),		// Color Key - 해당 색은 화면에 출력이 안됨(다른 것으로 대체 될 색)
		&_texInfo,
		NULL,					// Pallete
		&_textureDX);

	//_frame->Reset();
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset();
	}
}