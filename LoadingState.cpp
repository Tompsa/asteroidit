#include "LoadingState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context)
{
	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	_loadingText.setFont(font);
	_loadingText.setString("Loading Resources");
	centerOrigin(_loadingText);
	_loadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	_progressBarBackground.setFillColor(sf::Color::White);
	_progressBarBackground.setSize(sf::Vector2f(viewSize.x - 20, 10));
	_progressBarBackground.setPosition(10, _loadingText.getPosition().y + 40);

	_progressBar.setFillColor(sf::Color(100,100,100));
	_progressBar.setSize(sf::Vector2f(200, 10));
	_progressBar.setPosition(10, _loadingText.getPosition().y + 40);

	setCompletion(0.f);

	_loadingTask.execute();
}

void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(_loadingText);
	window.draw(_progressBarBackground);
	window.draw(_progressBar);
}

bool LoadingState::update(sf::Time)
{
	// Update the progress bar from the remote task or finish it
	if (_loadingTask.isFinished())
	{
		requestStackPop();
		requestStackPush(States::Game);
	}
	else
	{
		setCompletion(_loadingTask.getCompletion());
	}
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}

void LoadingState::setCompletion(float percent)
{
	if (percent > 1.f) // clamp
		percent = 1.f;

	_progressBar.setSize(sf::Vector2f(_progressBarBackground.getSize().x * percent, _progressBar.getSize().y));
}
