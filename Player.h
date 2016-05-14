#ifndef PLAYER_H
#define PLAYER_H

#include "Command.h"

#include <SFML/Window/Event.hpp>

#include <map>

class CommandQueue;

class Player
{
public:
	enum Action
	{
		TurnLeft,
		TurnRight,
		Accelerate,
		SlowDown,
		Shoot,
		ActionCount
	};

public:
	Player();
	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

private:
	void initializeActions();
	static bool isRealtimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Action> _keyBinding;
	std::map<Action, Command> _actionBinding;

};

#endif //PLAYER_H
