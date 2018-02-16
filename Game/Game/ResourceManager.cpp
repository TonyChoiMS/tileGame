#include <fstream>		// 파일 입출력 파일 라이브러리 (std)

#include "Texture.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::_instance = 0;

ResourceManager* ResourceManager::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new ResourceManager();
	}
	return _instance;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

Texture* ResourceManager::FindTexture(std::wstring fileName)
{
	WCHAR filePath[256];
	wsprintf(filePath, L"../Resources/images/%s", fileName.c_str());
	// fileName 으로 등록된 텍스쳐가 있는지 찾고 
	// std::(이름)<std ~> 형태 
	std::map<std::wstring, Texture*>::iterator it = _textureMap.find(filePath);

	// 있으면 있는걸 돌려주고 
	if (it != _textureMap.end())
	{
		return it->second;
	}

	// 없으면 새로 생성 하고 
	Texture* texture = new Texture(filePath);
	// 생성된거, fileName 으로 등록
	_textureMap[filePath] = texture;
	// 생성된걸 리턴
	return texture;
}

std::vector<std::string>ResourceManager::FindScript(std::wstring fileName)
{
	WCHAR filePath[256];
	wsprintf(filePath, L"../Resources/Scripts/%s", fileName.c_str());

	std::map<std::wstring, std::vector<std::string>>::iterator it = _scriptMap.find(filePath);
	if (it != _scriptMap.end())
	{
		return it->second;
	}
	std::vector<std::string>recordList;

	std::ifstream infile(filePath);
	if (infile.is_open())
	{
		while (!infile.eof())
		{
			char record[1000];
			infile.getline(record, 1000);
			recordList.push_back(record);
		}
		_scriptMap[filePath] = recordList;
	}

	return recordList;
}

void ResourceManager::RemoveAllTexture()
{
	for (std::map<std::wstring, Texture*>::iterator it = _textureMap.begin();
		it != _textureMap.end(); it++)
	{
		Texture* tex = it->second;
		tex->Release();
		delete tex;
	}
	_textureMap.clear();
}