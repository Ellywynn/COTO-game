#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

enum ButtonState : unsigned short
{
	IDLE = 0,
	ACTIVE,
	PRESSED
};

enum ButtonType
{
	BTN_SHOP = 0,
	BTN_PAUSED
};

class Button
{
public:
	//menu button
	Button(float x, float y, float width, float height,
		sf::Font* buttonFont, const std::string& text,
		unsigned int charSize = 14u);

	//shop button
	Button(float x, float y, float width, float height,
		sf::Font* buttonFont, const std::string& text,
		sf::Texture* coinTexture, int price,
		unsigned int charSize = 14u);
	~Button();

	void updateMenuButton(const sf::Vector2i& mousePos, sf::Event& event);
	void updateShopButton(const sf::Vector2i& mousePos, sf::Event& event, int& money);
	void render(sf::RenderTarget& target);

	void updatePricePosition();

	const bool isButtonPressed() const;
	unsigned short buttonState;
	const sf::Vector2f& getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const int getPrice() const;

	void setTextColor(sf::Color color);
	void setPrice(int newPrice);
	void setPriceText(const std::string& text);
private:
	void updateButton(const sf::Vector2i & mousePos, sf::Event & event);
private:
	ButtonType type;

	sf::RectangleShape background;
	sf::Font* font;
	sf::Text buttonText;

	int price;
	sf::Sprite priceSprite;
	sf::Text priceText;

	sf::Color idleColor;
	sf::Color activeColor;
	sf::Color pressedColor;
};

