#include "Game.h"
#include "StringHelpers.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: _window(sf::VideoMode(640, 480), "ASTEROIDIT")
	, _world(_window)
	, _player()
    , _font()
    , _statisticsText()
    , _statisticsUpdateTime()
    , _statisticsNumFrames(0)
{
	_window.setKeyRepeatEnabled(0);
    
    _font.loadFromFile("Media/Sansation.ttf");
	_statisticsText.setFont(_font);
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(10);
}

void Game::processInput()
{
	CommandQueue& commands = _world.getCommandQueue();

	sf::Event event;
	while (_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			_window.close();
	}
	_player.handleRealtimeInput(commands);
}

void Game::update(sf::Time delta)
{
	_world.update(delta);
}

void Game::render()
{
	_window.clear();
	_world.draw();
	
    _window.setView(_window.getDefaultView());
    _window.draw(_statisticsText);
    _window.display();
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);
		}

        updateStatistics(elapsedTime);
		render();
	}
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	_statisticsUpdateTime += elapsedTime;
	_statisticsNumFrames += 1;

	if (_statisticsUpdateTime >= sf::seconds(1.0f))
	{
		_statisticsText.setString(
			"Frames / Second = " + toString(_statisticsNumFrames) + "\n" +
			"Time / Update = " + toString(_statisticsUpdateTime.asMicroseconds() / _statisticsNumFrames) + "us");
							 
		_statisticsUpdateTime -= sf::seconds(1.0f);
		_statisticsNumFrames = 0;
	}
}
