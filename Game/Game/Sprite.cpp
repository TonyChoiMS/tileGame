<<<<<<< HEAD

=======
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
#include <reader.h>			// json library header file

#include "ResourceManager.h"
#include "Texture.h"
#include "Frame.h"
#include "Sprite.h"

<<<<<<< HEAD
Sprite::Sprite(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_device3d = device3d;
	_spriteDX = spriteDX;
=======
Sprite::Sprite()
{
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	_texture = NULL;
}

Sprite::~Sprite()
{
<<<<<<< HEAD
	/*if (NULL != _textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}*/
	if (NULL != _texture) {
		_texture->Release();
		delete _texture;
		_texture = NULL;
	}
=======
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	for (int i = 0; i < _frameList.size(); i++)
	{
		delete _frameList[i];
	}
	_frameList.clear();
<<<<<<< HEAD
}

void Sprite::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	_filePath = textureFilename;
	// 이미지 파일에서 텍스쳐 로드
	//_texture = new Texture(_device3d, filePath);
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename, _device3d);
	// json 스크립트 파싱
	{
		//std::wstring scriptFileName = L"TestScript.json";

		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(scriptFilename);
		for (int i = 0; i < recordList.size(); i++) 
		{
			Json::Value root;
			Json::Reader reader;
			bool isSuccess = reader.parse(recordList[i], root);
			if(isSuccess)
			{
				std::string textureFilename = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();
				int width = root["width"].asInt();
				int height = root["height"].asInt();
				double frameTime = root["frameTime"].asDouble();

				Frame* frame = new Frame();
				frame->Init(_spriteDX, _texture, x, y, width, height, frameTime);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
				_frameList.push_back(frame);
			}
		}
	}
	//std::wstring.c_str() -> std::wstring을 LPCWSTR값으로 바꿔줌
	//HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	//if (FAILED(hr))
	//{
	//	return;
	//}

	//hr = D3DXCreateTextureFromFileEx(
	//	_device3d,			// Direct3D
	//	filePath.c_str(),			// 파일 경로
	//	_texInfo.Width,		// 이미지의 가로
	//	_texInfo.Height,		// 이미지 세로
	//	1,					// 이미지가 가까워지거나 멀어질 때 퀄리티 설정
	//	0,
	//	D3DFMT_UNKNOWN,		// 현재 결정된 윈도우를 따라감
	//	D3DPOOL_DEFAULT,	// 자원 관리를 기본적인 윈도우 시스템을 따라감
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	D3DCOLOR_ARGB(255, 255, 255, 255),		// Color Key - 해당 색은 화면에 출력이 안됨(다른 것으로 대체 될 색)
	//	&_texInfo,
	//	NULL,					// Pallete
	//	&_textureDX);
	//if (FAILED(hr))
	//{
	//	return;
	//}
	
	//{
	//	Frame* frame = new Frame();
	//	frame->Init(_spriteDX, _texture, 32, 0, 32, 32, 0.2f);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
	//	_frameList.push_back(frame);
	//}
	//{
	//	Frame* frame = new Frame();
	//	frame->Init(_spriteDX, _texture, 0, 0, 32, 32, 0.2f);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
	//	_frameList.push_back(frame);
	//}
	//{
	//	Frame* frame = new Frame();
	//	frame->Init(_spriteDX, _texture, 64, 0, 32, 32, 0.2f);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
	//	_frameList.push_back(frame);
	//}
	_currentFrame = 0;
	_frameDuration = 0.0f;
		
=======
}

void Sprite::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	_filePath = textureFilename;
	// 이미지 파일에서 텍스쳐 로드
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename);
	// json 스크립트 파싱
	{

		std::vector<std::string> recordList = ResourceManager::GetInstance()->FindScript(scriptFilename);
		for (int i = 0; i < recordList.size(); i++)
		{
			Json::Value root;
			Json::Reader reader;
			bool isSuccess = reader.parse(recordList[i], root);
			if (isSuccess)
			{
				std::string textureFilename = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();
				int width = root["width"].asInt();
				int height = root["height"].asInt();
				double frameTime = root["frameTime"].asDouble();

				Frame* frame = new Frame();
				frame->Init(_texture, x, y, width, height, frameTime);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
				_frameList.push_back(frame);
			}
		}
	}

	_currentFrame = 0;
	_frameDuration = 0.0f;
}

void Sprite::Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime)
{
	// 이미지 파일에서 텍스쳐 로드
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename);

	// json 스크립트 파싱
	Frame* frame = new Frame();
	frame->Init(_texture, x, y, width, height, frameTime);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
	_frameList.push_back(frame);

	_currentFrame = 0;
	_frameDuration = 0.0f;
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
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
		_frameList[_currentFrame]->SetPosition(_x, _y);
		_frameList[_currentFrame]->Render();
	}
}

void Sprite::Release()
{
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Release();
	}
<<<<<<< HEAD
	/*if (_textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}*/
=======
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	_texture->Release();
}

void Sprite::Reset()
{
<<<<<<< HEAD
	_device3d = device3d;
	_spriteDX = spriteDX;

	_texture->Reset(_device3d);
	//Texture 로드
	//HRESULT hr = D3DXCreateTextureFromFileEx(
	//	device3d,			// Direct3D
	//	_filePath.c_str(),			// 파일 경로
	//	_texInfo.Width,		// 이미지의 가로
	//	_texInfo.Height,		// 이미지 세로
	//	1,					// 이미지가 가까워지거나 멀어질 때 퀄리티 설정
	//	0,
	//	D3DFMT_UNKNOWN,		// 현재 결정된 윈도우를 따라감
	//	D3DPOOL_DEFAULT,	// 자원 관리를 기본적인 윈도우 시스템을 따라감
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	D3DCOLOR_ARGB(255, 255, 255, 255),		// Color Key - 해당 색은 화면에 출력이 안됨(다른 것으로 대체 될 색)
	//	&_texInfo,
	//	NULL,					// Pallete
	//	&_textureDX);

	//_frame->Reset();
=======
	_texture->Reset();

>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset(_spriteDX);
	}
}

void Sprite::SetPosition(float x, float y) {
	_x = x;
	_y = y;
}