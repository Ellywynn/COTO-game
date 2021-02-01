#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <memory>
#include <map>
#include <cassert>
#include <stdexcept>
#include <iostream>

#include "Animation.h"

//game textures
enum Textures
{
	Background = 0,

	EnemyEagle,
	EnemyRaptor,
	EnemyZirael,
	EnemyVattgern,
	PlayerShip,

	EnemyBulletEagle,
	EnemyBulletRaptor,
	PlayerBullet,
	PlayerMissile,
	PlayerMissileBoom,
	PlayerMissileFire,
	ZiraelBullet,
	VattgernBullet,

	ExplosionEnemy,
	ExplosionPlayer,

	PlayerParticle,
	EnemyParticle,

	StatsBar,
	Hp,
	Coin,

	ShopBackground,
	MainMenuBackground,
	MainMenuCloud_1,
	MainMenuCloud_2,
	MainMenuCloud_3,
	MainMenuPixelShip
};

//gui fonts
enum Fonts
{
	Arial = 0
};

//game sounds names
enum Sounds
{
	PlayerBulletSound = 0,
	PlayerMissileSound,
	PlayerShiftSound,
	EnemyBulletRaptorSound,
	EnemyBulletEagleSound,
	ZiraelBulletSound,
	VattgernBulletSound,
	VattgernShiftSound,

	EnemyExplosionSound,
	PlayerExplosionSound,
	ZiraelDeathSound,
	VattgernDeathSound,
	PlayerMissileExplosion,
	BulletCollision,

	PlayerTakeDamage,
	PlayerLostLife,

	ButtonSound,
	NextWaveSound,
	StartWaveSound,
	ZiraelWaveSound,
	VattgernWaveSound
};

//game GUI stuff
struct GUI
{
	sf::Text scoreText;
	sf::Text enemyText;
	sf::Text hpText;
	sf::Text liveText;
	sf::Text moneyText;
	sf::Text gameOverText;
	sf::Text killToHealText;
	sf::Text healedText;
	sf::Text restartText;
	sf::Text enemiesRemainingText;

	sf::Sprite hpSprite;
	sf::Sprite liveSprite;
	sf::Sprite moneySprite;
	sf::Sprite background_1;
	sf::Sprite background_2;
	sf::Sprite statsBar;
	sf::Sprite rightSideBar;

	Animation enemyExplosion;
	Animation playerExplosion;
	Animation missileExplosion;

	//time before wave spawn
	sf::Clock startClock;
	sf::Time getReadyTime;

	sf::Clock enemySpawnClock;
	sf::Time enemySpawnRate;

	sf::Clock killToHealTimer;
	sf::Time killToHealVisibleTime;
	int healTextDisappearance;

	sf::Clock blurringClock;
	sf::Time blurringTime;
	float blurringDuration;

	sf::Clock WaveClock;
	sf::Text waveText;

	sf::Clock enemyMicrowaveClock;
	sf::Time microwaveRate;

	sf::Clock nextWaveClock;
	sf::Time nextWavePrepareTime;
	sf::Clock nextWaveStartClock;
	sf::Text nextWaveStartText;
	sf::Time nextWaveStartTextDuration;
	int startWaveTextDisappearance;
	int startWaveTextAppearance;

	sf::Clock nextWaveTextClock;
	sf::Time nextWaveTextDuration;
	sf::Text nextWaveText;
	int waveTextDisappearance;
	int waveTextAppearance;

	sf::Text enemiesRemaining;

	sf::Clock gameTimer;
	sf::Text gameDuration;
	int minutes;
	int seconds;

	sf::Clock prepareTimer;
	sf::Time prepareDuration;
	sf::Text prepareTimeText;
	int prepareTextDisappearance;
	int prepareTextAppearance;
	int prepareTimeLeft;

	sf::Text endText;
	sf::Text endScoreText;
	sf::Text endKillsText;

	sf::Text createText(int charSize, sf::Font& font)
	{
		sf::Text newText;
		newText.setFont(font);
		newText.setCharacterSize(charSize);
		return newText;
	}
};

//game sounds
struct GameSounds
{
	sf::Sound playerTakeDamage;
	sf::Sound playerLostLife;
	sf::Sound ziraelDeath;
	sf::Sound bulletCollision;
	sf::Sound vattgernDeath;
	sf::Sound nextWaveSound;
	sf::Sound startWaveSound;
	sf::Sound ziraelWaveSound;
	sf::Sound vattgernWaveSound;
};

//templaet resource holder
template<typename Resource, typename ID>
class ResourceHolder
{
public:
	ResourceHolder() {}
	~ResourceHolder() {}

	//load resource into container
	inline void load(ID id, const std::string& filename)
	{
		//create resource pointer and load it from file
		std::unique_ptr<Resource> res(new Resource());
		if (!res->loadFromFile(filename))
			throw std::runtime_error("[ResourceHolder]: can't load " + filename);

		//insert resource into the resource holder
		auto inserted = resourses.insert(std::make_pair(id, std::move(res)));
		assert(inserted.second);
	}

	//get resource
	inline Resource& get(ID id)
	{
		//find resource
		auto found = resourses.find(id);
		//return resource if there's resource with argument ID
		assert(found != resourses.end());
		return *found->second;
	}

	//get resource
	inline const Resource& get(ID id) const
	{
		//find resource
		auto found = resourses.find(id);
		//return resource if there's resource with argument ID
		assert(found != resourses.end());
		return *found->second;
	}
private:
	//container for resources
	std::map<ID, std::unique_ptr<Resource>> resourses;
};

//holders
typedef ResourceHolder<sf::Texture, Textures> textureHolder;
typedef ResourceHolder<sf::Font, Fonts> fontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds> soundHolder;