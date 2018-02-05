#include "Map.h"
#include "ComponentSystem.h"
#include "PathfinderMonster.h"


PathfinderMonster::PathfinderMonster(std::wstring name)
	:Monster(name)
{

}
PathfinderMonster::~PathfinderMonster()
{

}

void PathfinderMonster::Init(std::wstring textureFilename, std::wstring scriptFilename)
{
	Monster::Init(textureFilename, scriptFilename);

	// 위치를 고정
	Map* map = (Map*)ComponentSystem::GetInstance()->FindCompoent(L"Map");
	if (NULL != map)
	{
		map->ResetTileComponent(_tilePosition, this);

		_tilePosition.x = map->GetWidth() - 3;
		_tilePosition.y = map->GetHeight() / 2;
		map->setTileComponent(_tilePosition, this);
	}
}
void PathfinderMonster::UpdateAI(float deltaTime)
{

}