#include "../include/Bullet.h"
#include <iostream>

Bullet::Bullet(WeaponType b)
	:Weapon(b)
{

}

Bullet::Bullet(const sf::Texture& bulletTexture, const Entity* parent,
	BulletType type, const float& dmgMul, WeaponType b)
	: bulletType(type), Weapon(b)
{
	//init bullet
	sprite.setTexture(bulletTexture);
	sprite.setOrigin(bulletTexture.getSize().x / 2.f, bulletTexture.getSize().y / 2.f);
	sprite.setRotation(90.f);
	sprite.setPosition(x, y);

	//init bullet stats depending on bullet type
	switch (bulletType)
	{
	case BulletType::PLAYER:
		speed = -15.f;
		x = parent->sprite.getPosition().x;
		y = parent->sprite.getPosition().y - 50.f;
		sprite.setScale(0.15f, 0.25f);
		damage = 1 * dmgMul;
		break;
	case BulletType::EAGLE:
		speed = 7.f;
		damage = 2 * dmgMul;
		sprite.setScale(0.3f, 0.3f);
		break;
	case BulletType::RAPTOR:
		speed = 7.f;
		damage = 3 * dmgMul;
		sprite.setScale(0.3f, 0.3f);
		break;
	case BulletType::ZIRAEL:
		speed = 8.f;
		damage = 40;
		sprite.setScale(0.3f, 1.f);
		break;
	case BulletType::VATTGERN:
		speed = 10.f;
		damage = 30;
		sprite.setScale(0.25f, 0.8f);
		break;
	}
	setBulletPosition(parent);

	collider.setSize(sf::Vector2f(sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height));
	collider.setFillColor(sf::Color::Red);
	collider.setOrigin(collider.getGlobalBounds().width / 2.f,
		collider.getGlobalBounds().height / 2.f);
	collider.setPosition(x, y);
}

Bullet::~Bullet()
{
}

void Bullet::update()
{
	collider.move(0.f, speed);
	sprite.move(0.f, speed);
}

void Bullet::render(sf::RenderWindow & window)
{
	//window.draw(collider);
	window.draw(sprite);
}

void Bullet::setBulletPosition(const Entity * parent)
{
	//update bullet position
	switch (bulletType)
	{
	case BulletType::PLAYER:
		x = parent->sprite.getPosition().x;
		y = parent->sprite.getPosition().y - 50.f;
		sprite.setPosition(x, y);
		break;
	case BulletType::EAGLE:
		x = parent->sprite.getPosition().x;
		y = parent->sprite.getPosition().y + 50.f;
		sprite.setPosition(x, y);
		break;
	case BulletType::RAPTOR:
		x = parent->sprite.getPosition().x;
		y = parent->sprite.getPosition().y + 50.f;
		sprite.setPosition(x, y);
		break;
	case BulletType::ZIRAEL:
		x = parent->sprite.getPosition().x;
		y = parent->sprite.getPosition().y + 70.f;
		sprite.setPosition(x, y);
		break;
	case BulletType::VATTGERN:
		x = parent->sprite.getPosition().x;
		y = parent->sprite.getPosition().y + 79.f;
		sprite.setPosition(x, y);
		break;
	}
	collider.setPosition(x, y);
}