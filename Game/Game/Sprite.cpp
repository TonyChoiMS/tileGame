#include <reader.h>			// json library header file

#include "GameSystem.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Frame.h"
#include "Sprite.h"

Sprite::Sprite()
{
	_texture = NULL;
}

Sprite::~Sprite()
{
	for (int i = 0; i < _frameList.size(); i++)
	{
		delete _frameList[i];
	}
	_frameList.clear();
}

void Sprite::Init(std::wstring textureFilename, std::wstring scriptFilename, float rotate)
{
	_filePath = textureFilename;
	// �̹��� ���Ͽ��� �ؽ��� �ε�
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename);
	// json ��ũ��Ʈ �Ľ�
	{
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
				frame->Init(_texture, x, y, width, height, rotate, frameTime);
				_frameList.push_back(frame);
			}
		}
	}
	
	_currentFrame = 0;
	_frameDuration = 0.0f;
}

void Sprite::Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime)
{
	// �̹��� ���Ͽ��� �ؽ��� �ε�
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename);

	// json ��ũ��Ʈ �Ľ�
	Frame* frame = new Frame();
	frame->Init(_texture, x, y, width, height, 0.0f, frameTime);		// Params : ID3DXSprite*, IDirect3DTexture9*, x, y, width, height 
	_frameList.push_back(frame);

	_currentFrame = 0;
	_frameDuration = 0.0f;
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
	_texture->Release();
}

void Sprite::Reset()
{
	_texture->Reset();

	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset();
	}
}

void Sprite::SetPosition(float x, float y) {
	_x = x;
	_y = y;
}