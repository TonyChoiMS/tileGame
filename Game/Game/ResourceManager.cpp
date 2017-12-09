#include <fstream>			// STL
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

Texture* ResourceManager::FindTexture(std::wstring fileName, LPDIRECT3DDEVICE9 device3d)
{
	WCHAR filePath[256];
	wsprintf(filePath, L"../Resources/images/%s", fileName.c_str());
	// fileName 으로 등록된 텍스쳐가 있는지 찾고
	std::map<std::wstring, Texture*>::iterator it = _textureMap.find(filePath);

	// 있으면 있는걸 리턴
	if (it != _textureMap.end())
	{
		return it->second;
	}

	// 없으면, 새로 생성하고
	Texture* texture = new Texture(device3d, filePath);

	// 생성된걸, fileName 이름으로 등록
	_textureMap[filePath] = texture;

	// 생성된걸 리턴
	return texture;
}

std::vector<std::string> ResourceManager::FindScript(std::wstring fileName)
{
	WCHAR filePath[256];
	wsprintf(filePath, L"../Resources/Scripts/%s", fileName.c_str());
	std::map<std::wstring, std::vector<std::string>>::iterator it = _scriptMap.find(filePath);
	if (it != _scriptMap.end())
	{
		return it->second;
	}

	std::vector<std::string> recordList;
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