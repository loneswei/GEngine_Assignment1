#include "SaveData.h"

SaveData::SaveData() : Gold(5000), EquippedSkinName("Default")
{
}

SaveData::~SaveData()
{
}

SaveData & SaveData::GetInstance()
{
	static SaveData data;
	return data;
}

void SaveData::SetGold(const unsigned int & newGold)
{
	Gold = newGold;
}

unsigned int SaveData::GetGold()
{
	return Gold;
}
