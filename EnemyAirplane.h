#pragma once
#include "Enemy.h"


class EnemyAirplane : public Enemy
{
public:
	EnemyAirplane();
	EnemyAirplane(const sf::RenderTarget& target, textureHolder* enemyTextures,
		const sf::Font& font, EnemyType type,
		soundHolder* sounds,
		const float& dmgMul, const float& hpMul, const float& rewardMul);
	~EnemyAirplane();

	void update(sf::RenderTarget& target, std::vector<std::unique_ptr<Weapon>>* enemyBullets,
		const State& state) override;
	void renderEnemy(sf::RenderTarget& target) override;

	void fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets) override;
private:
	Bullet enemyBullet;
	Animation particle;

	sf::Sound fireSound;

	//enemy fire clock and fire rate
	sf::Clock fireClock;
	sf::Time fireRate;
	//how long enemy moves
	sf::Clock movingClock;
	sf::Time movingTime;
	//time space between movings
	sf::Clock movingRateClock;
	sf::Time movingRate;

	//if true, move
	bool toMove;
	bool moveRight;
};

