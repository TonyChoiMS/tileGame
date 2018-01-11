#pragma once

#include <d3dx9.h>
#include <string>

class Texture
{
private:
<<<<<<< HEAD
	LPDIRECT3DDEVICE9 _device3d;
=======
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30
	IDirect3DTexture9* _textureDX;
	D3DXIMAGE_INFO _texInfo;

	std::wstring _fileName;

public:
<<<<<<< HEAD
	Texture(LPDIRECT3DDEVICE9 device3d, std::wstring filePath);
	~Texture();

	void Release();
	void Reset(LPDIRECT3DDEVICE9 device3d);
=======
	Texture(std::wstring filePath);
	~Texture();

	void Release();
	void Reset();
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30

	IDirect3DTexture9* GetTextureDX();
};
