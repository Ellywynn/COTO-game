#pragma once
#include "Button.h"
#include "ResourceHolder.h"
#include "Player.h"
#include "State.h"

#include <map>

struct ShopGUI
{
	sf::Text armorText;
	sf::Text damageText;
	sf::Text healthText;
	sf::Text attackSpeedText;

	sf::Text currentStats;
};

class GameShop
{
public:
	GameShop();
	GameShop(textureHolder* textures, sf::Font* font, const sf::Vector2u& size,
		Player* pl, sf::SoundBuffer& sound);
	~GameShop();

	void update(const sf::Vector2i& mousePos, int& money,
		sf::Event& event, const State& state);
	void render(sf::RenderTarget& target, const State& state);

private:
	void initButtons();
	void initTexts();
	void initGraphics();
	void updateButtons(int& money);
private:
	bool isOpen;
	sf::Sprite background;
	sf::Sprite moneySprite;
	sf::RectangleShape mask;

	std::map <std::string, Button*> shopButtons;

	sf::Vector2u windowSize;
	textureHolder* textures;
	sf::Font* buttonFont;
	sf::Sound buttonSound;

	Player* player;
	ShopGUI gui;

	sf::Text message;
	sf::Text currentMoneyText;

	int disappearance;          //text disappeatance effect
	bool showMessage;
	sf::Clock messageClock;
	sf::Time messageDuration;
};

