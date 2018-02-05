#include "Character.h"
#include "DieState.h"
#include "Sprite.h"
DieState::DieState(Character* character)
	:State(character)
{

}
DieState::~DieState()
{

}
void DieState::Start()
{
	State::Start();
	//_character->IsDie();

	//_isLive = false;
	//_hp = 0;
}
void DieState::Update(float deltaTime)
{
	State::Update(deltaTime);
	if (eStateType::ST_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void DieState::CreateSprite(std::wstring textureFilename, std::wstring scriptFilename)
{
	_spriteList.clear();
	{
		// left
		std::wstring script = scriptFilename;
		script = script.append(L"_left.json");
		Sprite *sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
	{
		// right
		std::wstring script = scriptFilename;
		script = script.append(L"_right.json");
		Sprite *sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
	{
		// up
		std::wstring script = scriptFilename;
		script = script.append(L"_up.json");
		Sprite *sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
	{
		// down
		std::wstring script = scriptFilename;
		script = script.append(L"_down.json");
		Sprite *sprite = new Sprite();
		sprite->Init(textureFilename, script, 1.5f);
		_spriteList.push_back(sprite);
	}
}