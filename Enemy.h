#pragma once
#include "Bullet.h"
#include "Entity.h"
#include "ResourceHolder.h"
#include "State.h"

#include <iostream>

enum class EnemyType
{
	RAPTOR = 0,
	EAGLE,
	COUNT,
	ZiraelSummon,
	Zirael,    //Zirael    boss
	Vattgern   //Vattgern  boss
};

//enemy bar is GUI that allows player to see current enemy hp
struct enemyBar
{
	enemyBar() {};
	enemyBar(const sf::Font& font, sf::Vector2f& enemyPos,
		const sf::Texture& heart)
	{
		hp.setFont(font);
		hp.setCharacterSize(16);
		hp.setStyle(sf::Text::Bold);

		hearts.setTexture(heart);
		hearts.setOrigin(heart.getSize().x, heart.getSize().y);
		hearts.setScale(0.5f, 0.5f);
		hp.setString("Hp: " + std::to_string(100) + "/100");
	}

	sf::Sprite hearts;
	sf::Text hp;

	void updateText(sf::Vector2f enemyPos, float add, EnemyType eType = EnemyType::EAGLE)
	{
		if (eType == EnemyType::EAGLE)
		{
			hearts.setPosition(enemyPos.x - add + hearts.getGlobalBounds().width,
				enemyPos.y + hearts.getGlobalBounds().height / 2.f + 50.f);
			hp.setPosition(hearts.getPosition().x + hearts.getGlobalBounds().width / 2.f - 15.f,
				hearts.getPosition().y - hearts.getGlobalBounds().height / 2.f - hp.getGlobalBounds().height);
		}
		else if (eType == EnemyType::Zirael)
		{
			hp.setPosition(enemyPos.x - hp.getGlobalBounds().width / 2.f,
				enemyPos.y - hp.getGlobalBounds().height / 2.f + add - 15.f);
			hearts.setPosition(hp.getPosition().x,
				hp.getPosition().y + hearts.getGlobalBounds().height - 5.f);
		}
		else if (eType == EnemyType::Vattgern)
		{
			hp.setPosition(enemyPos.x - hp.getGlobalBounds().width / 2.f,
				enemyPos.y - hp.getGlobalBounds().height / 2.f + add + 50.f);
			hearts.setPosition(hp.getPosition().x,
				hp.getPosition().y + hearts.getGlobalBounds().height - 5.f);
		}
	}
};

class Enemy : public Entity
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void update(sf::RenderTarget& target,
		std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State& state) = 0;
	virtual void renderEnemy(sf::RenderTarget& target) = 0;
	virtual void checkWindowCollision(sf::RenderTarget& target);
	virtual void fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets) = 0;

	void takeDamage(int damage);

	const int getHp() const;
	const int getHpMax() const;

	EnemyType enemyType;
	int hp, hpMax;
	sf::Texture* enemyTexture;
	enemyBar enemyStats;
	int killReward;
};

