#pragma once

#include <d3dx9.h>
#include <string>

class Texture
{
private:
	IDirect3DTexture9* _textureDX;
	D3DXIMAGE_INFO _texInfo;

	std::wstring _fileName;

public:
	Texture(std::wstring filePath);
	~Texture();

	void Release();
	void Reset();


	IDirect3DTexture9* GetTextureDX();
};
