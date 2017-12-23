#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
{

}

ComponentSystem::~ComponentSystem()
{

}

ComponentSystem* ComponentSystem::_instance = 0;

ComponentSystem* ComponentSystem::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new ComponentSystem();
	}
	return _instance;
}

void ComponentSystem::AddComponent(std::wstring name, Component* component)
{
	std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);
	if (it == _componentMap.end())
	{
		_componentMap[name] = component;
	}
}

void ComponentSystem::RemoveAllComponents()
{
	for (std::map<std::wstring, Component*>::iterator it = _componentMap.begin();
		it != _componentMap.end(); it++)
	{
		(it->second)->Deinit();
		delete (it->second);
	}
	_componentMap.clear();
}

Component* ComponentSystem::FindComponent(std::wstring name)
{
	std::map<std::wstring, Component*>::iterator it = _componentMap.find(name);
	if (it != _componentMap.end())
	{
		return it->second;
	}
	return NULL;
}