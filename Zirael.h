#pragma once
#include "Enemy.h"
#include "ZiraelSummon.h"

class Zirael : public Enemy
{
public:
	Zirael(const sf::RenderTarget& target,
		textureHolder* enemyTextures, const sf::Font & font,
		std::vector<std::unique_ptr<Enemy>>* Enemies, int& enemiesRemaining,
		sf::SoundBuffer& sound);
	~Zirael();

	void update(sf::RenderTarget& target,
		std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State& state) override;
	void renderEnemy(sf::RenderTarget& target) override;
	void fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets) override;
	void checkWindowCollision(sf::RenderTarget& target) override;
private:
	std::vector<std::unique_ptr<Enemy>>* enemies;
	const sf::RenderTarget* window;
	textureHolder* textures;
	const sf::Font* textFont;
	int& enemiesLeft;

	sf::Sound fireSound;

	sf::Clock moveClock;
	sf::Time moveTime;
	sf::Clock movingRateClock;
	sf::Time movingRate;
	bool toMove;
	bool moveRight;

	sf::Clock fireClock;
	sf::Time fireRate;

	Bullet ziraelBullet;
	sf::Text ziraelText;

	sf::Clock spawnSummonsClock;
	sf::Time spawnSummonsRate;
	int summonCount;
	int summonCountMax;
};

