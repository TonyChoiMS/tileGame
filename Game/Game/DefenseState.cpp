#include "DefenseState.h"
#include "Character.h"

DefenseState::DefenseState(Character* character) : State(character)
{
}

DefenseState::~DefenseState()
{
}

void DefenseState::Start()
{
	// Initialize State member variable
	State::Start();

	int damagePoint = _character->GetDamagePoint();
	_character->DecreaseHP(damagePoint);
	if (false == _character->IsLive())
	{
		// 죽었을 때 캐릭터는 움직일 수 없다.
		_character->SetCanMove(true);
		_character->MoveStop();
		ChangeState(eStateType::ST_DIE);
	}
	else
	{
		ChangeState(eStateType::ST_IDLE);
	}
}

void DefenseState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}