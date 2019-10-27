#pragma once
#include <iostream>
#include <map>

class Level
{
public:
	Level();
	~Level();

	
	void nextWave();
	void setCurrentWave(int wave);

	int getEnemyLevelCount(int wave) const;
	int getCurrentWave() const;
	float getDamageMul(int wave) const;
	float getHpMul(int wave) const;
	float getRewardMul(int wave) const;
	const bool isBossWave() const;
	const bool giveLife() const;
private:
	std::map<int, int> enemiesPerWave;      //enemy count for each wave
	std::map<int, float> enemyDamageMul;    //enemy damage multiplyer
	std::map<int, float> enemyHpMul;        //enemy hp multiplyer
	std::map<int, float> enemyRewardMul;    //enemy reward multiplyer

	int currentWave;
};

