#pragma once
#include <list>
#include "GlobalTypes.h"

class Component;

class TileCell
{
private:
	Point _position;

public:
	TileCell();
	~TileCell();

	void Init();
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	void SetPosition(Point position);

	// Component List
private:
	std::list<Component*> _componentList;
public:
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	bool CanMove();
};