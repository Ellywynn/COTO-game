#pragma once
#include "Animation.h"
#include <SFML/Audio.hpp>

class Explosion
{
public:
	Explosion(Animation& a, const sf::Vector2f& position,
		sf::SoundBuffer& sound);
	~Explosion();

	Animation anim;
	bool life;

	void update();
	sf::Sound explosionSound;
	void play();
};

