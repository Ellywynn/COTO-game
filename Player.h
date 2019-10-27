#pragma once
#include "Bullet.h"
#include "Entity.h"
#include "Missile.h"
#include "State.h"

#include <SFML/Graphics.hpp>
#include <map>

//player upgrades
struct UpgradeSystem
{
	UpgradeSystem()
	{
		reset();

		bulletDamagePrices.emplace(1, 500);
		bulletDamagePrices.emplace(2, 1500);
		bulletDamagePrices.emplace(3, 5000);
		bulletDamagePrices.emplace(4, 7500);
		bulletDamagePrices.emplace(5, 10000);
		bulletDamagePrices.emplace(6, 15000);
		bulletDamagePrices.emplace(7, 25000);
		bulletDamagePrices.emplace(8, 40000);
		bulletDamagePrices.emplace(9, 60000);
		bulletDamagePrices.emplace(10, 100000);

		missileDamagePrices.emplace(1, 1000);
		missileDamagePrices.emplace(2, 3000);
		missileDamagePrices.emplace(3, 10000);
		missileDamagePrices.emplace(4, 15000);
		missileDamagePrices.emplace(5, 20000);
		missileDamagePrices.emplace(6, 30000);
		missileDamagePrices.emplace(7, 50000);
		missileDamagePrices.emplace(8, 80000);
		missileDamagePrices.emplace(9, 120000);
		missileDamagePrices.emplace(10, 200000);

		armorPrices.emplace(1, 250);
		armorPrices.emplace(2, 750);
		armorPrices.emplace(3, 2500);
		armorPrices.emplace(4, 3750);
		armorPrices.emplace(5, 5000);
		armorPrices.emplace(6, 7500);
		armorPrices.emplace(7, 12500);
		armorPrices.emplace(8, 20000);
		armorPrices.emplace(9, 30000);
		armorPrices.emplace(10, 60000);

		bAttackSpeedPrices.emplace(1, 250);
		bAttackSpeedPrices.emplace(2, 750);
		bAttackSpeedPrices.emplace(3, 2500);
		bAttackSpeedPrices.emplace(4, 3750);
		bAttackSpeedPrices.emplace(5, 5000);
		bAttackSpeedPrices.emplace(6, 7500);
		bAttackSpeedPrices.emplace(7, 12500);
		bAttackSpeedPrices.emplace(8, 20000);
		bAttackSpeedPrices.emplace(9, 30000);
		bAttackSpeedPrices.emplace(10, 60000);

		mAttackSpeedPrices.emplace(1, 1000);
		mAttackSpeedPrices.emplace(2, 3000);
		mAttackSpeedPrices.emplace(3, 10000);
		mAttackSpeedPrices.emplace(4, 15000);
		mAttackSpeedPrices.emplace(5, 20000);
		mAttackSpeedPrices.emplace(6, 30000);
		mAttackSpeedPrices.emplace(7, 50000);
		mAttackSpeedPrices.emplace(8, 80000);
		mAttackSpeedPrices.emplace(9, 120000);
		mAttackSpeedPrices.emplace(10, 200000);

		hpPrices.emplace(1, 500);
		hpPrices.emplace(2, 1500);
		hpPrices.emplace(3, 5000);
		hpPrices.emplace(4, 7500);
		hpPrices.emplace(5, 10000);
		hpPrices.emplace(6, 15000);
		hpPrices.emplace(7, 25000);
		hpPrices.emplace(8, 40000);
		hpPrices.emplace(9, 60000);
		hpPrices.emplace(10, 100000);
	}

	void reset()
	{
		bulletDamageLevel = 0, bulletDamageLevelMax = 9;
		missileDamageLevel = 0, missileDamageLevelMax = 9;
		armorLevel = 0, armorLevelMax = 9;
		bAttackSpeedLevel = 0, bAttackSpeedLevelMax = 9;
		mAttackSpeedLevel = 0, mAttackSpeedLevelMax = 9;
		hpLevel = 0, hpLevelMax = 9;
	}

	int bulletDamageLevel, bulletDamageLevelMax;
	int bAttackSpeedLevel, bAttackSpeedLevelMax;
	int missileDamageLevel, missileDamageLevelMax;
	int mAttackSpeedLevel, mAttackSpeedLevelMax;
	int armorLevel, armorLevelMax;
	int hpLevel, hpLevelMax;

	std::map<int, int> bulletDamagePrices;
	std::map<int, int> missileDamagePrices;
	std::map<int, int> armorPrices;
	std::map<int, int> bAttackSpeedPrices;
	std::map<int, int> mAttackSpeedPrices;
	std::map<int, int> hpPrices;
};

class Player : public Entity
{
public:
	Player();
	~Player();

	void update(sf::RenderTarget& target,
		std::vector<std::unique_ptr<Weapon>>& playerBullets,
		bool& shiftMode, const State& state);
	void render(sf::RenderTarget& target);

	void reset();

	const int getHpMax() const;
	int getHp() const;
	const sf::Vector2f& getPosition() const;

	void takeDamage(int damage);
	void setBulletAS(float as);          //bullet attack speed 
	void setMissileAS(float as);         //missile attack speed
	void setHp(int _hp);
	void setHpMax(int _hpMax);
	void setSpawnPosition(sf::RenderTarget& target);
	void setArmor(float _armor);
	float getArmor() const;
public:
	Animation particle;
	UpgradeSystem upgrades;
	float bulletAS;
	float missileAS;
	Bullet playerBullet;
	Missile playerMissile;
private:
	void checkWindowCollision(sf::RenderTarget& target);
	void updateInput(std::vector<std::unique_ptr<Weapon>>& playerBullets, const State& state);
	void updateParticles();
	
	void fireBullet(std::vector<std::unique_ptr<Weapon>>& playerBullets);
	void fireMissile(std::vector<std::unique_ptr<Weapon>>& playerBullets);

	void initPlayer();
private:
	float armor;
	int hp, hpMax;

	textureHolder textures;
	soundHolder sounds;

	sf::Sound shiftSound;
	sf::Sound bulletSound;
	sf::Sound missileSound;

	sf::Clock bulletFireClock;
	sf::Time bulletFireRate;

	sf::Clock missileFireClock;
	sf::Time missileFireRate;

	bool shift;                  //allows player to go through enemy and enemy bullets
	bool moveRight;              //shift right or left
	float shiftingSpeed;         
	sf::Clock shiftingTimer;
	sf::Time shiftingDuration;
};