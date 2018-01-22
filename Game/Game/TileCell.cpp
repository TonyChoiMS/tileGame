#include "TileCell.h"
#include "Sprite.h"
#include "Component.h"

TileCell::TileCell()
{
	_position.x = _position.y = 0.0f;
}

TileCell::~TileCell()
{

}

void TileCell::Init()
{

}

void TileCell::Update(float deltaTime)
{

}

void TileCell::Render()
{
	for (std::list<Component*>::iterator it = _componentList.begin();
		it != _componentList.end(); it++)
	{
		(*it)->SetPosition(_position);
		(*it)->Render();
	}
}

void TileCell::Release()
{

}

void TileCell::Reset()
{

}

void TileCell::SetPosition(Point position)
{
	_position = position;
}

void TileCell::AddComponent(Component* component)
{
	_componentList.push_back(component);
}

void TileCell::RemoveComponent(Component* component)
{
	_componentList.remove(component);
}

std::list<Component*>TileCell::GetCollisionList()
{
	std::list<Component*> collisionList;

	for (std::list<Component*>::iterator it = _componentList.begin();
		it != _componentList.end();
		it++)
	{
		if (false == (*it)->CanMove())
		{
			collisionList.push_back((*it));
		}
	}
	return collisionList;
}

bool TileCell::CanMove()
{
	// 타일셀이 가진 모든 컴포넌트리스트를 검사해서
	// 하나라도 갈 수 없는 타일이 있으면 
	// false 리턴
	for (std::list<Component*>::iterator it = _componentList.begin();
		it != _componentList.end(); 
		it++)
	{
		if (false == (*it)->CanMove())
			return false;
	}
	return true;

}