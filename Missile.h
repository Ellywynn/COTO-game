#pragma once
#include "Weapon.h"
#include "Animation.h"
#include "ResourceHolder.h"
#include <iostream>

class Missile : public Weapon
{
public:
	Missile(WeaponType m = WeaponType::PlayerMissile);
	Missile(const sf::Texture& texture, Entity* parent, WeaponType m = WeaponType::PlayerMissile);
	~Missile();

	void update() override;
	void render(sf::RenderWindow& window) override;

	void setMissilePosition(const Entity* parent);
};

