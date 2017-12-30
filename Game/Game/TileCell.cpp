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

void TileCell::SetPosition(Point position)
{
	_position = position;
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

void TileCell::AddComponent(Component* component)
{
	_componentList.push_back(component);
}

void TileCell::RemoveComponent(Component* component)
{
	_componentList.remove(component);
}

bool TileCell::CanMove()
{
	// Ÿ�ϼ��� ���� ��� ������Ʈ����Ʈ�� �˻��ؼ�
	// �ϳ��� �� �� ���� Ÿ���� ������ 
	// false ����
	for (std::list<Component*>::iterator it = _componentList.begin();
		it != _componentList.end(); 
		it++)
	{
		if (false == (*it)->CanMove())
			return false;
	}
	return true;

}