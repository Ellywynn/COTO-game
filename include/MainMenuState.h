#pragma once
#include "State.h"
#include "Button.h"
#include "ResourceHolder.h"

class MainMenuState
{
public:
	MainMenuState(textureHolder* menuTextures,
		sf::Font* font, const sf::Vector2u& windowSize,
		sf::SoundBuffer& sound, const State& state);
	~MainMenuState();

	bool update(const sf::Vector2i& mousePos, sf::Event& event,
		sf::RenderWindow& target, State& state, sf::Music* game_music);
	void render(sf::RenderTarget& target, const State& state);

	bool newGame;
	sf::Music mainMenuMusic;
private:
	void initGraphics(const State& state);
	void initButtons();
	void updateBackgrounds();
private:
	sf::Sprite background_1;
	sf::Sprite background_2;

	sf::Sprite clouds_1;
	sf::Sprite clouds_2;
	sf::Sprite clouds_3;
	sf::Sprite pixel_spaceship;

	float clouds_1_speed;
	float clouds_2_speed;
	float clouds_3_speed;
	float pixel_spaceship_speed;

	sf::Text mainMenuText;
	sf::Text alphaText;
	std::map <std::string, Button*> mainMenuButtons;
	sf::Vector2u size;

	sf::Font* textFont;
	textureHolder* textures;
	sf::Sound buttonSound;
};

