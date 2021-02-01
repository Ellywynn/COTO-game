#pragma once
#include "Enemy.h"

class Vattgern : public Enemy
{
public:
	Vattgern(const sf::RenderTarget& target,
		textureHolder* enemyTextures, const sf::Font & font,
		soundHolder* sounds);
	~Vattgern();

	void update(sf::RenderTarget& target,
		std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State& state) override;
	void renderEnemy(sf::RenderTarget& target) override;

	void checkWindowCollision(sf::RenderTarget& target) override;
	void fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets) override;
private:
	textureHolder* textures;
	const sf::Font* textFont;

	sf::Sound fireSound;

	sf::Clock moveClock;
	sf::Time moveTime;
	sf::Clock movingRateClock;
	sf::Time movingRate;
	bool toMove;
	bool moveRight;

	sf::Clock fireClock;
	sf::Time fireRate;

	Bullet vattgernBullet;
	sf::Text vattgernText;

	bool shift;
	float shiftingSpeed;
	sf::Clock shiftingTimer;
	sf::Time shiftingDuration;
	sf::Time shiftingRate;
	sf::Sound shiftSound;
};