#include "DefenseState.h"
#include "Character.h"
DefenseState::DefenseState(Character* character)
	:State(character)
{

}
DefenseState::~DefenseState()
{

}
void DefenseState::Start()
{
	State::Start();

	//int attackPoint = param.attackPoint;
	int damagePoint = _character->GetDamagePoint();
	//_hp -= attackPoint;
	//if (0 >_hp)
	_character->DecreaseHP(damagePoint);
	if (false == _character->IsLive())
	{
		// �׾��� �� ĳ���ʹ� ��
		//_isLive = false;
		//SetCanMove(true);
		_character->SetCanMove(true); // 
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