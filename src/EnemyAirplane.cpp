#include "../include/EnemyAirplane.h"



EnemyAirplane::EnemyAirplane()
{
}


EnemyAirplane::EnemyAirplane(const sf::RenderTarget& target,
	textureHolder* enemyTextures, const sf::Font & font, EnemyType type,
	soundHolder* sounds,
	const float& dmgMul, const float& hpMul, const float& rewardMul)
{
	//randomize spawn position
	float spawnx = std::rand() % static_cast<int>(target.getSize().x - 50.f) + 50;
	float spawny = std::rand() % 450 + 150;

	x = spawnx; y = -spawny;
	toMove = false;
	moveRight = true;
	enemyType = type;

	//init enemy
	switch (type)
	{
	case EnemyType::EAGLE:
		hpMax = (std::rand() % 3 + 2) * hpMul; //2-4
		enemyTexture = &enemyTextures->get(EnemyEagle);
		enemyBullet = Bullet(enemyTextures->get(EnemyBulletEagle),
			this, BulletType::EAGLE, dmgMul, WeaponType::EnemyBullet);
		speed = static_cast<float>(std::rand() % 3 + 3); //3-5
		killReward = (std::rand() % 15 + 15) * rewardMul; //15-30
		fireSound.setBuffer(sounds->get(EnemyBulletEagleSound));
		break;
	case EnemyType::RAPTOR:
		hpMax = (std::rand() % 3 + 4) * hpMul; //4-7
		enemyTexture = &enemyTextures->get(EnemyRaptor);
		enemyBullet = Bullet(enemyTextures->get(EnemyBulletRaptor),
			this, BulletType::RAPTOR, dmgMul, WeaponType::EnemyBullet);
		speed = static_cast<float>(std::rand() % 3 + 1.5); //1.5-3.5
		killReward = (std::rand() % 25 + 25) * rewardMul; //25-50
		fireSound.setBuffer(sounds->get(EnemyBulletRaptorSound));
		break;
	}
	hp = hpMax;
	fireSound.setVolume(10.f);

	//init time variables
	fireRate = sf::seconds(std::rand() % 5 + 1);
	movingTime = sf::seconds(std::rand() % 2 + 0.5);
	movingRate = sf::seconds(std::rand() % 5 + 1);


	//init enemy sprite
	sprite.setTexture(*enemyTexture);
	sprite.setOrigin((*enemyTexture).getSize().x / 2.f, (*enemyTexture).getSize().y / 2.f);
	sprite.setRotation(180.f);
	sprite.setScale(0.25f, 0.25f);
	sprite.setPosition(x, y);

	//init particle animation
	particle = Animation(enemyTextures->get(EnemyParticle), 0, 0, 32, 64, 16, 0.5f);
	particle.sprite.setRotation(180.f);
	particle.sprite.setScale(0.8f, 0.8f);
	particle.sprite.setOrigin(particle.sprite.getGlobalBounds().width / 2.f,
		particle.sprite.getGlobalBounds().height / 2.f);

	sf::Vector2f enemyPos(sprite.getPosition());
	enemyStats = enemyBar(font, enemyPos, enemyTextures->get(Hp));
}

EnemyAirplane::~EnemyAirplane()
{
}

void EnemyAirplane::update(sf::RenderTarget & target, std::vector<std::unique_ptr<Weapon>>* enemyBullets, const State & state)
{
	if (state == State::GAME
		|| state == State::SHOP
		|| state == State::GAMEOVER)
	{
		//if it's time to move, restart moving clock and moving rate clock
		if ((toMove == false) && (movingRateClock.getElapsedTime() >= movingRate))
		{
			toMove = true;
			moveRight = std::rand() % 2;
			movingRateClock.restart();
			movingClock.restart();
			movingRate = sf::seconds(std::rand() % 5 + 1);
		}

		if (toMove)
		{
			//if current moving time is less than final moving time, then move sprite
			if (movingClock.getElapsedTime() <= movingTime)
			{
				//right
				if (moveRight)
					sprite.move(speed, 0.f);
				//left
				else
					sprite.move(-speed, 0.f);
			}
			//if current moving time is grater than final moving time, then stop moving, restart moving rate clock
			else
			{
				toMove = false;
				movingRateClock.restart();
			}
		}

		sprite.move(0.f, speed);

		//if game is end, stop fire
		if (state != State::GAMEOVER)
			fire(enemyBullets);

		checkWindowCollision(target);

		//set enemy bar position related to the enemy position
		enemyStats.hp.setString(std::to_string(hp) + "/" + std::to_string(hpMax));
		enemyStats.updateText(sprite.getPosition(), sprite.getGlobalBounds().width / 2.f);

		//set particle sprite position related to the enemy position
		particle.sprite.setPosition(sprite.getPosition().x + 3.f,
			sprite.getPosition().y - sprite.getGlobalBounds().height / 2.f);
	}
	if (state != State::MAINMENU)
		particle.update();
}

void EnemyAirplane::renderEnemy(sf::RenderTarget & target)
{
	target.draw(particle.sprite);
	target.draw(sprite);
	target.draw(enemyStats.hearts);
	target.draw(enemyStats.hp);
}

void EnemyAirplane::fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets)
{
	if (fireClock.getElapsedTime() >= fireRate)
	{
		enemyBullet.setBulletPosition(this);
		enemyBullets->push_back(std::unique_ptr<Weapon>(new Bullet(enemyBullet)));
		fireRate = sf::seconds(std::rand() % 5 + 1);
		fireSound.play();
		fireClock.restart();
	}
}
