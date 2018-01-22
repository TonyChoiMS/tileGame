#pragma once

#include <queue>
#include <map>
#include <string>
#include "ComponentMessage.h"
#include "Component.h"

class ComponentSystem
{
private:
	//SingleTon
	static ComponentSystem* _instance;

public:
	static ComponentSystem* GetInstance();

private:
	ComponentSystem() {};
	~ComponentSystem() {};

public:
	void Update(float deltaTime);

private:
	std::map<std::wstring, Component*> _componentMap;

public:
	void AddComponent(std::wstring name, Component* component);
	void RemoveAllComponents();

	Component* FindComponent(std::wstring name);
	
	// Message
private:
	std::queue<sMessageParam> _msgQueue;

public:
	void SendMsg(const sMessageParam& param);
	void ProcessMsgQueue();
};