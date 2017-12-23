#include "TileCell.h"
#include "Sprite.h"
#include "Component.h"

TileCell::TileCell()
{
	_sprite = NULL;
	_position.x = _position.y = 0.0f;
}

TileCell::~TileCell()
{

}

void TileCell::Init(Sprite* sprite)
{
	_sprite = sprite;
}

void TileCell::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void TileCell::SetPosition(Point position)
{
	_position = position;
}

void TileCell::Render()
{
	_sprite->SetPosition(_position.x, _position.y);
	_sprite->Render();

	for (std::list<Component*>::iterator it = _componentList.begin();
		it != _componentList.end(); it++)
	{
		(*it)->SetPosition(_position);
		(*it)->Render();
	}
}

void TileCell::Release()
{
	_sprite->Release();
}

void TileCell::Reset()
{
	_sprite->Reset();
}

void TileCell::AddComponent(Component* component)
{
	_componentList.push_back(component);
}

void TileCell::RemoveComponent(Component* component)
{
	_componentList.remove(component);
}