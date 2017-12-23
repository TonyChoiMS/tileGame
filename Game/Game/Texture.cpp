#include "GameSystem.h"
#include "Texture.h"


Texture::Texture(std::wstring filePath)
{
	_fileName = filePath;

	// �̹��� ���Ϸ� ���� �̹��� ������ ���´�.
	HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	if (FAILED(hr))
	{
		return;
	}

	hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance()->GetDeviceDX(),
		filePath.c_str(),
		_texInfo.Width, _texInfo.Height,
		1, 0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo,
		NULL,
		&_textureDX);
	if (FAILED(hr))
	{
		return;
	}
}

Texture::~Texture()
{
	Release();
}

void Texture::Release()
{
	if (_textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}
}

void Texture::Reset()
{
	// Texture �ٽ� �ε�
	HRESULT hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance()->GetDeviceDX(),
		_fileName.c_str(),
		_texInfo.Width, _texInfo.Height,
		1, 0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo,
		NULL,
		&_textureDX);
}

IDirect3DTexture9* Texture::GetTextureDX()
{
	return _textureDX;
}
