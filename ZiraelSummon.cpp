#include "ZiraelSummon.h"



ZiraelSummon::ZiraelSummon(const sf::RenderTarget& target,
	textureHolder* enemyTextures, const sf::Font & font, EnemyType type)
{
	//randomize position
	float spawnx = std::rand() % static_cast<int>(target.getSize().x - 50.f) + 50;
	float spawny = std::rand() % 1250 + 450;

	x = spawnx; y = -spawny;

	speed = 13.f;
	enemyTexture = &enemyTextures->get(EnemyZirael);
	enemyType = type;

	killReward = 120;
	hpMax = 8;
	hp = hpMax;
	sprite.setTexture(*enemyTexture);
	sprite.setOrigin((*enemyTexture).getSize().x / 2.f, (*enemyTexture).getSize().y / 2.f);
	sprite.setRotation(180.f);
	sprite.setScale(0.5f, 0.5f);
	sprite.setPosition(x, y);

	sf::Vector2f enemyPos(sprite.getPosition());
	enemyStats = enemyBar(font, enemyPos, enemyTextures->get(Hp));
}


ZiraelSummon::~ZiraelSummon()
{
}

void ZiraelSummon::update(sf::RenderTarget & target, std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State & state)
{
	if (state == State::GAME
		|| state == State::SHOP
		|| state == State::GAMEOVER)
	{
		sprite.move(0.f, speed);
		enemyStats.hp.setString(std::to_string(hp) + "/" + std::to_string(hpMax));
		enemyStats.updateText(sprite.getPosition(), sprite.getGlobalBounds().width / 2.f);
	}
}

void ZiraelSummon::renderEnemy(sf::RenderTarget & target)
{
	target.draw(sprite);
	target.draw(enemyStats.hearts);
	target.draw(enemyStats.hp);
}

void ZiraelSummon::fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets)
{
	//can't fire
}
