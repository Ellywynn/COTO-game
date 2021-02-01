#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity() {}
	sf::Sprite sprite;
	float speed;
	float x, y;
};