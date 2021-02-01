#include "../include/Zirael.h"



Zirael::Zirael(const sf::RenderTarget& target,
	textureHolder* enemyTextures, const sf::Font & font,
	std::vector<std::unique_ptr<Enemy>>* Enemies, int& enemiesRemaining,
	sf::SoundBuffer& sound)
	:enemies(Enemies), window(&target),
	textures(enemyTextures), textFont(&font), enemiesLeft(enemiesRemaining)
{
	speed = 12.f;
	hpMax = 1000;
	hp = hpMax;
	killReward = 10000;
	enemyType = EnemyType::Zirael;

	ziraelBullet = Bullet(enemyTextures->get(ZiraelBullet), this,
		BulletType::ZIRAEL, 1.f, WeaponType::ZiraelBullet);

	fireSound.setBuffer(sound);
	fireSound.setVolume(30.f);

	enemyTexture = &enemyTextures->get(EnemyZirael);
	sprite.setTexture(*enemyTexture);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2.f,
		sprite.getGlobalBounds().height / 2.f);
	sprite.setPosition(target.getSize().x / 2.f, -500.f);
	sprite.setScale(1.3f, 1.3f);

	toMove = false;
	moveRight = true;

	moveTime = sf::seconds(1.f);
	movingRate = sf::seconds(1.f);
	fireRate = sf::seconds(std::rand() % 2 + 1);
	spawnSummonsRate = sf::seconds(std::rand() % 8 + 10);

	summonCount = 0;
	summonCountMax = 16;

	sf::Vector2f enemyPos(sprite.getPosition());
	enemyStats = enemyBar(font, enemyPos, enemyTextures->get(Hp));
	ziraelText.setFont(font);
	ziraelText.setCharacterSize(25);
	ziraelText.setString("Zirael");
	ziraelText.setFillColor(sf::Color::Red);
	ziraelText.setStyle(sf::Text::Style::Bold);
	ziraelText.setOrigin(ziraelText.getGlobalBounds().width / 2.f,
		ziraelText.getGlobalBounds().height / 2.f);
}


Zirael::~Zirael()
{
}

void Zirael::update(sf::RenderTarget & target,
	std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State & state)
{
	if (state == State::GAME
		|| state == State::SHOP
		|| state == State::GAMEOVER)
	{
		if (state == State::GAMEOVER)
		{
			sprite.move(0.f, speed);
			enemyStats.hp.setString(std::to_string(hp) + "/" + std::to_string(hpMax));
			enemyStats.updateText(sprite.getPosition(), sprite.getGlobalBounds().width / 2.f);
			ziraelText.setPosition(sprite.getPosition().x,
				sprite.getPosition().y - sprite.getGlobalBounds().width / 2.f
				- ziraelText.getGlobalBounds().width / 2.f);
		}
		else
		{
			if (sprite.getPosition().y < target.getSize().y - 700.f)
				sprite.move(0.f, speed);
			else
			{
				speed = 5.f;

				//if it's time to spawn new summones
				if (spawnSummonsClock.getElapsedTime() >= spawnSummonsRate)
				{
					//spawn summons
					if (summonCount < summonCountMax)
					{
						enemies->push_back(std::unique_ptr<Enemy>(
							new ZiraelSummon(*window, textures, *textFont)));
						summonCount++;
						enemiesLeft++;
					}
					else
					{
						summonCount = 0;
						spawnSummonsRate = sf::seconds(std::rand() % 8 + 10);
						spawnSummonsClock.restart();
					}
				}

				//move zirael if move rate is passed
				if ((toMove == false) && (movingRateClock.getElapsedTime() >= movingRate))
				{
					toMove = true;
					moveRight = std::rand() % 2;
					movingRateClock.restart();
					moveClock.restart();
					moveTime = sf::seconds(std::rand() % 5 + 1);
					movingRate = sf::seconds(std::rand() % 2 + 1);
				}

				if (toMove)
				{
					//if it's time to move
					if (moveClock.getElapsedTime() <= moveTime)
					{
						//move right
						if (moveRight)
							sprite.move(speed, 0.f);
						//move left
						else
							sprite.move(-speed, 0.f);
					}
					//if player moved
					else
					{
						toMove = false;                //stop moving
						movingRateClock.restart();     //restart clock
					}
				}

				if (state != State::GAMEOVER)
					fire(enemyBullets);
			}
			checkWindowCollision(target);

			//set enemy bar position related to the enemy position
			enemyStats.hp.setString(std::to_string(hp) + "/" + std::to_string(hpMax));
			enemyStats.updateText(sprite.getPosition(), sprite.getGlobalBounds().width / 2.f,
				EnemyType::Zirael);
			ziraelText.setPosition(sprite.getPosition().x,
				sprite.getPosition().y - sprite.getGlobalBounds().width / 2.f);
		}
	}
}

void Zirael::renderEnemy(sf::RenderTarget & target)
{
	target.draw(sprite);
	target.draw(enemyStats.hearts);
	target.draw(enemyStats.hp);
	target.draw(ziraelText);
}

void Zirael::fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets)
{
	if (fireClock.getElapsedTime() >= fireRate)
	{
		ziraelBullet.setBulletPosition(this);
		enemyBullets->push_back(std::unique_ptr<Weapon>(new Bullet(ziraelBullet)));
		fireRate = sf::seconds(std::rand() % 3 + 1);
		fireSound.play();
		fireClock.restart();
	}
}

void Zirael::checkWindowCollision(sf::RenderTarget & target)
{
	//left side
	if (sprite.getGlobalBounds().left - 10.f <= 0.f)
	{
		sprite.setPosition(sprite.getGlobalBounds().width / 2.f + 10.f, sprite.getPosition().y);
		moveRight = true;
	}

	//right side
	else if (sprite.getPosition().x >= target.getSize().x - 10.f - sprite.getGlobalBounds().width / 2.f)
	{
		sprite.setPosition(target.getSize().x - 10.f
			- sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);
		moveRight = false;
	}
}
