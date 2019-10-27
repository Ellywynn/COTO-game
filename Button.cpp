#include "Button.h"


//Menu Button
Button::Button(float x, float y, float width, float height, sf::Font * buttonFont,
	const std::string & text, unsigned int charSize)
{
	type = ButtonType::BTN_PAUSED;

	buttonState = ButtonState::IDLE;

	background.setSize(sf::Vector2f(width, height));
	background.setPosition(sf::Vector2f(x, y));
	background.setOrigin(background.getSize().x / 2.f, background.getSize().y / 2.f);

	font = buttonFont;
	buttonText.setFillColor(sf::Color::White);
	buttonText.setFont(*font);
	buttonText.setCharacterSize(charSize);
	buttonText.setString(text);
	buttonText.setOrigin(buttonText.getGlobalBounds().width / 2.f,
		buttonText.getGlobalBounds().height / 2.f);
	buttonText.setPosition(background.getPosition().x,
		background.getPosition().y);

	idleColor = sf::Color(9, 33, 43, 100);
	activeColor = sf::Color(10, 51, 68, 70);
	pressedColor = sf::Color(6, 29, 39, 100);

	background.setFillColor(idleColor);
}

//Shop Button
Button::Button(float x, float y, float width, float height,
	sf::Font* buttonFont, const std::string& text,
	sf::Texture* coinTexture, int itemPrice,
	unsigned int charSize)
{
	type = ButtonType::BTN_SHOP;

	buttonState = ButtonState::IDLE;
	price = itemPrice;

	background.setSize(sf::Vector2f(width, height));
	background.setPosition(sf::Vector2f(x, y));

	font = buttonFont;
	buttonText.setFillColor(sf::Color::White);
	buttonText.setFont(*font);
	buttonText.setCharacterSize(charSize);
	buttonText.setString(text);
	buttonText.setOrigin(buttonText.getGlobalBounds().width / 2.f,
		buttonText.getGlobalBounds().height / 2.f);
	buttonText.setPosition(background.getPosition().x + background.getGlobalBounds().width / 2.f,
		background.getPosition().y + background.getGlobalBounds().height / 2.f);

	priceText.setFillColor(sf::Color::White);
	priceText.setFont(*font);
	priceText.setCharacterSize(14);
	priceText.setString(std::to_string(price));
	priceText.setPosition(x + background.getGlobalBounds().width - priceText.getGlobalBounds().width - 10.f,
		y + background.getGlobalBounds().height - priceText.getGlobalBounds().height - 10.f);

	priceSprite.setTexture(*coinTexture);
	priceSprite.setScale(0.3f, 0.3f);
	priceSprite.setPosition(priceText.getPosition().x - priceSprite.getGlobalBounds().width - 5.f,
		priceText.getPosition().y);

	idleColor = sf::Color(9, 33, 43, 120);
	activeColor = sf::Color(16, 51, 68, 70);
	pressedColor = sf::Color(6, 29, 39, 100);

	background.setFillColor(idleColor);
}


Button::~Button()
{
}

void Button::updateMenuButton(const sf::Vector2i & mousePos, sf::Event & event)
{
	updateButton(mousePos, event);
}

void Button::updateShopButton(const sf::Vector2i & mousePos, sf::Event& event, int& money)
{
	if (money < price)
		priceText.setFillColor(sf::Color::Red);
	else
		priceText.setFillColor(sf::Color::White);

	updateButton(mousePos, event);
}

void Button::render(sf::RenderTarget & target)
{
	target.draw(background);
	target.draw(buttonText);
	if (type == ButtonType::BTN_SHOP)
	{
		target.draw(priceSprite);
		target.draw(priceText);
	}
}

void Button::updatePricePosition()
{
	priceText.setPosition(background.getPosition().x + background.getGlobalBounds().width
		- priceText.getGlobalBounds().width - 20.f, background.getPosition().y
		+ background.getGlobalBounds().height - priceText.getGlobalBounds().height - 20.f);

	priceSprite.setPosition(priceText.getPosition().x - priceSprite.getGlobalBounds().width - 5.f,
		priceText.getPosition().y);
}

const bool Button::isButtonPressed() const
{
	return buttonState == ButtonState::PRESSED;
}

const sf::Vector2f & Button::getPosition() const
{
	return background.getPosition();
}

const sf::FloatRect Button::getGlobalBounds() const
{
	return background.getGlobalBounds();
}

void Button::setTextColor(sf::Color color)
{
	priceText.setFillColor(color);
}

void Button::setPrice(int newPrice)
{
	price = newPrice;
	priceText.setString(std::to_string(price));
}

void Button::setPriceText(const std::string & text)
{
	priceText.setString(text);
}

void Button::updateButton(const sf::Vector2i & mousePos, sf::Event & event)
{
	//idle by default
	buttonState = ButtonState::IDLE;
	//if the mouse within the button bounds
	if (background.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
	{
		//set it to the active state
		buttonState = ButtonState::ACTIVE;
		//if button has been clicked
		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
		{
			//set it to the pressed state
			buttonState = ButtonState::PRESSED;
		}
	}

	switch (buttonState)
	{
	case ButtonState::IDLE:
		background.setFillColor(idleColor);
		//background.setOutlineColor(sf::Color(40, 40, 40, 100));
		background.setOutlineColor(sf::Color(9, 33, 43, 180));
		background.setOutlineThickness(5.f);
		break;
	case ButtonState::ACTIVE:
		background.setFillColor(activeColor);
		//background.setOutlineColor(sf::Color(40, 40, 40, 150));
		background.setOutlineColor(sf::Color(9, 33, 43, 150));
		background.setOutlineThickness(5.f);
		break;
	case ButtonState::PRESSED:
		background.setFillColor(pressedColor);
		//background.setOutlineColor(sf::Color(40, 40, 40, 150));
		background.setOutlineColor(sf::Color(9, 33, 43, 150));
		background.setOutlineThickness(5.f);
		break;
	default:
		background.setFillColor(sf::Color::White);
		break;
	}
}

const int Button::getPrice() const
{
	return price;
}
