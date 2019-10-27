#include "GameShop.h"

GameShop::GameShop()
	:textures(nullptr), buttonFont(nullptr)
{
}

GameShop::GameShop(textureHolder* Textures, sf::Font* font, const sf::Vector2u& size,
	Player* pl, sf::SoundBuffer& sound)
	: textures(Textures), buttonFont(font), player(pl), windowSize(size)
{
	initGraphics();
	initTexts();
	initButtons();
	buttonSound.setBuffer(sound);
	buttonSound.setVolume(30.f);
}


GameShop::~GameShop()
{
	for (auto it = shopButtons.begin(); it != shopButtons.end(); ++it)
		delete it->second;
}

void GameShop::update(const sf::Vector2i& mousePos, int& money,
	sf::Event& event, const State& state)
{
	if (state == State::SHOP)
	{
		for (auto& i : shopButtons)
			i.second->updateShopButton(mousePos, event, money);

		updateButtons(money);

		if (money < 0)
			money = 0;
		std::string offset = "      ";
		currentMoneyText.setString(std::to_string(money));
		gui.currentStats.setString("Armor: Lvl. " + std::to_string(player->upgrades.armorLevel)
			+ offset + "Attack Speed:  Lvl. " + std::to_string(player->upgrades.bAttackSpeedLevel)
			+ offset + "Damage:  Lvl. " + std::to_string(player->upgrades.bulletDamageLevel)
			+ offset + "Hp:  Lvl. " + std::to_string(player->upgrades.hpLevel));
	}

	if (showMessage)
	{
		message.move(0.f, -0.5f);
		message.setFillColor(sf::Color(255, 255, 255, disappearance -= 2));
	}
}

void GameShop::render(sf::RenderTarget & target, const State& state)
{
	if (state == State::SHOP)
	{
		//target.draw(mask);
		target.draw(background);
		target.draw(moneySprite);
		target.draw(currentMoneyText);
		target.draw(gui.currentStats);
		target.draw(gui.armorText);
		target.draw(gui.attackSpeedText);
		target.draw(gui.damageText);
		target.draw(gui.healthText);

		for (auto& it : shopButtons)
			it.second->render(target);
	}

	if (showMessage)
	{
		if (messageClock.getElapsedTime() < messageDuration)
			target.draw(message);
		else
		{
			showMessage = false;
			message.setPosition(windowSize.x / 2.f, background.getGlobalBounds().top + 50.f);
			message.setFillColor(sf::Color::White);
			disappearance = 255;
		}
	}
}

void GameShop::initButtons()
{
	//ARMOR
	shopButtons["BuyArmor"] = new Button(background.getPosition().x + 80.f,
		gui.armorText.getPosition().y + 50.f, 250.f, 68.f, buttonFont, "+5% Damage Resistance",
		&(*textures).get(Coin), player->upgrades.armorPrices[1]);

	//ATTACK SPEED
	shopButtons["BuyBulletAS"] = new Button(
		shopButtons["BuyArmor"]->getPosition().x,
		gui.attackSpeedText.getPosition().y + 50.f, 250.f, 68.f, buttonFont, "Increase Bullet AS",
		&(*textures).get(Coin), player->upgrades.bAttackSpeedPrices[1]);

	shopButtons["BuyMissileAS"] = new Button(
		gui.attackSpeedText.getPosition().x - shopButtons["BuyBulletAS"]->getGlobalBounds().width / 2.f,
		shopButtons["BuyBulletAS"]->getPosition().y, 250.f, 68.f, buttonFont, "Increase Missile AS",
		&(*textures).get(Coin), player->upgrades.mAttackSpeedPrices[1]);

	//DAMAGE
	shopButtons["BuyBulletDamage"] = new Button(
		shopButtons["BuyBulletAS"]->getPosition().x,
		gui.damageText.getPosition().y + 50.f, 250.f, 68.f, buttonFont, "+1 Bullet Damage",
		&(*textures).get(Coin), player->upgrades.bulletDamagePrices[1]);

	shopButtons["BuyMissileDamage"] = new Button(
		gui.damageText.getPosition().x - shopButtons["BuyBulletAS"]->getGlobalBounds().width / 2.f,
		gui.damageText.getPosition().y + 50.f, 250.f, 68.f, buttonFont, "+5 Missile Damage",
		&(*textures).get(Coin), player->upgrades.missileDamagePrices[1]);

	//HEALTH
	shopButtons["IncHp"] = new Button(
		shopButtons["BuyBulletDamage"]->getPosition().x,
		gui.healthText.getPosition().y + 50.f, 250.f, 68.f, buttonFont, "+10 Max Hp",
		&(*textures).get(Coin), player->upgrades.hpPrices[1]);

	shopButtons["BuyHp"] = new Button(
		gui.healthText.getPosition().x - shopButtons["IncHp"]->getGlobalBounds().width / 2.f,
		gui.healthText.getPosition().y + 50.f, 250.f, 68.f, buttonFont, "Heal 10 HP",
		&(*textures).get(Coin), 5000);
}

