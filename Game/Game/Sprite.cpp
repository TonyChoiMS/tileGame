
#include <reader.h>		// json lib

#include "Sprite.h"
#include "Frame.h"
#include "Texture.h"
#include "ResourceManager.h"
Sprite::Sprite()
{
	//_device3d = device3d;
	//_spriteDX = spriteDX;
	_texture = NULL;

}
Sprite::~Sprite()
{
	/*
	if (_textureDX)
	{
	_textureDX->Release();
	_textureDX = NULL;
	}
	*/
	/*
	if (NULL != _frame)
	{
	delete _frame;
	_frame = NULL;
	}
	*/

	// 
	/*
	if (NULL != _texture)
	{
	_texture->Release();
	delete _texture;
	_texture = NULL;
	}
	*/

	for (int i = 0; i < _frameList.size(); i++)
	{
		delete _frameList[i];
	}
	_frameList.clear();
}

void Sprite::Init(std::wstring textureFilename, std::wstring scriptFilename, float rotate)
{
	//HRESULT hr;

	_filePath = textureFilename;

	// �̹��� ���Ͽ��� �ؽ��� �ε�
	_texture = ResourceManager::GetInstance()->FindTexture(_filePath);

	// json ��ũ��Ʈ ����
	{
		//std::wstring scriptFilename = L"TestScript.json";

		std::vector<std::string>recordList = ResourceManager::GetInstance()->FindScript(scriptFilename);
		for (int i = 0; i < recordList.size(); i++)
		{

			Json::Value root;		// ���δ� Ʈ���� �Ǿ� �ִ�.
			Json::Reader reader;
			//bool isSucces = reader.parse(recordString, root);
			bool isSucces = reader.parse(recordList[i], root);
			if (isSucces)
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



	//_texture = new Texture(_device3d, _filePath);


	/*
	// �̹��� ���Ϸ� ���� �̹��� ������ ���´�.

	hr = D3DXGetImageInfoFromFile(_filePath.c_str(), &_texInfo);
	if (FAILED(hr))
	{
	return ;
	}

	hr = D3DXCreateTextureFromFileEx(
	_device3d,
	_filePath.c_str() ,
	_texInfo.Width,
	_texInfo.Height,
	1,
	0,
	D3DFMT_UNKNOWN,
	D3DPOOL_DEFAULT,
	D3DX_DEFAULT,
	D3DX_DEFAULT,
	D3DCOLOR_ARGB(255, 255, 255, 255),					// color key (����) ����Ҷ� Ư�� ���� ����.
	&_texInfo,
	NULL,												// �ȷ�Ʈ
	&_textureDX);

	if (FAILED(hr))
	{
	return ;
	}
	*/

	/*
	{
	Frame* frame = new Frame();
	frame->Init(_spriteDX, _texture, 0, 32, 32, 32, 0.2f);
	_frameList.push_back(frame);
	}

	{
	Frame* frame = new Frame();
	frame->Init(_spriteDX, _texture, 32, 32, 32, 32, 0.2f);
	_frameList.push_back(frame);
	}

	{
	Frame* frame = new Frame();
	frame->Init(_spriteDX, _texture, 64, 32, 32, 32, 0.2f);
	_frameList.push_back(frame);
	}
	*/
	_currentFrame = 0;
	_frameDurateion = 0;

	/*
	// �̹������� ��¿� ���� ������ �����Ѵ�.

	_textureRect.left = 0;									//
	_textureRect.right = _textureRect.left + _texInfo.Width;	//
	_textureRect.top = 0;									//
	_textureRect.bottom = _textureRect.top + _texInfo.Height;	//
	*/

}
void Sprite::Init(std::wstring textureFilename, int x, int y, int width, int height, float frameTime)
{


	// �̹��� ���Ͽ��� �ؽ��� �ε�
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename);

	Frame* frame = new Frame();
	frame->Init(_texture, x, y, width, height, 0.0f, frameTime);
	_frameList.push_back(frame);

	_currentFrame = 0;
	_frameDurateion = 0;


}

void Sprite::Render()
{
	//_frame->Render();
	//_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
	if (_currentFrame < _frameList.size())
	{
		_frameList[_currentFrame]->SetPosition(_x, _y);
		_frameList[_currentFrame]->Render();
	}

}
void Sprite::Release()
{
	//_frame->Relase();
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Relase();
	}
	/*
	if (_textureDX)
	{
	_textureDX->Release();
	_textureDX = NULL;
	}
	*/
	_texture->Release();
}
void Sprite::Reset()
{
	//_device3d = device3d;
	//_spriteDX = spriteDX;

	_texture->Reset();
	/*
	HRESULT hr = D3DXCreateTextureFromFileEx(
	device3d,
	_filePath.c_str(),
	_texInfo.Width,
	_texInfo.Height,
	1,
	0,
	D3DFMT_UNKNOWN,
	D3DPOOL_DEFAULT,
	D3DX_DEFAULT,
	D3DX_DEFAULT,
	D3DCOLOR_ARGB(255, 255, 255, 255),					// color key (����) ����Ҷ� Ư�� ���� ����.
	&_texInfo,
	NULL,												// �ȷ�Ʈ
	&_textureDX);
	*/
	//_frame->Reset();

	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset();
	}
}

void Sprite::Update(float deltaTime)
{
	_frameDurateion += deltaTime;
	if (_frameList[_currentFrame]->GetFrameTime() <= _frameDurateion)
	{
		_frameDurateion = 0.0f;
		_currentFrame = (_currentFrame + 1) % _frameList.size();

	}
}

void Sprite::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}
