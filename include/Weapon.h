#pragma once
#include "Entity.h"
#include <SFML/Audio.hpp>

enum class WeaponType
{
	PlayerBullet = 0,
	PlayerMissile,

	EnemyBullet,
	ZiraelBullet,
	VattgernBullet
};

class Weapon : public Entity
{
public:
	Weapon(WeaponType type);
	virtual ~Weapon();

	virtual void update() = 0;
	virtual void render(sf::RenderWindow& window) = 0;

	WeaponType weaponType;
	sf::RectangleShape collider;
	int damage;

	int getWeaponDamage();
	void setWeaponDamage(int _damage);
};

