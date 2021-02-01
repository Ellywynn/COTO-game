#include "../include/Explosion.h"



Explosion::Explosion(Animation& a, const sf::Vector2f& position,
	sf::SoundBuffer& sound)
	:anim(a)
{
	life = true;
	anim.sprite.setPosition(position);
	explosionSound.setBuffer(sound);
	explosionSound.setVolume(30.f);
}


Explosion::~Explosion()
{
}

void Explosion::update()
{
	anim.update();
	if (anim.isEnd())
		life = false;
}

void Explosion::play()
{
	explosionSound.play();
}
