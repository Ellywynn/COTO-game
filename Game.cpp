#include "Game.h"



Game::Game()
{
	fullscreen = true;
	currentState = State::MAINMENU;

	initWindow();

	loadResources();
	initGUI();

	initVariables();
}


Game::~Game()
{
}

bool Game::running()
{
	return window->isOpen();
}

void Game::update()
{
	sf::Event e = pollEvents();
	updateBackground();
	updateCursor();
	updateMainMenu(e);
	updatePrepareState();
	spawnWave();
	updatePlayer();
	updateEnemies();
	updateBullets();
	checkCollisions();
	checkWindowCollision();
	updateExplosions();
	updateShop(e);
	updatePauseMenu(e);
	updateGUI();
}

void Game::render()
{
	window->clear();

	renderBackground();
	renderPlayer();
	renderEnemy();
	renderBullets();
	renderExplosions();
	renderShop();
	renderGUI();
	renderMainMenu();
	renderPauseMenu();

	window->display();
}

void Game::run()
{
	update();
	render();
}

void Game::initWindow()
{
	if (fullscreen)
		window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(1920, 1080),
			"Confrontation of the Obscurity", sf::Style::Fullscreen));
	else
		window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(1920, 1080),
			"Confrontation of the Obscurity"));
	window->setFramerateLimit(60);
}

void Game::initVariables()
{
	isEnd = false;
	isHealed = false;
	isInvulnerability = false;
	prepareState = true;

	score = 0;
	money = 0;
	lives = 3;
	player.setHp(player.getHpMax());
	enemyKilled = 0;
	killToHealMax = 10;
	currentKillToHeal = killToHealMax;

	gui.healTextDisappearance = 255;
	gui.waveTextDisappearance = 255;
	gui.waveTextAppearance = 0;
	gui.startWaveTextDisappearance = 255;
	gui.startWaveTextAppearance = 0;
	gui.minutes = 0;
	gui.seconds = 0;

	nextWave = false;
	toSpawn = true;
	restarted = false;
	shiftMode = false;
	showNextWaveText = false;
	showSpawnWaveText = false;
	level.setCurrentWave(1);
	maxWaveEnemies = level.getEnemyLevelCount(level.getCurrentWave());
	enemiesRemaining = maxWaveEnemies;
	waveCount = level.getCurrentWave();
	currentWaveEnemyCount = 0;
	microwaveEnemyCount = 0;
	maxMicrowaveEnemyCount = 15;

	gui.blurringDuration = 0.f;
	gui.blurringTime = sf::seconds(0.1f);
	gui.getReadyTime = sf::seconds(3.f);
	gui.enemySpawnRate = sf::seconds(0.5f);
	gui.killToHealVisibleTime = sf::seconds(2.f);
	gui.enemySpawnRate = sf::seconds(0.5f);
	gui.microwaveRate = sf::seconds(10.f);
	gui.nextWavePrepareTime = sf::seconds(10.f);
	gui.nextWaveTextDuration = sf::seconds(7.f);
	gui.nextWaveStartTextDuration = sf::seconds(3.f);
	gui.prepareDuration = sf::seconds(10.f);

	gui.background_1.setTexture(textures.get(Background));
	gui.background_1.setPosition(0.f, 0.f);
	gui.background_2.setTexture(textures.get(Background));
	gui.background_2.setPosition(gui.background_1.getPosition().x,
		gui.background_1.getPosition().y - gui.background_2.getGlobalBounds().height);

	player.reset();
	player.setSpawnPosition(*window);

	sf::Vector2u size = window->getSize();
	shop = std::unique_ptr<GameShop>(new GameShop(&textures, &fonts.get(Arial),
		size, &player, sounds.get(ButtonSound)));
	pause = std::unique_ptr<PauseState>(new PauseState(&textures, &fonts.get(Arial),
		size, sounds.get(ButtonSound)));
	mainMenu = std::unique_ptr<MainMenuState>(new MainMenuState(&textures, &fonts.get(Arial),
		size, sounds.get(ButtonSound), currentState));

	gameMusic.setVolume(15.f);
	if (currentState == State::GAME)
		gameMusic.play();
	gui.gameTimer.restart();
	gui.gameDuration.setString("Time:    " + std::to_string(gui.minutes)
		+ ":" + std::to_string(gui.seconds));
	gui.prepareTimer.restart();
}

void Game::updateEnemies()
{
	for (size_t i = 0; i < enemies.size(); ++i)
		enemies[i]->update(*window, &enemyBullets, currentState);
}

void Game::renderEnemy()
{
	if (currentState != State::MAINMENU)
	{
		for (size_t i = 0; i < enemies.size(); ++i)
			enemies[i]->renderEnemy(*window);
	}
}

void Game::updatePlayer()
{
	player.update(*window, playerBullets, shiftMode, currentState);
	if (lives < 1)
	{
		lives = 0;
		player.setHp(0);
		if (currentState == State::GAME)
			isEnd = true;
		if (currentState != State::GAMEOVER && currentState != State::PAUSE && currentState != State::MAINMENU)
			currentState = State::GAMEOVER;
	}
}

void Game::renderPlayer()
{
	if (currentState != State::GAMEOVER && currentState != State::MAINMENU && isEnd == false)
	{
		if (isInvulnerability == false || currentState == State::PAUSE)
			player.render(*window);
		else if (isInvulnerability == true && (currentState == State::GAME || currentState == State::SHOP))
		{
			if (gui.blurringClock.getElapsedTime() <= gui.blurringTime)
				player.render(*window);
			else if (gui.blurringClock.getElapsedTime() <= (gui.blurringTime + gui.blurringTime))
				/*nothing*/;
			else
			{
				gui.blurringDuration += gui.blurringClock.getElapsedTime().asSeconds();

				if (sf::seconds(gui.blurringDuration) >= sf::seconds(3.f))
					isInvulnerability = false;
				else
					gui.blurringClock.restart();
			}
		}
	}
}

