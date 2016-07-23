#include "Player.h"
#include "CommandQueue.h"
#include "Ship.h"
#include "Foreach.h"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std::placeholders;


struct SpaceshipMover
{
	SpaceshipMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Ship& ship, sf::Time) const
	{
		ship.accelerate(velocity*ship.getMaxSpeed());
        std::cout << "X velocity "<< ship.getVelocity().x << "\n";
        std::cout << "y velocity "<< ship.getVelocity().y << "\n";
	}

	sf::Vector2f velocity;
};

struct SpaceshipThruster
{
	SpaceshipThruster(float f)
		: force(f)
	{

	}

	void operator() (Ship& ship, sf::Time) const
	{
        float rotation = ship.getRotation();
        rotation -= 90;
        
        //sf::Vector2f currentVelocity = ship.getVelocity();
        //if(std::abs(currentVelocity.x) <= ship.getMaxSpeed() || std::abs(currentVelocity.y) <= ship.getMaxSpeed())
        //{
            //const float approachRate = 200.f;

            //sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * _targetDirection + getVelocity());
            //newVelocity *= getMaxSpeed();
            //float angle = std::atan2(newVelocity.y, newVelocity.x);

            //setRotation(toDegree(angle) + 90.f);
            //setVelocity(newVelocity);
        
            sf::Vector2f velocity;
            velocity.x = (float)std::cos(rotation * (3.1415926 / 180.0f))*force;
            velocity.y = (float)std::sin(rotation * (3.1415926 / 180.0f))*force;
            ship.accelerate(velocity);
        //}
        //std::cout << "X velocity "<< ship.getVelocity().x << "\n";
        //std::cout << "y velocity "<< ship.getVelocity().y << "\n";
	}

	float force;
};

struct SpaceshipRotator
{
    SpaceshipRotator(float a)
        : angle(a)
    {
    }
    
    void operator () (Ship& ship, sf::Time) const
    {
        ship.rotate(angle);
    }
    
    float angle;
};

Player::Player()
: _currentMissionStatus(MissionRunning)
{
	// Set initial key bindings
	_keyBinding[sf::Keyboard::Left] = TurnLeft;
	_keyBinding[sf::Keyboard::Right] = TurnRight;
	_keyBinding[sf::Keyboard::Up] = Accelerate;
	_keyBinding[sf::Keyboard::Down] = Warp;
	_keyBinding[sf::Keyboard::Space] = Fire;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's aircraft
	FOREACH(auto& pair, _actionBinding)
		pair.second.category = Category::PlayerSpaceship;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = _keyBinding.find(event.key.code);
		if (found != _keyBinding.end() && !isRealtimeAction(found->second))
			commands.push(_actionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, _keyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(_actionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = _keyBinding.begin(); itr != _keyBinding.end();)
	{
		if (itr->second == action)
			_keyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	_keyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, _keyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatus status)
{
	_currentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return _currentMissionStatus;
}


void Player::initializeActions()
{
    _actionBinding[TurnLeft].action = derivedAction<Ship>(SpaceshipRotator(-10));
    _actionBinding[TurnRight].action = derivedAction<Ship>(SpaceshipRotator(10));
    _actionBinding[Accelerate].action = derivedAction<Ship>(SpaceshipThruster(+10));
    _actionBinding[Warp].action = derivedAction<Ship>([](Ship& a, sf::Time){ a.warp(); });
	_actionBinding[Fire].action = derivedAction<Ship>([](Ship& a, sf::Time){ a.fire(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case TurnLeft:
	case TurnRight:
	case Accelerate:
	case Fire:
		return true;

	default:
		return false;
	}
}
