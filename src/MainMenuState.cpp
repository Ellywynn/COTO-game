#include "../include/MainMenuState.h"


MainMenuState::MainMenuState(textureHolder * menuTextures,
	sf::Font * font, const sf::Vector2u & windowSize,
	sf::SoundBuffer& sound, const State& state)
	:textures(menuTextures), textFont(font), size(windowSize)
{
	initGraphics(state);
	initButtons();
	buttonSound.setBuffer(sound);
	buttonSound.setVolume(30.f);
}

MainMenuState::~MainMenuState()
{
	for (auto it = mainMenuButtons.begin(); it != mainMenuButtons.end(); ++it)
		delete it->second;
}

void MainMenuState::initButtons()
{
	mainMenuButtons["Play"] = new Button(background_1.getGlobalBounds().width / 2.f,
		background_1.getGlobalBounds().height / 2.f + 75.f, 250.f, 68.f, textFont, "Play", 16u);

	mainMenuButtons["Exit"] = new Button(background_1.getGlobalBounds().width / 2.f,
		mainMenuButtons["Play"]->getPosition().y
		+ mainMenuButtons["Play"]->getGlobalBounds().height + 30.f,
		250.f, 68.f, textFont, "Exit Game", 16u);
}

void MainMenuState::updateBackgrounds()
{
	//move and repeat main menu dynamic backgrounds

	background_1.move(0.7f, 0.f);
	background_2.move(0.7f, 0.f);

	clouds_1.move(clouds_1_speed, 0.f);
	clouds_2.move(clouds_2_speed, 0.f);
	clouds_3.move(clouds_3_speed, 0.f);
	pixel_spaceship.move(pixel_spaceship_speed, 0.f);

	if (background_1.getGlobalBounds().left > size.x)
		background_1.setPosition(-background_1.getGlobalBounds().width, 0.f);
	else if ((background_2.getGlobalBounds().left > size.x))
		background_2.setPosition(-background_2.getGlobalBounds().width, 0.f);

	if (clouds_1.getGlobalBounds().left > size.x + 1500.f)
	{
		clouds_1.setPosition(size.x - (std::rand() % 1000 + 3000.f), std::rand() % 240 + 90.f);
		clouds_1_speed = std::rand() % 3 + 5.f;
	}
	else if (clouds_2.getGlobalBounds().left > size.x + 1500.f)
	{
		clouds_2.setPosition(size.x - (std::rand() % 1000 + 2500.f), std::rand() % 300 + 400.f);
		clouds_2_speed = std::rand() % 3 + 5.f;
	}
	else if (clouds_3.getGlobalBounds().left > size.x + 1500.f)
	{
		clouds_3.setPosition(size.x - (std::rand() % 1000 + 2000.f), std::rand() % 400 + 480.f);
		clouds_3_speed = std::rand() % 3 + 5.f;
	}
	else if (pixel_spaceship.getGlobalBounds().left > size.x + 1500.f)
	{
		pixel_spaceship.setPosition(size.x - (std::rand() % 1000 + 2700.f), std::rand() % 340 + 560.f);
		pixel_spaceship_speed = std::rand() % 3 + 5.f;
	}
}

bool MainMenuState::update(const sf::Vector2i & mousePos, sf::Event & event,
	sf::RenderWindow & target, State & state, sf::Music* game_music)
{
	if (state == State::MAINMENU)
	{
		updateBackgrounds();

		for (auto& i : mainMenuButtons)
			i.second->updateMenuButton(mousePos, event);

		if (mainMenuButtons["Play"]->isButtonPressed())
		{
			buttonSound.play();
			newGame = true;
			mainMenuMusic.stop();
			game_music->play();
		}

		if (mainMenuButtons["Exit"]->isButtonPressed())
		{
			buttonSound.play();
			target.close();
		}
	}
	return newGame;
}

void MainMenuState::render(sf::RenderTarget & target, const State & state)
{
	if (state == State::MAINMENU)
	{
		target.draw(background_1);
		target.draw(background_2);
		target.draw(clouds_1);
		target.draw(clouds_2);
		target.draw(clouds_3);
		target.draw(pixel_spaceship);
		target.draw(mainMenuText);
		target.draw(alphaText);

		for (auto& it : mainMenuButtons)
			it.second->render(target);
	}
}

void MainMenuState::initGraphics(const State& state)
{
	newGame = false;

	background_1.setTexture(textures->get(MainMenuBackground));
	clouds_1.setTexture(textures->get(MainMenuCloud_1));
	clouds_2.setTexture(textures->get(MainMenuCloud_2));
	clouds_3.setTexture(textures->get(MainMenuCloud_3));
	pixel_spaceship.setTexture(textures->get(MainMenuPixelShip));
	background_2 = background_1;
	background_2.setPosition(-background_1.getGlobalBounds().width, 0.f);
	clouds_1.setScale(0.5f, 0.5f);
	clouds_2.setScale(0.5f, 0.5f);
	clouds_3.setScale(0.25f, 0.25f);
	pixel_spaceship.setScale(0.5f, 0.5f);

	clouds_1.setPosition(size.x - (std::rand() % 1000 + 3000.f), std::rand() % 240 + 90.f);
	clouds_2.setPosition(size.x - (std::rand() % 1000 + 2500.f), std::rand() % 300 + 400.f);
	clouds_3.setPosition(size.x - (std::rand() % 1000 + 2000.f), std::rand() % 400 + 480.f);
	pixel_spaceship.setPosition(size.x - (std::rand() % 1000 + 2700.f), std::rand() % 340 + 560.f);

	clouds_1_speed = std::rand() % 3 + 5.f;
	clouds_2_speed = std::rand() % 3 + 5.f;
	clouds_3_speed = std::rand() % 3 + 5.f;
	pixel_spaceship_speed = std::rand() % 3 + 5.f;

	mainMenuText.setFont(*textFont);
	mainMenuText.setCharacterSize(40u);
	mainMenuText.setString("Confrontation of the Obscurity");
	mainMenuText.setOrigin(mainMenuText.getGlobalBounds().width / 2.f,
		mainMenuText.getGlobalBounds().height / 2.f);
	mainMenuText.setPosition(background_1.getGlobalBounds().width / 2.f,
		background_1.getGlobalBounds().height - (2 * (background_1.getGlobalBounds().height / 3.f)));

	alphaText.setFont(*textFont);
	alphaText.setCharacterSize(20u);
	alphaText.setString("Alpha v0.1");
	alphaText.setPosition(size.x - alphaText.getGlobalBounds().width - 5.f,
		size.y - alphaText.getGlobalBounds().height - 5.f);

	mainMenuMusic.openFromFile("Assets/Music/main_menu_music.ogg");
	mainMenuMusic.setLoop(true);
	mainMenuMusic.setVolume(5.f);
	if (state == State::MAINMENU)
		mainMenuMusic.play();
}
