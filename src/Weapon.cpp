#include "../include/Weapon.h"



Weapon::Weapon(WeaponType type)
{
	weaponType = type;
}


Weapon::~Weapon()
{
}

int Weapon::getWeaponDamage()
{
	return damage;
}

void Weapon::setWeaponDamage(int _damage)
{
	damage = _damage;
}
