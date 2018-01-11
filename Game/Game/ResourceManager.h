#pragma once

#include <d3dx9.h>
#include <string>
#include <map>
#include <vector>

class Texture;

class ResourceManager
{
	// Singleton
private:
	static ResourceManager* _instance;

public:
	static ResourceManager* GetInstance();

private:
	ResourceManager();
	~ResourceManager();

	// Texture
private:
	std::map<std::wstring, Texture*> _textureMap;

public:
<<<<<<< HEAD
	Texture* FindTexture(std::wstring fileName, LPDIRECT3DDEVICE9 device3d);
=======
	Texture* FindTexture(std::wstring fileName);
	void RemoveAllTexture();
>>>>>>> 86cde9369e6be281b5907e7a6397782e9162fe30

	// Script
private:
	std::map<std::wstring, std::vector<std::string>> _scriptMap;

public:
	std::vector<std::string> FindScript(std::wstring fileName);
};
