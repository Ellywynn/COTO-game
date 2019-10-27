#include "Player.h"
#include <iostream>


Player::Player()
{
	initPlayer();
}


Player::~Player()
{
}

void Player::update(sf::RenderTarget& target,
	std::vector<std::unique_ptr<Weapon>>& playerBullets,
	bool& shiftMode, const State& state)
{
	updateInput(playerBullets, state);
	updateParticles();
	if (state == State::GAME || state == State::SHOP)
		checkWindowCollision(target);
	shiftMode = shift;
}

void Player::render(sf::RenderTarget & target)
{
	target.draw(particle.sprite);
	target.draw(sprite);
}

void Player::setSpawnPosition(sf::RenderTarget & target)
{
	x = target.getSize().x / 2.f;
	y = target.getSize().y - 150.f;

	sprite.setPosition(x, y);
}

void Player::setArmor(float _armor)
{
	armor = _armor;
}

float Player::getArmor() const
{
	return armor;
}

void Player::takeDamage(int damage)
{
	hp -= (damage - damage * armor);
	if (hp <= 0)
		hp = 0;
}

const int Player::getHpMax() const
{
	return hpMax;
}

int Player::getHp() const
{
	return hp;
}

void Player::setHp(int _hp)
{
	hp = _hp;
	if (hp > hpMax)
		hp = hpMax;
}

void Player::setHpMax(int _hpMax)
{
	hpMax = _hpMax;
}

const sf::Vector2f & Player::getPosition() const
{
	return sprite.getPosition();
}

void Player::setBulletAS(float as)
{
	bulletAS = as;
	bulletFireRate = sf::seconds(bulletAS);
}

void Player::setMissileAS(float as)
{
	missileAS = as;
	missileFireRate = sf::seconds(missileAS);
}

void Player::updateInput(std::vector<std::unique_ptr<Weapon>>& playerBullets, const State& state)
{
	if (state == State::GAME || state == State::SHOP)
	{
		//if player isn't in the shift mode it can go left or right
		if (!shift)
		{
			//left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				sprite.move(-speed, 0.f);
				moveRight = false;
			}
			//right
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				sprite.move(speed, 0.f);
				moveRight = true;
			}
		}
		//Up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			sprite.move(0.f, -speed);
		//Down
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			sprite.move(0.f, speed);

		//if shift mode is active, shift player
		if (!shift)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				shift = true;
				shiftSound.play();
				shiftingTimer.restart();
			}

		if (shift)
		{
			if (shiftingTimer.getElapsedTime() <= shiftingDuration)
			{ 
				//shift player depending to the last side direction
				if (moveRight)
					sprite.move(shiftingSpeed - (shiftingSpeed * shiftingTimer.getElapsedTime().asSeconds() * 2), 0.f);
				else
					sprite.move(-(shiftingSpeed - (shiftingSpeed * shiftingTimer.getElapsedTime().asSeconds() * 2)), 0.f);
			}
			else
				shift = false;
		}
	}

	if (state == State::GAME)
	{
		//fire bullet
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			fireBullet(playerBullets);

		//fire missile
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
			fireMissile(playerBullets);
	}
}

void Player::updateParticles()
{
	particle.sprite.setPosition(sprite.getPosition().x - 3.f,
		sprite.getPosition().y + sprite.getGlobalBounds().height / 2.f);
	particle.update();
}

void Player::fireBullet(std::vector<std::unique_ptr<Weapon>>& playerBullets)
{
	if (bulletFireClock.getElapsedTime() >= bulletFireRate)
	{
		//push bullet and restart the fire rate clock
		playerBullet.setBulletPosition(this);
		playerBullets.push_back(std::unique_ptr<Bullet>(new Bullet(playerBullet)));
		bulletSound.play();
		bulletFireClock.restart();
	}
}

void Player::fireMissile(std::vector<std::unique_ptr<Weapon>>& playerBullets)
{
	if (missileFireClock.getElapsedTime() >= missileFireRate)
	{
		//push bullet and restart the fire rate clock
		playerMissile.setMissilePosition(this);
		playerBullets.push_back(std::unique_ptr<Missile>(new Missile(playerMissile)));
		missileSound.play();
		missileFireClock.restart();
	}
}

