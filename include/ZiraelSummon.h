#pragma once
#include "Enemy.h"

class ZiraelSummon : public Enemy
{
public:
	ZiraelSummon(const sf::RenderTarget& target,
		textureHolder* enemyTextures, const sf::Font & font,
		EnemyType type = EnemyType::ZiraelSummon);
	~ZiraelSummon();

	void update(sf::RenderTarget& target,
		std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State& state) override;
	void renderEnemy(sf::RenderTarget& target) override;
	void fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets) override;
};

