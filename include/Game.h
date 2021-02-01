#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

#include "Player.h"
#include "Enemy.h"
#include "EnemyAirplane.h"
#include "Zirael.h"
#include "Vattgern.h"

#include "Bullet.h"
#include "ResourceHolder.h"
#include "Explosion.h"

#include "Level.h"
#include "State.h"
#include "Button.h"
#include "GameShop.h"
#include "PauseState.h"
#include "MainMenuState.h"

class Game
{
public:
	Game();
	~Game();

	bool running();

	void update();
	void render();

	void run();
private:
	//update
	void updateBackground();
	void updatePlayer();
	void updateEnemies();
	void updateExplosions();
	void updateGUI();
	void updateShop(sf::Event& e);
	void updatePauseMenu(sf::Event& e);
	void updateMainMenu(sf::Event& e);
	void updateNextWaveText();
	void updatePrepareState();
	void updateCursor();

	//render
	void renderBackground();
	void renderPlayer();
	void renderEnemy();
	void renderBullets();
	void renderExplosions();
	void renderGUI();
	void renderShop();
	void renderPauseMenu();
	void renderMainMenu();

	//Functions
	void initWindow();
	void initVariables();
	void restart();
	sf::Event& pollEvents();
	bool isEnemyKill(Enemy& enemy);//did enemy died
	void clearScene();             //delete all entitites
	void spawnWave();              //spawn enemies
	void checkCollisions();		   //update all collisions
	void checkWindowCollision();   //check all window collisions
	void updateBullets();
	void endGame();    

	void loadResources();
	void initGUI();
private:
	bool fullscreen;
	bool isEnd;
	bool prepareState;

	State currentState;

	std::unique_ptr<sf::RenderWindow> window;

	int score, money;
	int maxWaveEnemies;
	int enemyKilled;
	int currentKillToHeal;
	int killToHealMax;
	int lives;

	textureHolder textures;
	fontHolder fonts;
	soundHolder sounds;

	Player player;

	sf::Music gameMusic;
	GameSounds gameSounds;

	std::unique_ptr<GameShop> shop;
	std::unique_ptr<PauseState> pause;
	std::unique_ptr<MainMenuState> mainMenu;
	
	//level stuff
	Level level;
	int currentWaveEnemyCount;
	int microwaveEnemyCount;
	int maxMicrowaveEnemyCount;
	bool nextWave;
	int waveCount;
	int enemiesRemaining;
	bool restarted;
	bool toSpawn;
	bool showNextWaveText;
	bool showSpawnWaveText;

	//polymorphic containers
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::vector<std::unique_ptr<Weapon>> enemyBullets;
	std::vector<std::unique_ptr<Weapon>> playerBullets;
	std::vector<Explosion> explosions;


	//GUI
	GUI gui;

	bool shiftMode;              //if player in the shift mode right now
	bool isInvulnerability;      //player is invulnerable after death
	bool isHealed;               //when player kills some enemy, it will be healed
};