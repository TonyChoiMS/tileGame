#include "IdleState.h"
#include "Character.h"

IdleState::IdleState(Character* character)
	:State(character)
{
	//_character = character;
}
IdleState::~IdleState()
{

}
void IdleState::Start()
{
	State::Start();
}
void IdleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
	_character->UpdateAI(deltaTime);
}
