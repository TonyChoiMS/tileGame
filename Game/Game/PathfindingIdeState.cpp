#include "Map.h"
#include "ComponentSystem.h"
#include "InputSystem.h"
#include "Character.h"
#include "PathfindingIdeState.h"

PathfindingIdeState::PathfindingIdeState(Character* character)
	:State(character)
{

}
PathfindingIdeState::~PathfindingIdeState()
{

}

void PathfindingIdeState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	// ���콺 Ŭ�� -> Ŭ���� ��ġ�� Ÿ�ϼ��� Ÿ�� Ÿ�ϼ��� ����

	if (InputSystem::GetInstance()->IsMouseDown())
	{
		int mouseX = InputSystem::GetInstance()->GetMouseX();
		int mouseY = InputSystem::GetInstance()->GetMouseY();

		Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
		TileCell* tileCell = map->FindTileCellByMousePosition(mouseX, mouseY);

		if (NULL != tileCell)
		{
			_character->SetTargetTileCell(tileCell);
		}
	}

	TileCell* targetTileCell = _character->GetTargetTileCell();
	if (NULL != targetTileCell)
	{
		_nextState = eStateType::ST_PATHFINDING;
	}
}