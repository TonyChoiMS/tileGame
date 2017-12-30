#pragma once

#include <map>
#include <string>
#include "Component.h"

class Component;

class ComponentSystem
{
private:
	//Single Ton
	static ComponentSystem* _instance;

public:
	static ComponentSystem* GetInstance();

private:
	ComponentSystem() {};
	~ComponentSystem() {};

private:
	std::map<std::wstring, Component*> _componentMap;

public:
	void AddComponent(std::wstring name, Component* component);
	void RemoveAllComponents();
	Component* FindComponent(std::wstring name);
};