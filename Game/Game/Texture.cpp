#include "Texture.h"
#include "GameSystem.h"
Texture::Texture(std::wstring filePath)
{
	// 이미지 파일로 부터 이미지 정보를 얻어온다.
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
		D3DCOLOR_ARGB(255, 255, 255, 255),					// color key (투명값) 출력할때 특정 색을 뺀다.
		&_texInfo,
		NULL,												// 팔레트
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
		D3DCOLOR_ARGB(255, 255, 255, 255),					// color key (투명값) 출력할때 특정 색을 뺀다.
		&_texInfo,
		NULL,												// 팔레트
		&_textureDX);
}
IDirect3DTexture9* Texture::GetTextureDX()
{
	return _textureDX;
}