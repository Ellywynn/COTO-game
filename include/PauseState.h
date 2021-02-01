#pragma once
#include "Button.h"
#include "ResourceHolder.h"
#include "State.h"

class PauseState
{
public:
	PauseState(textureHolder* Textures, sf::Font* font,
		const sf::Vector2u& windowSize, sf::SoundBuffer& sound);
	~PauseState();

	void update(const sf::Vector2i& mousePos, sf::Event& event,
		sf::RenderWindow& target, State& state, sf::Music* mainMenu_music, sf::Music* game_music);
	void render(sf::RenderTarget& target, const State& state);
private:
	void initGraphics();
	void initButtons();
private:
	sf::RectangleShape background;
	sf::Text pauseText;
	std::map <std::string, Button*> pauseButtons;
	sf::Vector2u size;

	sf::Font* textFont;
	textureHolder* textures;
	sf::Sound buttonSound;
};

