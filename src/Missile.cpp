#include "../include/Missile.h"



Missile::Missile(WeaponType m)
	:Weapon(m)
{
}

Missile::Missile(const sf::Texture& texture, Entity* parent, WeaponType m)
	:Weapon(m)
{
	x = parent->sprite.getPosition().x;
	y = parent->sprite.getPosition().y - 50.f;

	speed = -11.f;
	damage = 10;

	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
	sprite.setRotation(90.f);
	sprite.setScale(0.5f, 0.5f);
	sprite.setPosition(x, y);

	collider.setSize(sf::Vector2f(280.f, 280.f));
	collider.setFillColor(sf::Color(70,70,70,100));
	collider.setOrigin(collider.getSize().x / 2.f, collider.getSize().y / 2.f);
	collider.setPosition(x, y);
}

Missile::~Missile()
{
}

void Missile::update()
{
	collider.move(0.f, speed);
	sprite.move(0.f, speed);
}

void Missile::render(sf::RenderWindow & window)
{
	//window.draw(collider);
	window.draw(sprite);
}

void Missile::setMissilePosition(const Entity * parent)
{
	x = parent->sprite.getPosition().x;
	y = parent->sprite.getPosition().y - 50.f;
	sprite.setPosition(x, y);
	collider.setPosition(x, y);
}
