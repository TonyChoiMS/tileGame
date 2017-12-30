#include "ComponentSystem.h"
#include "Map.h"

#include "NPC.h"

NPC::NPC(std::wstring name) : Character(name)
{

}

NPC::~NPC()
{

}

void NPC::UpdateAI(float deltaTime)
{
	if (false == _isMoving)
	{
		Map* map = (Map*)ComponentSystem::GetInstance()->FindComponent(L"Map");
		if (NULL != map)
		{
			eDirection currentDirection = (eDirection)(rand() % 4);
			MoveStart(currentDirection);
		}
	}
	else
	{
		if (_moveTime <= _movingDuration)
		{
			_movingDuration = 0.0f;
			_isMoving = false;
		}
		else 
		{
			_movingDuration += deltaTime;
		}
	}
}