sf::Event& Game::pollEvents()
{
	sf::Event e;
	while (window->pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			window->close(); break;
		case sf::Event::KeyPressed:
			switch (e.key.code)
			{
			case sf::Keyboard::Escape:
				switch (currentState)
				{
				case State::PAUSE:
					if (isEnd == false)
					{
						currentState = State::GAME;
						gameMusic.play();
					}
					else
						currentState = State::GAMEOVER;
					break;
				case State::GAMEOVER:
					currentState = State::PAUSE;
					break;
				case State::END:
					currentState = State::PAUSE;
					break;
				case State::GAME:
					currentState = State::PAUSE;
					gameMusic.pause();
					break;
				case State::SHOP:
					currentState = State::GAME;
					break;
				}
				break;

			case sf::Keyboard::F11:
				window->close();
				fullscreen = !fullscreen;
				initWindow();
				break;
			case sf::Keyboard::Tab:
				if (currentState == State::GAME)
					currentState = State::SHOP;
				else if (currentState == State::SHOP)
					currentState = State::GAME;
				break;

			case sf::Keyboard::R:
				if (currentState == State::GAMEOVER || currentState == State::END)
					restart();
				break;
			}
			break;
		}
	}
	return e;
}

void Game::spawnWave()
{
	//spawn enemies in game or shop mode
	if (currentState == State::GAME || currentState == State::SHOP)
	{
		if (!prepareState)
		{
			//if it's time to prepare for the next wave
			if (nextWave == true)
			{
				//if there's no enemies in the scene and prepare timer is not started
				if ((enemies.empty()) && restarted == false)
				{
					level.nextWave();                   //increment the wave
					//update maximum enemy wave count
					maxWaveEnemies = level.getEnemyLevelCount(level.getCurrentWave());
					waveCount++;                        //increase current wave count
					showNextWaveText = true;            //show the next wave text
					enemiesRemaining = maxWaveEnemies;  //reset the enemy counter
					gui.nextWaveTextClock.restart();    //start the next wave text timer
					gui.nextWaveClock.restart();        //start wave prepare time
					restarted = true;                   //set the timer to the restarted mode
					updateNextWaveText();
				}

				//if prepare time is over, start spawn enemies
				if (gui.nextWaveClock.getElapsedTime() >= gui.nextWavePrepareTime && enemies.empty())
				{
					nextWave = false;
					toSpawn = true;
					showSpawnWaveText = true;
					gui.nextWaveStartClock.restart();
					gui.enemySpawnClock.restart();
					gameSounds.startWaveSound.play();
				}
			}

			//wait for microwave time gap
			if (toSpawn == false && nextWave == false)
			{
				if (gui.enemyMicrowaveClock.getElapsedTime() >= gui.microwaveRate)
				{
					toSpawn = true;
					gui.enemySpawnClock.restart();
				}
			}

			//if it's time to spawn enemies
			if (toSpawn)
			{
				//if enemy spawn rate time has passed, spawn the new enemy
				if (gui.enemySpawnClock.getElapsedTime() >= gui.enemySpawnRate)
				{
					//if there are enemies to spawn
					if (currentWaveEnemyCount < maxWaveEnemies)
					{
						//if it's common wave
						if (waveCount != 10 && waveCount != 20)
						{
							//spawn enemy if microwave timer allows to do it
							if (microwaveEnemyCount < maxMicrowaveEnemyCount)
							{
								//randomize enemy
								EnemyType type = static_cast<EnemyType>(rand() % static_cast<int>(EnemyType::COUNT));

								//initialize enemy
								if (type == EnemyType::EAGLE || type == EnemyType::RAPTOR)
								{
									float dmgMul = level.getDamageMul(waveCount);    //set damage wave multiplyer
									float hpMul = level.getHpMul(waveCount);         //set hp wave multiplyer
									float rewardMul = level.getRewardMul(waveCount); //set reward wave multiplyer

									//spawn enemy
									enemies.push_back(std::unique_ptr<Enemy>(
										new EnemyAirplane(*window, &textures, fonts.get(Arial), type,
											&sounds, dmgMul, hpMul, rewardMul)));
									microwaveEnemyCount++;                           //increase current microwave enemy count
									currentWaveEnemyCount++;                         //increase current wave enemy count
								}
								gui.enemySpawnClock.restart();                       //restart enemy spawn clock
							}
							else
							{
								//if microwave is over

								toSpawn = false;                                     //stop spawn enemies
								gui.enemyMicrowaveClock.restart();                   //restart microwave spawn clock
								microwaveEnemyCount = 0;                             //reset current microwave enemy count
							}
						}
						//if it's zirael wave
						else if (waveCount == 10)
						{
							enemies.push_back(std::unique_ptr<Enemy>(
								new Zirael(*window, &textures, fonts.get(Arial),
									&enemies, enemiesRemaining, sounds.get(ZiraelBulletSound))));
							currentWaveEnemyCount++;
						}
						//if it's vattgern wave
						else if (waveCount == 20)
						{
							enemies.push_back(std::unique_ptr<Enemy>(
								new Vattgern(*window, &textures, fonts.get(Arial), &sounds)));
							currentWaveEnemyCount++;
						}
					}
					else
					{
						//if the wave is over

						toSpawn = false;                                        //stop spawn enemies
						restarted = false;                                      //restart prepare timer
						nextWave = true;                                        //start preparing to the next wave
						currentWaveEnemyCount = 0;                              //reset current wave enemy count
						microwaveEnemyCount = 0;                                //reset current microwave enemy count
						if (level.giveLife() && lives < 3)
							lives++;                                            //add life every 5 passed waves
					}
				}
			}
		}
	}
}

