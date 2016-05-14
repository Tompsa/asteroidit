#include "MenuState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, _options()
, _optionIndex(0)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	_backgroundSprite.setTexture(texture);
	
	// A simple menu demonstration
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	_options.push_back(playOption);

	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	_options.push_back(exitOption);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(_backgroundSprite);

	FOREACH(const sf::Text& text, _options)
		window.draw(text);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// The demonstration menu logic
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Return)
	{
		if (_optionIndex == Play)
		{
			requestStackPop();
			requestStackPush(States::Game);
		}
		else if (_optionIndex == Exit)
		{
			// The exit option was chosen, by removing itself, the stack will be empty, and the game will know it is time to close.
			requestStackPop();
		}
	}

	else if (event.key.code == sf::Keyboard::Up)
	{
		// Decrement and wrap-around
		if (_optionIndex > 0)
			_optionIndex--;
		else
			_optionIndex = _options.size() - 1;

		updateOptionText();
	}

	else if (event.key.code == sf::Keyboard::Down)
	{
		// Increment and wrap-around
		if (_optionIndex < _options.size() - 1)
			_optionIndex++;
		else
			_optionIndex = 0;

		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (_options.empty())
		return;

	// White all texts
	FOREACH(sf::Text& text, _options)
		text.setColor(sf::Color::White);

	// Red the selected text
	_options[_optionIndex].setColor(sf::Color::Red);
}
