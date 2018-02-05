#pragma once
#include <string>
#include <map>
#include <queue>
#include "ComponentMessage.h"

#include"Component.h"

class ComponentSystem
{
private:
	static ComponentSystem* _instance;
public:
	static ComponentSystem* GetInstance();
private:
	ComponentSystem() {}
	~ComponentSystem() {}
private:
	std::map<std::wstring, Component*> _componentMap;
private:
	std::queue<sMessageParam> _msgQueue;
public:
	void Update(float deltatime);
public:
	void AddComponent(std::wstring name, Component* component);
	void RemoveAllComponents();
	Component* FindCompoent(std::wstring name);
	void SendMsg(const sMessageParam& param);
	void ProcessMsgQueue();

};