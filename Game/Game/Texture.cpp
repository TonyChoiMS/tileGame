#include "Texture.h"
#include "GameSystem.h"
Texture::Texture(std::wstring filePath)
{
	// �̹��� ���Ϸ� ���� �̹��� ������ ���´�.
	//_device3d = device3d;

	_fileName = filePath;

	HRESULT hr = D3DXGetImageInfoFromFile(filePath.c_str(), &_texInfo);
	if (FAILED(hr))
	{
		return;
	}

	hr = D3DXCreateTextureFromFileEx(
		GameSystem::GetInstance()->GetDeviceDX(),
		filePath.c_str(),
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
	//_device3d = device3d;

	HRESULT hr = D3DXCreateTextureFromFileEx(
		GameSystem::GetInstance()->GetDeviceDX(),
		_fileName.c_str(),
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
}
IDirect3DTexture9* Texture::GetTextureDX()
{
	return _textureDX;
}