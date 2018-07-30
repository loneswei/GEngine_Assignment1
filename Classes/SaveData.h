#ifndef  _SAVE_DATA_H_
#define  _SAVE_DATA_H_

#include <string>
#include <map>

class SaveData
{
public:
	SaveData();
	~SaveData();

	static SaveData& GetInstance();

	void SetGold(const unsigned int &newGold);
	unsigned int GetGold();

	std::string EquippedSkinName;
	std::map<std::string, bool> BoughtSkins;
	
private:
	unsigned int Gold;
};
#endif // _SAVE_DATA_H_

