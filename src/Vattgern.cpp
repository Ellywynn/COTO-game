#include "../include/Vattgern.h"



Vattgern::Vattgern(const sf::RenderTarget& target,
	textureHolder* enemyTextures, const sf::Font & font,
	soundHolder* sounds)
{
	speed = 12.f;
	shiftingSpeed = 28.f;
	hpMax = 700;
	hp = hpMax;
	killReward = 30000;
	enemyType = EnemyType::Vattgern;

	vattgernBullet = Bullet(enemyTextures->get(VattgernBullet), this,
		BulletType::VATTGERN, 1.f, WeaponType::VattgernBullet);

	fireSound.setBuffer(sounds->get(VattgernBulletSound));
	shiftSound.setBuffer(sounds->get(VattgernShiftSound));

	fireSound.setVolume(30.f);
	shiftSound.setVolume(30.f);

	enemyTexture = &enemyTextures->get(EnemyVattgern);
	sprite.setTexture(*enemyTexture);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2.f,
		sprite.getGlobalBounds().height / 2.f);
	sprite.setPosition(target.getSize().x / 2.f, -500.f);
	sprite.setScale(0.8f, 0.8f);
	sprite.setRotation(90.f);

	toMove = false;
	moveRight = true;

	moveTime = sf::seconds(1.f);
	movingRate = sf::seconds(1.f);
	fireRate = sf::seconds(std::rand() % 1 + 0.5);
	shiftingRate = sf::seconds(std::rand() % 2 + 0.5);
	shiftingDuration = sf::seconds(0.3f);

	sf::Vector2f enemyPos(sprite.getPosition());
	enemyStats = enemyBar(font, enemyPos, enemyTextures->get(Hp));
	vattgernText.setFont(font);
	vattgernText.setCharacterSize(25);
	vattgernText.setString("Vattgern");
	vattgernText.setFillColor(sf::Color::Red);
	vattgernText.setStyle(sf::Text::Style::Bold);
	vattgernText.setOrigin(vattgernText.getGlobalBounds().width / 2.f,
		vattgernText.getGlobalBounds().height / 2.f);
}


Vattgern::~Vattgern()
{
}

void Vattgern::update(sf::RenderTarget & target,
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
			vattgernText.setPosition(sprite.getPosition().x,
				sprite.getPosition().y - sprite.getGlobalBounds().width / 2.f
				- vattgernText.getGlobalBounds().width / 2.f);
		}
		else
		{
			if (sprite.getPosition().y < target.getSize().y - 700.f)
				sprite.move(0.f, speed);
			else
			{
				speed = 8.f;

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
					else
					{
						toMove = false;                //stop moving
						movingRateClock.restart();     //restart clock
					}
				}

				if (!shift)
				{
					if (shiftingTimer.getElapsedTime() >= shiftingRate)
					{
						//if it's time to shift, set the shift mode to true
						shift = true;
						shiftSound.play();
						shiftingRate = sf::seconds(std::rand() % 2 + 0.5);
						shiftingTimer.restart();
					}
				}

				if (shift)
				{
					if (shiftingTimer.getElapsedTime() <= shiftingDuration)
					{
						//move vattgern
						if (moveRight)
							sprite.move(shiftingSpeed
								- (shiftingSpeed * shiftingTimer.getElapsedTime().asSeconds() * 2), 0.f);
						else
							sprite.move(-(shiftingSpeed
								- (shiftingSpeed * shiftingTimer.getElapsedTime().asSeconds() * 2)), 0.f);
					}
					else
						shift = false;
				}

				if (state != State::GAMEOVER)
					fire(enemyBullets);
			}
			checkWindowCollision(target);

			//set enemy bar position related to the enemy position
			enemyStats.hp.setString(std::to_string(hp) + "/" + std::to_string(hpMax));
			enemyStats.updateText(sprite.getPosition(), sprite.getGlobalBounds().width / 2.f,
				EnemyType::Vattgern);
			vattgernText.setPosition(sprite.getPosition().x,
				sprite.getPosition().y - sprite.getGlobalBounds().width / 2.f - 50.f);
		}
	}
}

void Vattgern::renderEnemy(sf::RenderTarget & target)
{
	target.draw(sprite);
	target.draw(enemyStats.hearts);
	target.draw(enemyStats.hp);
	target.draw(vattgernText);
}

void Vattgern::checkWindowCollision(sf::RenderTarget & target)
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

void Vattgern::fire(std::vector<std::unique_ptr<Weapon>>* enemyBullets)
{
	if (fireClock.getElapsedTime() >= fireRate)
	{
		vattgernBullet.setBulletPosition(this);
		enemyBullets->push_back(std::unique_ptr<Weapon>(new Bullet(vattgernBullet)));
		fireRate = sf::seconds(std::rand() % 2 + 0.5); //0.5 - 1.5
		fireSound.play();
		fireClock.restart();
	}
}
