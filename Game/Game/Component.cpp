#include "Component.h"
#include "ComponentSystem.h"

Component::Component(std::wstring name)
{
	_name = name;
	_tilePosition.x = _tilePosition.y = 0;

	ComponentSystem::GetInstance()->AddComponent(_name, this);
}

Component::~Component()
{

}