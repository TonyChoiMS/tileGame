#pragma once
#include <list>
#include "GlobalTypes.h"

class Sprite;
class Component;

class TileCell
{
private:
	Sprite* _sprite;
	Point _position;

public:
	TileCell();
	~TileCell();

	void Init(Sprite* sprite);
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
};