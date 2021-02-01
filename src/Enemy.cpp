#include "../include/Enemy.h"
#include <iostream>


Enemy::Enemy()
	:enemyTexture(nullptr)
{
}

Enemy::~Enemy()
{
}

const int Enemy::getHp() const
{
	return hp;
}

const int Enemy::getHpMax() const
{
	return hpMax;
}

void Enemy::takeDamage(int damage)
{
	hp -= damage;
	if (hp < 0)
		hp = 0;
}

void Enemy::checkWindowCollision(sf::RenderTarget& target)
{
	y = sprite.getPosition().y;

	//left side
	if (sprite.getGlobalBounds().left <= 0)
		sprite.setPosition(0.f + sprite.getGlobalBounds().width / 2.f, y);
	//right side
	else if (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width >= target.getSize().x)
		sprite.setPosition(target.getSize().x - sprite.getGlobalBounds().width / 2.f, y);
}