void Game::checkCollisions()
{
	if (currentState == State::GAME || currentState == State::SHOP)
	{
		//PLAYER BULLET AND ENEMY COLLISION
		//for each player bullet
		for (size_t b = 0; b < playerBullets.size(); ++b)
		{
			//if enemy collides with player bullets
			bool collide = false;

			for (size_t e = 0; e < enemies.size(); ++e)
			{
				//if collide, set the bool to true
				if (enemies[e]->sprite.getGlobalBounds().intersects(playerBullets[b]->sprite.getGlobalBounds()))
				{
					collide = true;
					break;
				}
			}

			if (collide)
			{
				//for each enemy
				for (size_t e = 0; e < enemies.size(); ++e)
				{
					//if enemy intersects with the missile area
					if (enemies[e]->sprite.getGlobalBounds().intersects(playerBullets[b]->collider.getGlobalBounds()))
					{
						//enemy take damage
						enemies[e]->takeDamage(playerBullets[b]->getWeaponDamage());
						//if enemy has been killed
						if (isEnemyKill(*enemies[e]))
						{
							//delete enemy
							enemies.erase(enemies.begin() + e);
							enemiesRemaining--;
							//if current weapon is missile, continue to check every enemy
							//in the missile's area
							if (playerBullets[b]->weaponType == WeaponType::PlayerMissile)
								e--;
							//if current weapon is bullet, skip enemies,
							//cause bullet can't touch more than one enemy
							else if (playerBullets[b]->weaponType == WeaponType::PlayerBullet)
								break;
						}
					}
				}
				//if current weapon is missle, instantiate an explosion
				if (playerBullets[b]->weaponType == WeaponType::PlayerMissile)
				{
					explosions.push_back(Explosion(gui.missileExplosion, playerBullets[b]->sprite.getPosition(),
						sounds.get(PlayerMissileExplosion)));
					explosions.back().play();
				}
				//delete player bullet(after collision)
				playerBullets.erase(playerBullets.begin() + b);
				gameSounds.bulletCollision.play();
				//set the bool variable to false
				collide = false;
				//end the loop
				break;
			}
		}


		//ENEMY BULLET AND PLAYER COLLISION
		if (!shiftMode)
		{
			//for each enemy bullet
			for (size_t b = 0; b < enemyBullets.size(); ++b)
			{
				//if enemy bullet intersects with player
				if (enemyBullets[b]->sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
				{
					//player take damage if it's uninvulnerable
					if (isInvulnerability == false)
					{
						int damage = enemyBullets[b]->getWeaponDamage();
						player.takeDamage(damage);
						gameSounds.playerTakeDamage.play();
						//if current player's hp is <= 0,
						//decrement lives, set the invulnerability to "on",
						//reset the player's hp
						if (player.getHp() <= 0)
						{
							--lives;
							gameSounds.playerLostLife.play();
							isInvulnerability = true;
							gui.blurringDuration = 0.f;
							gui.blurringClock.restart();
							player.setHp(player.getHpMax());
						}
					}
					//delete enemy bullet after collision
					enemyBullets.erase(enemyBullets.begin() + b);
					break;
				}
			}
		}

		//decides to end the loop
		bool toBreak = false;

		//ENEMY BULLET AND PLAYER BULLET COLLISION
		//for each player bullet
		for (size_t pb = 0; pb < playerBullets.size(); ++pb)
		{
			if (toBreak)
			{
				toBreak = false;
				break;
			}

			//for each enemy bullet
			for (size_t eb = 0; eb < enemyBullets.size(); ++eb)
			{
				//player missile can go through enemy bullets
				if (playerBullets[pb]->weaponType == WeaponType::PlayerMissile)
					break;

				if (enemyBullets[eb]->weaponType == WeaponType::ZiraelBullet)
					break;

				//if player bullet collides with enemy bullet, delete these bullets,
				//increase the score
				if (playerBullets[pb]->sprite.getGlobalBounds().intersects(enemyBullets[eb]->sprite.getGlobalBounds()))
				{
					playerBullets.erase(playerBullets.begin() + pb);
					enemyBullets.erase(enemyBullets.begin() + eb);
					gameSounds.bulletCollision.play();
					score += std::rand() % 6 + 1; // 1-6
					toBreak = true;
					break;
				}
			}
		}

		//ENEMY AND PLAYER COLLISION
		//for each enemy
		for (size_t e = 0; e < enemies.size(); ++e)
		{
			//if enemy ship intersects with player
			if (enemies[e]->sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
			{
				//if player isn't invulnerable,
				//decrease lives, take damage,
				//set the invulnerability to "on"
				//delete enemy, instantiate explosion,
				//if player is dead
				if (isInvulnerability == false)
				{
					if (!shiftMode)
					{
						if (enemies[e]->enemyType == EnemyType::Zirael
							|| enemies[e]->enemyType == EnemyType::Vattgern)
						{
							lives -= 10;
							player.takeDamage(player.getHpMax());
							gameSounds.playerLostLife.play();
							explosions.push_back(Explosion(gui.playerExplosion, player.getPosition(),
								sounds.get(EnemyExplosionSound)));
							explosions.back().play();
							currentState = State::GAMEOVER;
						}
						else
						{
							--lives;
							if (lives < 1)
							{
								explosions.push_back(Explosion(gui.playerExplosion, player.getPosition(),
									sounds.get(PlayerExplosionSound)));
								explosions.back().play();
							}
							player.takeDamage(player.getHpMax());
							gameSounds.playerLostLife.play();
							isInvulnerability = true;
							gui.blurringDuration = 0.f;
							gui.blurringClock.restart();
							player.setHp(player.getHpMax());
						}
					}
				}
				if (enemies[e]->enemyType != EnemyType::Zirael
					|| enemies[e]->enemyType != EnemyType::Vattgern)
				{
					explosions.push_back(Explosion(gui.enemyExplosion, enemies[e]->sprite.getPosition(),
						sounds.get(EnemyExplosionSound)));
					explosions.back().play();
					enemies.erase(enemies.begin() + e);
					enemiesRemaining--;
				}
			}
		}
	}
}

void Game::checkWindowCollision()
{
	if (currentState != State::PAUSE)
	{
		//for each player bullet
		for (size_t i = 0; i < playerBullets.size(); ++i)
		{
			//if player bullet go over the screen, delete it
			if (playerBullets[i]->sprite.getPosition().y <= -20.f)
			{
				playerBullets.erase(playerBullets.begin() + i);
				break;
			}
		}

		float windowPos = window->getSize().y + 40.f;
		//for each enemy
		//destroy enemy ship if it reached the end of the screen
		for (size_t i = 0; i < enemies.size(); ++i)
		{
			if (enemies[i]->sprite.getPosition().y >= windowPos)
			{
				enemies.erase(enemies.begin() + i);
				enemiesRemaining--;
				break;
			}
		}

		//for each enemy bullet
		//destroy enemy bullet if it reached the end of the screen
		for (size_t i = 0; i < enemyBullets.size(); ++i)
		{
			if (enemyBullets[i]->sprite.getPosition().y >= windowPos)
			{
				enemyBullets.erase(enemyBullets.begin() + i);
				break;
			}
		}
	}
}

//update enemy and player bullets
void Game::updateBullets()
{
	if (currentState == State::GAME
		|| currentState == State::SHOP
		|| currentState == State::GAMEOVER
		|| currentState == State::END)
	{
		for (size_t i = 0; i < playerBullets.size(); ++i)
			playerBullets[i]->update();

		for (size_t i = 0; i < enemyBullets.size(); ++i)
			enemyBullets[i]->update();
	}
}

void Game::endGame()
{
	currentState = State::END;
	isEnd = true;
	gui.endScoreText.setString("Your score: " + std::to_string(score));
	gui.endKillsText.setString("Kills: " + std::to_string(enemyKilled));
	gui.endScoreText.setOrigin(gui.endScoreText.getGlobalBounds().width / 2.f,
		gui.endScoreText.getGlobalBounds().height / 2.f);
	gui.endScoreText.setPosition(gui.endText.getPosition().x,
		gui.endText.getPosition().y + gui.endText.getGlobalBounds().height / 2.f + 30.f);
	gui.endKillsText.setOrigin(gui.endKillsText.getGlobalBounds().width / 2.f,
		gui.endKillsText.getGlobalBounds().height / 2.f);
	gui.endKillsText.setPosition(gui.endScoreText.getPosition().x,
		gui.endScoreText.getPosition().y + gui.endScoreText.getGlobalBounds().height / 2.f + 30.f);
	gui.restartText.setPosition(gui.endKillsText.getPosition().x,
		gui.endKillsText.getPosition().y + gui.endKillsText.getGlobalBounds().height + 45.f);
}

void Game::updateGUI()
{
	if (currentState != State::MAINMENU && currentState != State::PAUSE)
	{
		//update gui info
		gui.scoreText.setString("Score: " + std::to_string(score));
		gui.enemyText.setString("Kills: " + std::to_string(enemyKilled));
		gui.hpText.setString("Hp: " + std::to_string(player.getHp())
			+ "/" + std::to_string(player.getHpMax()));
		gui.liveText.setString("Lives: " + std::to_string(lives));
		gui.moneyText.setString("Money: " + std::to_string(money));
		gui.killToHealText.setString("Heal after "
			+ std::to_string(currentKillToHeal) + " kills");
		gui.waveText.setString("Wave #" + std::to_string(waveCount));
		gui.enemiesRemainingText.setString(std::to_string(enemiesRemaining) + " enemies remaining");

		//update gui game timer
		if (!prepareState && currentState != State::GAMEOVER && currentState != State::END)
		{
			gui.seconds = gui.gameTimer.getElapsedTime().asSeconds();
			if (gui.seconds >= 60)
			{
				gui.seconds -= 60;
				gui.minutes++;
				gui.gameTimer.restart();
			}
			gui.gameDuration.setString("Time:    " + std::to_string(gui.minutes)
				+ ":" + std::to_string(gui.seconds));
		}

		//if player has been healed, move text
		if (isHealed)
		{
			gui.healedText.move(0.f, -0.8);
			gui.healedText.setFillColor(sf::Color(255, 255, 255, gui.healTextDisappearance -= 2));
		}

		//move the next wave text
		if (showNextWaveText)
		{
			if (gui.nextWaveTextClock.getElapsedTime() <= sf::seconds(1.f))
			{
				gui.nextWaveText.move(0.f, -0.3);
				gui.nextWaveText.setFillColor(sf::Color(255, 255, 255, gui.waveTextAppearance += 4));
				if (gui.waveTextAppearance >= 255)
					gui.nextWaveText.setFillColor(sf::Color::White);
			}
			else
				gui.waveTextAppearance = 0;

			if (gui.nextWaveTextClock.getElapsedTime() >= sf::seconds(6.f) &&
				gui.nextWaveTextClock.getElapsedTime() <= gui.nextWaveTextDuration)
			{
				gui.nextWaveText.move(0.f, -0.3);
				gui.nextWaveText.setFillColor(sf::Color(255, 255, 255, gui.waveTextDisappearance -= 4));
				if (gui.waveTextDisappearance <= 0)
					gui.nextWaveText.setFillColor(sf::Color(255, 255, 255, 0));
			}
			else
				gui.waveTextDisappearance = 255;
		}

		//move start wave text
		if (showSpawnWaveText)
		{
			if (gui.nextWaveStartClock.getElapsedTime() <= sf::seconds(1.f))
			{
				gui.nextWaveStartText.move(0.f, -0.3);
				gui.nextWaveStartText.setFillColor(sf::Color(255, 255, 255, gui.startWaveTextAppearance += 4));
				if (gui.startWaveTextAppearance >= 255)
					gui.nextWaveStartText.setFillColor(sf::Color::White);
			}
			else
				gui.startWaveTextAppearance = 0;

			if (gui.nextWaveStartClock.getElapsedTime() >= sf::seconds(2.f) &&
				gui.nextWaveStartClock.getElapsedTime() <= gui.nextWaveStartTextDuration)
			{
				gui.nextWaveStartText.move(0.f, -0.3);
				gui.nextWaveStartText.setFillColor(sf::Color(255, 255, 255, gui.startWaveTextDisappearance -= 4));
				if (gui.startWaveTextDisappearance <= 0)
					gui.nextWaveStartText.setFillColor(sf::Color(255, 255, 255, 0));
			}
			else
				gui.startWaveTextDisappearance = 255;
		}

		//move prepare text
		if (prepareState)
		{
			gui.prepareTimeLeft =
				static_cast<int>(gui.prepareDuration.asSeconds() - gui.prepareTimer.getElapsedTime().asSeconds());
			gui.prepareTimeText.setString(
				"Prepare for Battle! Game starts at " + std::to_string(gui.prepareTimeLeft) + "...");
			if (gui.prepareTimer.getElapsedTime() <= sf::seconds(1.f))
			{
				gui.prepareTimeText.move(0.f, -0.3);
				gui.prepareTimeText.setFillColor(sf::Color(255, 255, 255, gui.prepareTextAppearance += 4));
				if (gui.prepareTextAppearance >= 255)
					gui.prepareTimeText.setFillColor(sf::Color::White);
			}
			else
				gui.prepareTextAppearance = 0;

			if (gui.prepareTimer.getElapsedTime() >= sf::seconds(9.f) &&
				gui.prepareTimer.getElapsedTime() <= gui.prepareDuration)
			{
				gui.prepareTimeText.move(0.f, -0.3);
				gui.prepareTimeText.setFillColor(sf::Color(255, 255, 255, gui.prepareTextDisappearance -= 4));
				if (gui.prepareTextDisappearance <= 0)
					gui.prepareTimeText.setFillColor(sf::Color(255, 255, 255, 0));
			}
			else
				gui.prepareTextDisappearance = 255;
		}
	}
}

void Game::renderGUI()
{
	//draw gui stuff
	window->draw(gui.statsBar);
	window->draw(gui.rightSideBar);
	window->draw(gui.scoreText);
	window->draw(gui.enemyText);
	window->draw(gui.killToHealText);
	window->draw(gui.hpSprite);
	window->draw(gui.hpText);
	window->draw(gui.liveSprite);
	window->draw(gui.liveText);
	window->draw(gui.moneySprite);
	window->draw(gui.moneyText);
	window->draw(gui.waveText);
	window->draw(gui.enemiesRemainingText);
	window->draw(gui.gameDuration);

	//draw game over stuff if the game ended
	if (currentState == State::GAMEOVER)
	{
		window->draw(gui.gameOverText);
		window->draw(gui.restartText);
	}

	//draw the end text
	if (currentState == State::END)
	{
		window->draw(gui.endText);
		window->draw(gui.endScoreText);
		window->draw(gui.endKillsText);
		window->draw(gui.restartText);
	}

	//draw text if player has been healed
	if (isHealed)
	{
		if (gui.killToHealTimer.getElapsedTime() <= gui.killToHealVisibleTime)
			window->draw(gui.healedText);
		else
		{
			isHealed = false;
			gui.healedText.setFillColor(sf::Color::White);
			gui.healedText.setPosition(window->getSize().x / 2.f,
				window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);
			gui.healTextDisappearance = 255;
		}
	}

	//draw the next wave text
	if (showNextWaveText)
	{
		if (gui.nextWaveTextClock.getElapsedTime() <= gui.nextWaveTextDuration)
			window->draw(gui.nextWaveText);
		else
		{
			showNextWaveText = false;
			gui.nextWaveText.setPosition(window->getSize().x / 2.f,
				window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);
			gui.waveTextAppearance = 0;
			gui.waveTextDisappearance = 255;
		}
	}

	//draw start wave text
	if (showSpawnWaveText)
	{
		if (gui.nextWaveStartClock.getElapsedTime() <= gui.nextWaveStartTextDuration)
			window->draw(gui.nextWaveStartText);
		else
		{
			showSpawnWaveText = false;
			gui.nextWaveStartText.setPosition(window->getSize().x / 2.f,
				window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);
			gui.startWaveTextAppearance = 0;
			gui.startWaveTextDisappearance = 255;
		}
	}

	//draw prepare text
	if (prepareState)
		window->draw(gui.prepareTimeText);
}

void Game::updateExplosions()
{
	if (currentState != State::PAUSE && currentState != State::MAINMENU)
	{
		for (size_t e = 0; e < explosions.size(); ++e)
		{
			explosions[e].update();
			if (explosions[e].life == false)
				explosions.erase(explosions.begin() + e);
		}
	}
}

void Game::renderExplosions()
{
	if (currentState != State::MAINMENU)
	{
		for (size_t e = 0; e < explosions.size(); ++e)
			window->draw(explosions[e].anim.sprite);
	}
}

void Game::renderBullets()
{
	if (currentState != State::MAINMENU)
	{
		for (size_t i = 0; i < playerBullets.size(); ++i)
			playerBullets[i]->render(*window);

		for (size_t i = 0; i < enemyBullets.size(); ++i)
			window->draw(enemyBullets[i]->sprite);
	}
}

void Game::updateShop(sf::Event & e)
{
	shop->update(sf::Mouse::getPosition(*window), money, e, currentState);
}

void Game::updatePauseMenu(sf::Event & e)
{
	pause->update(sf::Mouse::getPosition(*window), e,
		*window, currentState, &mainMenu->mainMenuMusic, &gameMusic);
}

void Game::updateMainMenu(sf::Event & e)
{
	if (currentState == State::MAINMENU)
	{
		//if the play button is pressed, start the new game
		if (mainMenu->update(sf::Mouse::getPosition(*window), e, *window, currentState, &gameMusic))
		{
			mainMenu->newGame = false;
			restart();
			mainMenu->mainMenuMusic.stop();
		}
	}
}

void Game::updateNextWaveText()
{
	//zirael wave text
	if (waveCount == 10)
	{
		gui.nextWaveText.setString("ZIRAEL WAVE!");
		gui.nextWaveText.setOrigin(gui.nextWaveText.getGlobalBounds().width / 2.f,
			gui.nextWaveText.getGlobalBounds().height / 2.f);
		gui.nextWaveText.setPosition(window->getSize().x / 2.f,
			window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);
		gameSounds.ziraelWaveSound.play();
	}
	//vattgern wave text
	else if (waveCount == 20)
	{
		gui.nextWaveText.setString("VATTGERN WAVE!");
		gui.nextWaveText.setOrigin(gui.nextWaveText.getGlobalBounds().width / 2.f,
			gui.nextWaveText.getGlobalBounds().height / 2.f);
		gui.nextWaveText.setPosition(window->getSize().x / 2.f,
			window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);
		gameSounds.vattgernWaveSound.play();
	}
	//common wave text
	else
	{
		gui.nextWaveStartText.setOrigin(gui.nextWaveStartText.getGlobalBounds().width / 2.f,
			gui.nextWaveStartText.getGlobalBounds().height / 2.f);
		gui.nextWaveText.setString("WAVE " + std::to_string(waveCount));
		gui.nextWaveText.setPosition(window->getSize().x / 2.f,
			window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);
		gameSounds.nextWaveSound.play();
	}
}

void Game::updatePrepareState()
{
	if (currentState == State::GAME)
	{
		//if prepare time is over, start the game
		if (gui.prepareTimer.getElapsedTime() >= gui.prepareDuration && prepareState == true)
		{
			prepareState = false;
			gui.gameTimer.restart();
			showSpawnWaveText = true;
			gui.nextWaveStartClock.restart();
			gameSounds.startWaveSound.play();
		}
	}
}

void Game::updateCursor()
{
	//show or do not show cursor
	switch (currentState)
	{
	case State::GAME:
		window->setMouseCursorVisible(false);
		break;
	case State::GAMEOVER:
		window->setMouseCursorVisible(false);
		break;
	case State::END:
		window->setMouseCursorVisible(false);
		break;
	case State::MAINMENU:
		window->setMouseCursorVisible(true);
		break;
	case State::PAUSE:
		window->setMouseCursorVisible(true);
		break;
	case State::SHOP:
		window->setMouseCursorVisible(true);
		break;
	}
}

void Game::updateBackground()
{
	//move the backgrounds and repeat it
	if (currentState == State::GAME || currentState == State::SHOP
		|| currentState == State::GAMEOVER || currentState == State::END)
	{
		gui.background_1.move(0.f, 0.7f);
		gui.background_2.move(0.f, 0.7f);

		if (gui.background_1.getGlobalBounds().top > window->getSize().y)
		{
			gui.background_1.setPosition(0.f, -gui.background_1.getGlobalBounds().height);
		}
		else if ((gui.background_2.getGlobalBounds().top > window->getSize().y))
		{
			gui.background_2.setPosition(0.f, -gui.background_2.getGlobalBounds().height);
		}
	}
}

void Game::renderBackground()
{
	if (currentState != State::MAINMENU)
	{
		window->draw(gui.background_1);
		window->draw(gui.background_2);
	}
}

void Game::renderShop()
{
	shop->render(*window, currentState);
}

void Game::renderPauseMenu()
{
	pause->render(*window, currentState);
}

void Game::renderMainMenu()
{
	mainMenu->render(*window, currentState);
}

void Game::restart()
{
	//restart the game
	clearScene();
	currentState = State::GAME;
	initVariables();
	mainMenu->mainMenuMusic.stop();
}

void Game::clearScene()
{
	//clear all entities in the game scene
	playerBullets.clear();
	enemies.clear();
	enemyBullets.clear();
	explosions.clear();
}

void Game::loadResources()
{
	//load textures
	textures.load(MainMenuBackground, "Assets/Textures/main_menu_background.png");
	textures.load(Background, "Assets/Textures/background.jpg");
	textures.load(EnemyBulletEagle, "Assets/Textures/enemy_bullet_eagle.png");
	textures.load(EnemyBulletRaptor, "Assets/Textures/enemy_bullet_raptor.png");
	textures.load(EnemyEagle, "Assets/Textures/enemy_eagle.png");
	textures.load(EnemyRaptor, "Assets/Textures/enemy_raptor.png");
	textures.load(EnemyZirael, "Assets/Textures/zirael.png");
	textures.load(ZiraelBullet, "Assets/Textures/zirael_bullet.png");
	textures.load(ExplosionEnemy, "Assets/Textures/Explosions/enemy_explosion.png");
	textures.load(ExplosionPlayer, "Assets/Textures/Explosions/player_explosion.png");
	textures.load(Hp, "Assets/Textures/player_heart.png");
	textures.load(PlayerShip, "Assets/Textures/player_ship.png");
	textures.load(Coin, "Assets/Textures/coin.png");
	textures.load(StatsBar, "Assets/Textures/stats_bar.png");
	textures.load(EnemyParticle, "Assets/Particles/enemy_particle.png");
	textures.load(PlayerMissileBoom, "Assets/Textures/Explosions/player_missile_explosion.png");
	textures.load(ShopBackground, "Assets/Textures/shop_background.png");
	textures.load(EnemyVattgern, "Assets/Textures/vattgern.png");
	textures.load(VattgernBullet, "Assets/Textures/vattgern_bullet.png");
	textures.load(MainMenuCloud_1, "Assets/Textures/clouds_1.png");
	textures.load(MainMenuCloud_2, "Assets/Textures/clouds_2.png");
	textures.load(MainMenuCloud_3, "Assets/Textures/clouds_3.png");
	textures.load(MainMenuPixelShip, "Assets/Textures/pixel_space_ship.png");

	//load fonts
	fonts.load(Arial, "Assets/Fonts/arial.ttf");

	//load sounds
	sounds.load(PlayerExplosionSound, "Assets/Sounds/player_explosion.wav");
	sounds.load(EnemyExplosionSound, "Assets/Sounds/enemy_explosion.wav");
	sounds.load(PlayerMissileExplosion, "Assets/Sounds/missile_explosion.wav");
	sounds.load(ZiraelDeathSound, "Assets/Sounds/zirael_death.wav");
	sounds.load(VattgernDeathSound, "Assets/Sounds/vattgern_death.wav");
	sounds.load(BulletCollision, "Assets/Sounds/bullet_collision.wav");
	sounds.load(EnemyBulletEagleSound, "Assets/Sounds/enemy_eagle_fire.wav");
	sounds.load(EnemyBulletRaptorSound, "Assets/Sounds/enemy_raptor_fire.wav");
	sounds.load(ZiraelBulletSound, "Assets/Sounds/zirael_fire.wav");
	sounds.load(PlayerTakeDamage, "Assets/Sounds/player_take_damage.wav");
	sounds.load(PlayerLostLife, "Assets/Sounds/player_lost_life.wav");
	sounds.load(VattgernBulletSound, "Assets/Sounds/vattgern_fire.wav");
	sounds.load(VattgernShiftSound, "Assets/Sounds/vattgern_shift.wav");
	sounds.load(ButtonSound, "Assets/Sounds/button_clicked.wav");
	sounds.load(NextWaveSound, "Assets/Sounds/next_wave.wav");
	sounds.load(StartWaveSound, "Assets/Sounds/start_wave.wav");
	sounds.load(ZiraelWaveSound, "Assets/Sounds/zirael_wave.wav");
	sounds.load(VattgernWaveSound, "Assets/Sounds/vattgern_wave.wav");

	//init sounds
	gameSounds.ziraelDeath.setBuffer(sounds.get(ZiraelDeathSound));
	gameSounds.vattgernDeath.setBuffer(sounds.get(VattgernDeathSound));
	gameSounds.bulletCollision.setBuffer(sounds.get(BulletCollision));
	gameSounds.playerLostLife.setBuffer(sounds.get(PlayerLostLife));
	gameSounds.playerTakeDamage.setBuffer(sounds.get(PlayerTakeDamage));
	gameSounds.nextWaveSound.setBuffer(sounds.get(NextWaveSound));
	gameSounds.startWaveSound.setBuffer(sounds.get(StartWaveSound));
	gameSounds.ziraelWaveSound.setBuffer(sounds.get(ZiraelWaveSound));
	gameSounds.vattgernWaveSound.setBuffer(sounds.get(VattgernWaveSound));

	//set the sound volume
	gameSounds.ziraelDeath.setVolume(30.f);
	gameSounds.vattgernDeath.setVolume(30.f);
	gameSounds.bulletCollision.setVolume(30.f);
	gameSounds.playerTakeDamage.setVolume(30.f);
	gameSounds.playerLostLife.setVolume(30.f);
	gameSounds.ziraelWaveSound.setVolume(15.f);
	gameSounds.vattgernWaveSound.setVolume(15.f);

	//load animations
	gui.enemyExplosion = Animation(textures.get(ExplosionEnemy), 0, 0, 192, 195, 25, 0.5);
	gui.enemyExplosion.sprite.setScale(0.7f, 0.7f);
	gui.playerExplosion = Animation(textures.get(ExplosionPlayer), 0, 0, 192, 192, 64, 0.5);
	gui.missileExplosion = Animation(textures.get(PlayerMissileBoom), 0, 0, 256, 256, 48, 0.5);
	gui.missileExplosion.sprite.setScale(1.5f, 1.5f);

	//load music
	gameMusic.openFromFile("Assets/Music/game_music.ogg");
	gameMusic.setLoop(true);
	gameMusic.setVolume(50.f);
}

void Game::initGUI()
{
	//load font
	sf::Font& arial = fonts.get(Arial);

	//set text character size and font
	gui.scoreText = gui.createText(20, arial);
	gui.enemyText = gui.createText(20, arial);
	gui.hpText = gui.createText(20, arial);
	gui.liveText = gui.createText(20, arial);
	gui.moneyText = gui.createText(20, arial);
	gui.gameOverText = gui.createText(35, arial);
	gui.killToHealText = gui.createText(20, arial);
	gui.healedText = gui.createText(35, arial);
	gui.restartText = gui.createText(30, arial);
	gui.waveText = gui.createText(20, arial);
	gui.nextWaveText = gui.createText(45, arial);
	gui.enemiesRemainingText = gui.createText(20, arial);
	gui.nextWaveStartText = gui.createText(45, arial);
	gui.gameDuration = gui.createText(20, arial);
	gui.prepareTimeText = gui.createText(30, arial);
	gui.endText = gui.createText(35, arial);
	gui.endScoreText = gui.createText(25, arial);
	gui.endKillsText = gui.createText(25, arial);

	gui.restartText.setFillColor(sf::Color::Red);

	//set sprite textures
	gui.hpSprite.setTexture(textures.get(Hp));
	gui.hpSprite.setScale(0.7f, 0.7f);
	gui.liveSprite.setTexture(textures.get(PlayerShip));
	gui.liveSprite.setScale(0.15f, 0.15f);
	gui.moneySprite.setTexture(textures.get(Coin));
	gui.moneySprite.setScale(0.7f, 0.7f);
	gui.statsBar.setTexture(textures.get(StatsBar));
	gui.statsBar.setScale(4.f, 3.2f);
	gui.rightSideBar = gui.statsBar;
	gui.rightSideBar.setPosition(window->getSize().x
		- gui.rightSideBar.getGlobalBounds().width, 0.f);
	gui.rightSideBar.setScale(4.f, 3.1f);

	//set text strings
	gui.scoreText.setString("Score: " + std::to_string(score));
	gui.enemyText.setString("Kills: " + std::to_string(enemyKilled));
	gui.hpText.setString("Hp: " + std::to_string(player.getHp()) + "/" + std::to_string(player.getHpMax()));
	gui.liveText.setString("Lives: " + std::to_string(lives));
	gui.moneyText.setString("Money: " + std::to_string(money));
	gui.gameOverText.setString("Game Over!");
	gui.killToHealText.setString("Heal after " + std::to_string(killToHealMax) + "kills");
	gui.healedText.setString("You have been healed!");
	gui.restartText.setString("Press R to restart");
	gui.nextWaveText.setString("WAVE " + std::to_string(waveCount));
	gui.nextWaveStartText.setString("THE WAVE IS COMING!");
	gui.waveText.setString("Wave #" + std::to_string(waveCount));
	gui.enemiesRemainingText.setString(std::to_string(enemiesRemaining) + " enemies remaining");
	gui.gameDuration.setString("Time:    " + std::to_string(gui.minutes)
		+ ":" + std::to_string(gui.seconds));
	gui.prepareTimeText.setString("Prepare for Battle! Game starts at " + gui.prepareTimeLeft);
	gui.endText.setString("You Win!");
	gui.endScoreText.setString("Your score: 0");
	gui.endKillsText.setString("Kills: 0");

	//init texts
	gui.enemyText.setPosition(gui.rightSideBar.getPosition().x + 20.f, 15.f);

	gui.killToHealText.setPosition(gui.enemyText.getPosition().x,
		gui.enemyText.getPosition().y + gui.killToHealText.getGlobalBounds().height + 20.f);

	gui.waveText.setPosition(gui.enemyText.getPosition().x,
		gui.killToHealText.getPosition().y + gui.waveText.getGlobalBounds().height + 20.f);

	gui.enemiesRemainingText.setPosition(gui.waveText.getPosition().x,
		gui.waveText.getPosition().y + gui.enemiesRemainingText.getGlobalBounds().height + 20.f);

	gui.gameDuration.setPosition(gui.enemiesRemainingText.getPosition().x,
		gui.enemiesRemainingText.getPosition().y + gui.gameDuration.getGlobalBounds().height + 20.f);

	gui.scoreText.setPosition(20.f, 15.f);

	gui.hpSprite.setPosition(15.f, gui.scoreText.getPosition().y + gui.scoreText.getGlobalBounds().height + 20.f);
	gui.hpText.setPosition(gui.hpSprite.getPosition().x + gui.hpSprite.getGlobalBounds().width + 5.f,
		gui.hpSprite.getPosition().y + gui.hpText.getGlobalBounds().height / 2.f);

	gui.liveSprite.setPosition(15.f, gui.hpText.getPosition().y + gui.hpText.getGlobalBounds().height + 20.f);
	gui.liveText.setPosition(gui.liveSprite.getPosition().x + gui.liveSprite.getGlobalBounds().width + 5.f,
		gui.liveSprite.getPosition().y + gui.liveSprite.getGlobalBounds().height
		/ 2.f - gui.liveText.getGlobalBounds().height);

	gui.moneySprite.setPosition(15.f, gui.liveText.getPosition().y + gui.liveText.getGlobalBounds().height + 25.f);
	gui.moneyText.setPosition(gui.moneySprite.getPosition().x + gui.moneySprite.getGlobalBounds().width + 5.f,
		gui.moneySprite.getPosition().y + gui.moneySprite.getGlobalBounds().height
		/ 2.f - gui.moneyText.getGlobalBounds().height / 2.f);

	gui.gameOverText.setOrigin(gui.gameOverText.getGlobalBounds().width / 2.f,
		gui.gameOverText.getGlobalBounds().height / 2.f);
	gui.gameOverText.setPosition(window->getSize().x / 2.f,
		window->getSize().y - (window->getSize().y - window->getSize().y / 3.f));

	gui.restartText.setOrigin(gui.restartText.getGlobalBounds().width / 2.f,
		gui.restartText.getGlobalBounds().height / 2.f);
	gui.restartText.setPosition(gui.gameOverText.getPosition().x,
		gui.gameOverText.getPosition().y + gui.gameOverText.getGlobalBounds().height + 15.f);

	gui.healedText.setOrigin(gui.healedText.getGlobalBounds().width / 2.f,
		gui.healedText.getGlobalBounds().height / 2.f);
	gui.healedText.setPosition(window->getSize().x / 2.f,
		window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);

	gui.nextWaveText.setOrigin(gui.nextWaveText.getGlobalBounds().width / 2.f,
		gui.nextWaveText.getGlobalBounds().height / 2.f);
	gui.nextWaveText.setPosition(window->getSize().x / 2.f,
		window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);

	gui.nextWaveStartText.setOrigin(gui.nextWaveStartText.getGlobalBounds().width / 2.f,
		gui.nextWaveStartText.getGlobalBounds().height / 2.f);
	gui.nextWaveStartText.setPosition(window->getSize().x / 2.f,
		window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);

	gui.prepareTimeText.setOrigin(gui.prepareTimeText.getGlobalBounds().width / 2.f,
		gui.prepareTimeText.getGlobalBounds().height / 2.f);
	gui.prepareTimeText.setPosition(window->getSize().x / 2.f,
		window->getSize().y - (window->getSize().y - window->getSize().y / 3.f) - 150.f);

	gui.endText.setOrigin(gui.endText.getGlobalBounds().width / 2.f,
		gui.endText.getGlobalBounds().height / 2.f);
	gui.endText.setPosition(window->getSize().x / 2.f,
		window->getSize().y - (window->getSize().y - window->getSize().y / 3.f));

	gui.endScoreText.setOrigin(gui.endScoreText.getGlobalBounds().width / 2.f,
		gui.endScoreText.getGlobalBounds().height / 2.f);
	gui.endScoreText.setPosition(gui.endText.getPosition().x,
		gui.endText.getPosition().y + gui.endText.getGlobalBounds().height / 2.f + 30.f);

	gui.endKillsText.setOrigin(gui.endKillsText.getGlobalBounds().width / 2.f,
		gui.endKillsText.getGlobalBounds().height / 2.f);
	gui.endKillsText.setPosition(gui.endScoreText.getPosition().x,
		gui.endScoreText.getPosition().y + gui.endScoreText.getGlobalBounds().height / 2.f + 30.f);
}

bool Game::isEnemyKill(Enemy & enemy)
{
	//true if enemy died
	bool died = false;

	//if enemy has been destroyed
	if (enemy.getHp() <= 0)
	{
		//instantiate an explotion
		explosions.push_back(Explosion(gui.enemyExplosion, enemy.sprite.getPosition(),
			sounds.get(EnemyExplosionSound)));
		explosions.back().play();
		money += enemy.killReward;           //set the money reward
		++enemyKilled;                       //increase kill count
		--currentKillToHeal;                 //decrease enemy count to heal

		//if it's time to heal, increase the player's hp,
		//reset the killToHeal count
		if (currentKillToHeal <= 0)
		{
			if (player.getHp() < player.getHpMax())
			{
				player.setHp(player.getHp() + 10);
				isHealed = true;
			}
			currentKillToHeal = killToHealMax;
			gui.killToHealTimer.restart();
		}
		if (enemy.enemyType == EnemyType::Zirael)
		{
			//instantiate explosions if zirael is dead

			explosions.push_back(Explosion(gui.missileExplosion,
				sf::Vector2f(enemy.sprite.getPosition().x - 50.f,
					enemy.sprite.getPosition().y + 40.f),
				sounds.get(EnemyExplosionSound)));

			explosions.push_back(Explosion(gui.missileExplosion,
				sf::Vector2f(enemy.sprite.getPosition().x + 40.f,
					enemy.sprite.getPosition().y - 30.f),
				sounds.get(EnemyExplosionSound)));

			explosions.push_back(Explosion(gui.missileExplosion,
				sf::Vector2f(enemy.sprite.getPosition().x,
					enemy.sprite.getPosition().y),
				sounds.get(EnemyExplosionSound)));

			gameSounds.ziraelDeath.play();

			score += 10000;
		}
		else if (enemy.enemyType == EnemyType::Vattgern)
		{
			//instantiate explosions if vattgern is dead

			explosions.push_back(Explosion(gui.missileExplosion,
				sf::Vector2f(enemy.sprite.getPosition().x - 50.f,
					enemy.sprite.getPosition().y + 40.f),
				sounds.get(EnemyExplosionSound)));

			explosions.push_back(Explosion(gui.missileExplosion,
				sf::Vector2f(enemy.sprite.getPosition().x + 40.f,
					enemy.sprite.getPosition().y - 30.f),
				sounds.get(EnemyExplosionSound)));

			explosions.push_back(Explosion(gui.missileExplosion,
				sf::Vector2f(enemy.sprite.getPosition().x,
					enemy.sprite.getPosition().y),
				sounds.get(EnemyExplosionSound)));

			gameSounds.vattgernDeath.play();

			score += 50000;
			endGame();           //end the game if vattgern is killed
		}
		//increase the score
		score += std::rand() % 15 + 35; //35 - 50
		//set the bool variable to true
		died = true;
	}
	//if enemy hasn't died, return false
	return died;
}