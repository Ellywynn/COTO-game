#include "../include/Level.h"



Level::Level()
{
	currentWave = 1;

	enemiesPerWave[1] = 40;   
	enemiesPerWave[2] = 50;   
	enemiesPerWave[3] = 65;
	enemiesPerWave[4] = 70;
	enemiesPerWave[5] = 40;
	enemiesPerWave[6] = 50;
	enemiesPerWave[7] = 60;
	enemiesPerWave[8] = 70;
	enemiesPerWave[9] = 80; 
	enemiesPerWave[10] = 1;      //BOSS ZIRAEL WAVE
	enemiesPerWave[11] = 100;
	enemiesPerWave[12] = 90;
	enemiesPerWave[13] = 80;
	enemiesPerWave[14] = 70;
	enemiesPerWave[15] = 60;
	enemiesPerWave[16] = 70;
	enemiesPerWave[17] = 80;
	enemiesPerWave[18] = 90;
	enemiesPerWave[19] = 100; 
	enemiesPerWave[20] = 1;      //BOSS VATTGERN WAVE

	enemyDamageMul[1] = 1.0f;
	enemyDamageMul[2] = 1.2f;
	enemyDamageMul[3] = 1.25f;
	enemyDamageMul[4] = 1.3f;
	enemyDamageMul[5] = 1.3f;
	enemyDamageMul[6] = 1.4f;
	enemyDamageMul[7] = 1.45f;
	enemyDamageMul[8] = 1.45f;
	enemyDamageMul[9] = 1.5f;
	//enemyDamageMul[10] = 1.0f;
	enemyDamageMul[11] = 1.6f;
	enemyDamageMul[12] = 1.85f;
	enemyDamageMul[13] = 2.0f;
	enemyDamageMul[14] = 2.1f;
	enemyDamageMul[15] = 2.25f;
	enemyDamageMul[16] = 2.3f;
	enemyDamageMul[17] = 2.35f;
	enemyDamageMul[18] = 2.4f;
	enemyDamageMul[19] = 2.5f;
	//enemyDamageMul[20] = 1.0f;

	enemyHpMul[1] = 1.0f;
	enemyHpMul[2] = 1.5f;
	enemyHpMul[3] = 1.75f;
	enemyHpMul[4] = 2.0f;
	enemyHpMul[5] = 3.0f;
	enemyHpMul[6] = 3.5f;
	enemyHpMul[7] = 3.75f;
	enemyHpMul[8] = 4.0f;
	enemyHpMul[9] = 4.5f;
	//enemyHpMul[10] = 1.f;
	enemyHpMul[11] = 5.0f;
	enemyHpMul[12] = 5.5f;
	enemyHpMul[13] = 6.3f;
	enemyHpMul[14] = 6.5f;
	enemyHpMul[15] = 7.0f;
	enemyHpMul[16] = 7.3f;
	enemyHpMul[17] = 7.6f;
	enemyHpMul[18] = 7.8f;
	enemyHpMul[19] = 8.0f;
	//enemyHpMul[20] = 1.f;

	enemyRewardMul[1] = 1.0f;
	enemyRewardMul[2] = 1.2f;
	enemyRewardMul[3] = 1.4f;
	enemyRewardMul[4] = 1.6;
	enemyRewardMul[5] = 1.8f;
	enemyRewardMul[6] = 2.0f;
	enemyRewardMul[7] = 2.3f;
	enemyRewardMul[8] = 2.6f;
	enemyRewardMul[9] = 2.9f;
	//enemyRewardMul[10] = 1.f;
	enemyRewardMul[11] = 3.2f;
	enemyRewardMul[12] = 3.5f;
	enemyRewardMul[13] = 4.0f;
	enemyRewardMul[14] = 4.3f;
	enemyRewardMul[15] = 5.0f;
	enemyRewardMul[16] = 5.5f;
	enemyRewardMul[17] = 6.2f;
	enemyRewardMul[18] = 6.8f;
	enemyRewardMul[19] = 7.4f;
	//enemyRewardMul[20] = 1.f;
}


Level::~Level()
{
}

int Level::getEnemyLevelCount(int wave) const
{
	return enemiesPerWave.at(wave);
}

void Level::nextWave()
{
	currentWave++;
}

void Level::setCurrentWave(int wave)
{
	currentWave = wave;
}

int Level::getCurrentWave() const
{
	return currentWave;
}

float Level::getDamageMul(int wave) const
{
	return enemyDamageMul.at(wave);
}

float Level::getHpMul(int wave) const
{
	return enemyHpMul.at(wave);
}

float Level::getRewardMul(int wave) const
{
	return enemyRewardMul.at(wave);
}

const bool Level::isBossWave() const
{
	if (currentWave == 10)
		return true;

	return false;
}

const bool Level::giveLife() const
{
	//give player the life every 5 waves
	if (currentWave == 5 || currentWave == 10
		|| currentWave == 15 || currentWave == 20
		|| currentWave == 25)
		return true;

	return false;
}
