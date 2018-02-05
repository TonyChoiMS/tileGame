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

	//Texture
private:
	std::map<std::wstring, Texture*> _textureMap;
public:
	Texture* FindTexture(std::wstring fileName);
private:
	std::map<std::wstring, std::vector<std::string>> _scriptMap;
	//Script
public:
	std::vector<std::string>FindScript(std::wstring fileName);
public:
	void RemoveAllTexture();

};