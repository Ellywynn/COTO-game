#include "PauseState.h"


PauseState::PauseState(textureHolder* Textures, sf::Font* font,
	const sf::Vector2u & windowSize, sf::SoundBuffer& sound)
	:textures(Textures), textFont(font), size(windowSize)
{
	initGraphics();
	initButtons();
	buttonSound.setBuffer(sound);
	buttonSound.setVolume(30.f);
}

PauseState::~PauseState()
{
	for (auto it = pauseButtons.begin(); it != pauseButtons.end(); ++it)
		delete it->second;
}

void PauseState::initButtons()
{
	pauseButtons["Resume"] = new Button(background.getSize().x / 2.f,
		pauseText.getPosition().y + 100.f, 250.f, 68.f, textFont, "Resume Game");

	pauseButtons["Exit"] = new Button(background.getSize().x / 2.f,
		pauseButtons["Resume"]->getPosition().y + pauseButtons["Resume"]->getGlobalBounds().height
		+ 50.f, 250.f, 68.f, textFont, "Exit");
}

void PauseState::update(const sf::Vector2i& mousePos, sf::Event& event,
	sf::RenderWindow& target, State& state, sf::Music* mainMenu_music, sf::Music* game_music)
{
	if (state == State::PAUSE)
	{
		for (auto& i : pauseButtons)
			i.second->updateMenuButton(mousePos, event);

		if (pauseButtons["Resume"]->isButtonPressed())
		{
			buttonSound.play();
			state = State::GAME;
		}

		if (pauseButtons["Exit"]->isButtonPressed())
		{
			buttonSound.play();
			state = State::MAINMENU;
			game_music->stop();
			mainMenu_music->play();
		}
	}
}

void PauseState::render(sf::RenderTarget & target, const State& state)
{
	if (state == State::PAUSE)
	{
		target.draw(background);
		target.draw(pauseText);

		for (auto& it : pauseButtons)
			it.second->render(target);
	}
}

void PauseState::initGraphics()
{
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(size);
	background.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	background.setFillColor(sf::Color(30, 30, 30, 80));

	pauseText.setFont(*textFont);
	pauseText.setString("Paused");
	pauseText.setCharacterSize(24u);
	pauseText.setOrigin(pauseText.getGlobalBounds().width / 2.f,
		pauseText.getGlobalBounds().height / 2.f);
	pauseText.setPosition(windowSize.x / 2.f, windowSize.y - (2 * (windowSize.y / 3.f)));
}