void GameShop::initTexts()
{
	message.setFont(*buttonFont);
	message.setCharacterSize(29);
	message.setString("You haven't enough money!");
	message.setOrigin(message.getGlobalBounds().width / 2.f, message.getGlobalBounds().height / 2.f);
	message.setPosition(windowSize.x / 2.f, background.getGlobalBounds().top + 50.f);

	currentMoneyText.setFont(*buttonFont);
	currentMoneyText.setCharacterSize(18);
	currentMoneyText.setString("0");
	currentMoneyText.setPosition(moneySprite.getPosition().x + moneySprite.getGlobalBounds().width + 5.f,
		moneySprite.getPosition().y + currentMoneyText.getGlobalBounds().height);

	float xPos = background.getPosition().x + background.getGlobalBounds().width / 2.f;
	float yPox = background.getPosition().y;
	gui.armorText.setFont(*buttonFont);
	gui.armorText.setCharacterSize(16u);
	gui.armorText.setString("Armor");
	gui.armorText.setOrigin(gui.armorText.getGlobalBounds().width / 2.f,
		gui.armorText.getGlobalBounds().height / 2.f);
	gui.armorText.setPosition(xPos, yPox + 150.f);

	gui.attackSpeedText.setFont(*buttonFont);
	gui.attackSpeedText.setCharacterSize(16u);
	gui.attackSpeedText.setString("Attack Speed");
	gui.attackSpeedText.setOrigin(gui.attackSpeedText.getGlobalBounds().width / 2.f,
		gui.attackSpeedText.getGlobalBounds().height / 2.f);
	gui.attackSpeedText.setPosition(xPos,
		gui.armorText.getPosition().y + 155.f);

	gui.damageText.setFont(*buttonFont);
	gui.damageText.setCharacterSize(16u);
	gui.damageText.setString("Damage");
	gui.damageText.setOrigin(gui.damageText.getGlobalBounds().width / 2.f,
		gui.damageText.getGlobalBounds().height / 2.f);
	gui.damageText.setPosition(xPos,
		gui.attackSpeedText.getPosition().y + 155.f);

	gui.healthText.setFont(*buttonFont);
	gui.healthText.setCharacterSize(16u);
	gui.healthText.setString("Health");
	gui.healthText.setOrigin(gui.healthText.getGlobalBounds().width / 2.f,
		gui.healthText.getGlobalBounds().height / 2.f);
	gui.healthText.setPosition(xPos,
		gui.damageText.getPosition().y + 155.f);

	std::string offset = "      ";
	gui.currentStats.setFont(*buttonFont);
	gui.currentStats.setCharacterSize(16u);
	gui.currentStats.setString("Armor: " + std::to_string(player->upgrades.armorLevel)
		+ " Lvl. " + offset + "Attack Speed: " + std::to_string(player->upgrades.bAttackSpeedLevel)
		+ " Lvl. " + offset + "Damage: " + std::to_string(player->upgrades.bulletDamageLevel)
		+ " Lvl. " + offset + "Hp: " + std::to_string(player->upgrades.hpLevel)
		+ " Lvl.");
	gui.currentStats.setOrigin(gui.currentStats.getGlobalBounds().width / 2.f,
		gui.currentStats.getGlobalBounds().height / 2.f);
	gui.currentStats.setPosition(background.getPosition().x + background.getGlobalBounds().width / 2.f,
		currentMoneyText.getPosition().y - gui.currentStats.getGlobalBounds().height / 2.f + 30.f);

	showMessage = false;
	messageDuration = sf::seconds(2.f);
}

void GameShop::initGraphics()
{
	disappearance = 255;
	background.setTexture((*textures).get(ShopBackground));
	background.setPosition(windowSize.x / 2.f - background.getGlobalBounds().width / 2.f,
		windowSize.y / 2.f - background.getGlobalBounds().height / 2.f);

	mask.setSize(sf::Vector2f(background.getGlobalBounds().width, background.getGlobalBounds().height));
	mask.setFillColor(sf::Color(30, 30, 30, 150));
	mask.setPosition(background.getPosition());

	moneySprite.setTexture((*textures).get(Coin));
	moneySprite.setScale(0.6f, 0.6f);
	moneySprite.setPosition(background.getPosition().x + 70.f,
		background.getPosition().y + 90.f);
}

