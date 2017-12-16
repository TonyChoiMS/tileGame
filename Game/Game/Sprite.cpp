
#include <reader.h>			// json library header file

#include "ResourceManager.h"
#include "Texture.h"
#include "Frame.h"
#include "Sprite.h"

Sprite::Sprite(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_device3d = device3d;
	_spriteDX = spriteDX;
	_texture = NULL;
}

Sprite::~Sprite()
{
	/*if (NULL != _textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}*/
	/*if (NULL != _texture) {
		_texture->Release();
		delete _texture;
		_texture = NULL;
	}*/
	for (int i = 0; i < _frameList.size(); i++)
	{
		delete _frameList[i];
	}
	_frameList.clear();
}

void Sprite::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	_filePath = textureFilename;
	// �̹��� ���Ͽ��� �ؽ��� �ε�
	//_texture = new Texture(_device3d, filePath);
	_texture = ResourceManager::GetInstance()->FindTexture(textureFilename, _device3d);
	// json ��ũ��Ʈ �Ľ�
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

	_currentFrame = 0;
	_frameDuration = 0.0f;
	//std::wstring.c_str() -> std::wstring�� LPCWSTR������ �ٲ���
	//HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	//if (FAILED(hr))
	//{
	//	return;
	//}

	//hr = D3DXCreateTextureFromFileEx(
	//	_device3d,			// Direct3D
	//	filePath.c_str(),			// ���� ���
	//	_texInfo.Width,		// �̹����� ����
	//	_texInfo.Height,		// �̹��� ����
	//	1,					// �̹����� ��������ų� �־��� �� ����Ƽ ����
	//	0,
	//	D3DFMT_UNKNOWN,		// ���� ������ �����츦 ����
	//	D3DPOOL_DEFAULT,	// �ڿ� ������ �⺻���� ������ �ý����� ����
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	D3DCOLOR_ARGB(255, 255, 255, 255),		// Color Key - �ش� ���� ȭ�鿡 ����� �ȵ�(�ٸ� ������ ��ü �� ��)
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
	//_frame->Release();
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Release();
	}
	/*if (_textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}*/
	_texture->Release();
}

void Sprite::Reset(LPDIRECT3DDEVICE9 device3d, ID3DXSprite* spriteDX)
{
	_device3d = device3d;
	_spriteDX = spriteDX;

	_texture->Reset(_device3d);
	//Texture �ε�
	//HRESULT hr = D3DXCreateTextureFromFileEx(
	//	device3d,			// Direct3D
	//	_filePath.c_str(),			// ���� ���
	//	_texInfo.Width,		// �̹����� ����
	//	_texInfo.Height,		// �̹��� ����
	//	1,					// �̹����� ��������ų� �־��� �� ����Ƽ ����
	//	0,
	//	D3DFMT_UNKNOWN,		// ���� ������ �����츦 ����
	//	D3DPOOL_DEFAULT,	// �ڿ� ������ �⺻���� ������ �ý����� ����
	//	D3DX_DEFAULT,
	//	D3DX_DEFAULT,
	//	D3DCOLOR_ARGB(255, 255, 255, 255),		// Color Key - �ش� ���� ȭ�鿡 ����� �ȵ�(�ٸ� ������ ��ü �� ��)
	//	&_texInfo,
	//	NULL,					// Pallete
	//	&_textureDX);

	//_frame->Reset();
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset(_spriteDX);
	}
}

void Sprite::SetPosition(float x, float y) {
	_x = x;
	_y = y;
}