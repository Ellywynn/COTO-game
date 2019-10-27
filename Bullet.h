#pragma once
#include "Weapon.h"
#include "ResourceHolder.h"

enum class BulletType
{
	PLAYER = 0,
	EAGLE,
	RAPTOR,
	ZIRAEL,
	VATTGERN
};

class Bullet : public Weapon
{
public:
	Bullet(WeaponType b = WeaponType::PlayerBullet);
	Bullet(const sf::Texture& bulletTexture, const Entity* parent,
		BulletType type, const float& dmgMul = 1.f, WeaponType b = WeaponType::PlayerBullet);
	~Bullet();
	void update() override;
	void render(sf::RenderWindow& window) override;

	void setBulletPosition(const Entity* parent);
private:
	BulletType bulletType;
};