void GameShop::updateButtons(int& money)
{
	//ARMOR
	if (shopButtons["BuyArmor"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["BuyArmor"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->upgrades.armorLevel <= player->upgrades.armorLevelMax)
			{
				if (player->upgrades.armorLevel <= player->upgrades.armorLevelMax)
				{
					money -= shopButtons["BuyArmor"]->getPrice();
					player->setArmor(player->getArmor() + 0.05f);
					player->upgrades.armorLevel++;
				}
				if (player->upgrades.armorLevel > player->upgrades.armorLevelMax)
					shopButtons["BuyArmor"]->setPriceText("Max Level");
				else
					shopButtons["BuyArmor"]->setPrice(player->upgrades.armorPrices[
						player->upgrades.armorLevel + 1]);
				shopButtons["BuyArmor"]->updatePricePosition();
			}
		}
	}

	//HEALTH
	if (shopButtons["IncHp"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["IncHp"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->upgrades.hpLevel <= player->upgrades.hpLevelMax)
			{
				if (player->upgrades.hpLevel <= player->upgrades.hpLevelMax)
				{
					money -= shopButtons["IncHp"]->getPrice();
					player->setHpMax(player->getHpMax() + 10);
					player->upgrades.hpLevel++;
				}
				if (player->upgrades.hpLevel > player->upgrades.hpLevelMax)
					shopButtons["IncHp"]->setPriceText("Max Level");
				else
					shopButtons["IncHp"]->setPrice(player->upgrades.hpPrices[
						player->upgrades.hpLevel + 1]);
				shopButtons["IncHp"]->updatePricePosition();
			}
		}
	}

	if (shopButtons["BuyHp"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["BuyHp"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->getHp() < player->getHpMax())
			{
				money -= shopButtons["BuyHp"]->getPrice();
				player->setHp(player->getHp() + 10);
				shopButtons["BuyHp"]->updatePricePosition();
			}
		}
	}

	//DAMAGE
	if (shopButtons["BuyBulletDamage"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["BuyBulletDamage"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->upgrades.bulletDamageLevel <= player->upgrades.bulletDamageLevelMax)
			{
				if (player->upgrades.bulletDamageLevel <= player->upgrades.bulletDamageLevelMax)
				{
					money -= shopButtons["BuyBulletDamage"]->getPrice();
					player->playerBullet.setWeaponDamage(player->playerBullet.getWeaponDamage() + 1);
					player->upgrades.bulletDamageLevel++;
				}
				if (player->upgrades.bulletDamageLevel > player->upgrades.bulletDamageLevelMax)
					shopButtons["BuyBulletDamage"]->setPriceText("Max Level");
				else
					shopButtons["BuyBulletDamage"]->setPrice(player->upgrades.bulletDamagePrices[
						player->upgrades.bulletDamageLevel + 1]);
				shopButtons["BuyBulletDamage"]->updatePricePosition();
			}
		}
	}

	if (shopButtons["BuyMissileDamage"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["BuyMissileDamage"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->upgrades.missileDamageLevel <= player->upgrades.missileDamageLevelMax)
			{
				if (player->upgrades.missileDamageLevel <= player->upgrades.missileDamageLevelMax)
				{
					money -= shopButtons["BuyMissileDamage"]->getPrice();
					player->playerMissile.setWeaponDamage(player->playerMissile.getWeaponDamage() + 5);
					player->upgrades.missileDamageLevel++;
				}
				if (player->upgrades.missileDamageLevel > player->upgrades.missileDamageLevelMax)
					shopButtons["BuyMissileDamage"]->setPriceText("Max Level");
				else
					shopButtons["BuyMissileDamage"]->setPrice(player->upgrades.missileDamagePrices[
						player->upgrades.missileDamageLevel + 1]);
				shopButtons["BuyMissileDamage"]->updatePricePosition();
			}
		}
	}

	//ATTACK SPEED
	if (shopButtons["BuyBulletAS"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["BuyBulletAS"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->upgrades.bAttackSpeedLevel <= player->upgrades.bAttackSpeedLevelMax)
			{
				if (player->upgrades.bAttackSpeedLevel <= player->upgrades.bAttackSpeedLevelMax)
				{
					money -= shopButtons["BuyBulletAS"]->getPrice();
					player->setBulletAS(player->bulletAS - 0.008f);
					player->upgrades.bAttackSpeedLevel++;
				}
				if (player->upgrades.bAttackSpeedLevel > player->upgrades.bAttackSpeedLevelMax)
					shopButtons["BuyBulletAS"]->setPriceText("Max Level");
				else
					shopButtons["BuyBulletAS"]->setPrice(player->upgrades.bAttackSpeedPrices[
						player->upgrades.bAttackSpeedLevel + 1]);
				shopButtons["BuyBulletAS"]->updatePricePosition();
			}
		}
	}

	if (shopButtons["BuyMissileAS"]->isButtonPressed())
	{
		buttonSound.play();
		if ((money - shopButtons["BuyMissileAS"]->getPrice()) < 0)
		{
			if (showMessage == false)
			{
				showMessage = true;
				messageClock.restart();
			}
		}
		else
		{
			if (player->upgrades.mAttackSpeedLevel <= player->upgrades.mAttackSpeedLevelMax)
			{
				if (player->upgrades.mAttackSpeedLevel <= player->upgrades.mAttackSpeedLevelMax)
				{
					money -= shopButtons["BuyMissileAS"]->getPrice();
					player->setMissileAS(player->missileAS - 0.3f);
					player->upgrades.mAttackSpeedLevel++;
				}
				if (player->upgrades.mAttackSpeedLevel > player->upgrades.mAttackSpeedLevelMax)
					shopButtons["BuyMissileAS"]->setPriceText("Max Level");
				else
					shopButtons["BuyMissileAS"]->setPrice(player->upgrades.mAttackSpeedPrices[
						player->upgrades.mAttackSpeedLevel + 1]);
				shopButtons["BuyMissileAS"]->updatePricePosition();
			}
		}
	}
}
