<<<<<<< HEAD
#include "Texture.h"


Texture::Texture(LPDIRECT3DDEVICE9 device3d, std::wstring filePath)
{
	_device3d = device3d;
=======
#include "GameSystem.h"
#include "Texture.h"


Texture::Texture(std::wstring filePath)
{
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	_fileName = filePath;

	// �̹��� ���Ϸ� ���� �̹��� ������ ���´�.
	HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	if (FAILED(hr))
	{
		return;
	}

<<<<<<< HEAD
	hr = D3DXCreateTextureFromFileEx(_device3d,
=======
	hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance()->GetDeviceDX(),
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
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

<<<<<<< HEAD
void Texture::Reset(LPDIRECT3DDEVICE9 device3d)
{
	_device3d = device3d;

	// Texture �ٽ� �ε�
	HRESULT hr = D3DXCreateTextureFromFileEx(_device3d,
=======
void Texture::Reset()
{
	// Texture �ٽ� �ε�
	HRESULT hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance()->GetDeviceDX(),
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
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