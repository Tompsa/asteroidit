#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "Player.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game : private sf::NonCopyable
{
public:
	Game();
	
	void run();
	
	void processInput();
	void update(sf::Time delta);
	void render();
    void updateStatistics(sf::Time elapsedTime);

private:
	static const sf::Time TimePerFrame;

	sf::RenderWindow _window;
	World _world;
	Player _player;

	sf::Font _font;
	sf::Text _statisticsText;
	sf::Time _statisticsUpdateTime;
	std::size_t	_statisticsNumFrames;
};


#endif //GAME_H