void Player::initPlayer()
{
	//load textures
	textures.load(PlayerBullet, "Assets/Textures/player_bolt.png");
	textures.load(PlayerParticle, "Assets/Particles/player_particle.png");
	textures.load(PlayerMissile, "Assets/Textures/player_missile.png");
	textures.load(PlayerShip, "Assets/Textures/player_ship.png");

	//load sounds
	sounds.load(PlayerShiftSound, "Assets/Sounds/player_blurring.wav");
	sounds.load(PlayerBulletSound, "Assets/Sounds/player_bullet.wav");
	sounds.load(PlayerMissileSound, "Assets/Sounds/player_missile.wav");

	//initialize player 
	hpMax = 100;
	hp = hpMax;
	bulletAS = 0.3f;
	missileAS = 4.f;
	armor = 0.f;
	bulletFireRate = sf::seconds(bulletAS);
	missileFireRate = sf::seconds(missileAS);
	shiftingDuration = sf::seconds(0.2f);
	speed = 10.f;
	shift = false;
	moveRight = false;
	shiftingSpeed = 35.f;

	//init sounds
	shiftSound.setBuffer(sounds.get(PlayerShiftSound));
	bulletSound.setBuffer(sounds.get(PlayerBulletSound));
	missileSound.setBuffer(sounds.get(PlayerMissileSound));

	shiftSound.setVolume(15.f);
	bulletSound.setVolume(20.f);
	missileSound.setVolume(20.f);

	//init player sprite
	sprite.setOrigin(textures.get(PlayerShip).getSize().x / 2.f,
		textures.get(PlayerShip).getSize().y / 2.f);
	sprite.setTexture(textures.get(PlayerShip));
	sprite.setScale(0.3f, 0.3f);

	//init weapons
	playerBullet = Bullet(textures.get(PlayerBullet), this, BulletType::PLAYER);
	playerMissile = Missile(textures.get(PlayerMissile), this);

	//init particle
	particle = Animation(textures.get(PlayerParticle), 0, 0, 32, 64, 16, 0.5f);
	particle.sprite.setScale(0.8f, 0.8f);
	particle.sprite.setOrigin(particle.sprite.getGlobalBounds().width / 2.f,
		particle.sprite.getGlobalBounds().height / 2.f);
}

void Player::checkWindowCollision(sf::RenderTarget & target)
{
	//left side
	if (sprite.getGlobalBounds().left <= 0.f)
		sprite.setPosition(sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);
	//right side
	else if (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width >= target.getSize().x)
		sprite.setPosition(target.getSize().x - sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);

	//top
	if (sprite.getGlobalBounds().top <= (target.getSize().y - 800.f))
		sprite.setPosition(sprite.getPosition().x,
			target.getSize().y - 800.f + sprite.getGlobalBounds().height / 2.f);
	//down
	else if (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height >= target.getSize().y)
		sprite.setPosition(sprite.getPosition().x,
			target.getSize().y - sprite.getGlobalBounds().height / 2.f);


	//particle left
	if (particle.sprite.getGlobalBounds().left <= 0.f + sprite.getGlobalBounds().width / 2.f)
		particle.sprite.setPosition(sprite.getGlobalBounds().width / 2.f - 3.f,
			particle.sprite.getPosition().y);

	//particle right side
	else if (particle.sprite.getGlobalBounds().left
		+ particle.sprite.getGlobalBounds().width > target.getSize().x - sprite.getGlobalBounds().width / 2.f)
		particle.sprite.setPosition(target.getSize().x - sprite.getGlobalBounds().width / 2.f - 3.f,
			particle.sprite.getPosition().y);

	//particle top
	if (particle.sprite.getGlobalBounds().top <= (target.getSize().y - 800.f + sprite.getGlobalBounds().height))
		particle.sprite.setPosition(particle.sprite.getPosition().x,
			target.getSize().y - 800.f + sprite.getGlobalBounds().height / 2.f
			+ particle.sprite.getGlobalBounds().height - 5.f);

	//particle bottom
	else if (particle.sprite.getGlobalBounds().top + particle.sprite.getGlobalBounds().height / 2.f
		>= target.getSize().y)
		particle.sprite.setPosition(particle.sprite.getPosition().x,
			target.getSize().y);
}