#include "Ship.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "DataTables.h"
#include "CommandQueue.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

namespace
{
	const std::vector<ShipData> Table = initializeShipData();
}

Ship::Ship(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, _type(type)
	, _sprite(textures.get(Table[type].texture))
    , _fireCommand()
    , _fireCountdown(sf::Time::Zero)
    , _isFiring(false)
    , _isMarkedForRemoval(false)
    , _travelledDistance(0.f)
    , _directionIndex(0)
{
	centerOrigin(_sprite);
    
    _fireCommand.category = Category::Scene;
	_fireCommand.action   = [this, &textures] (SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};
}

void Ship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

void Ship::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed
	if (isDestroyed())
	{
		_isMarkedForRemoval = true;
		return;
	}

	// Check if bullets fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity & friction
	Entity::updateCurrent(dt, commands);
    setVelocity((1-0.025)*getVelocity().x, (1-0.025)*getVelocity().y);
}

unsigned int Ship::getCategory() const
{
	switch (_type)
	{
	case Spaceship:
		return Category::PlayerSpaceship;
        
    case AlienShip:
        return Category::EnemySpaceship;

	default:
		return Category::PlayerSpaceship;
	}
}

sf::FloatRect Ship::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

bool Ship::isMarkedForRemoval() const
{
	return _isMarkedForRemoval;
}

float Ship::getMaxSpeed() const
{
	return Table[_type].speed;
}

void Ship::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (Table[_type].fireInterval != sf::Time::Zero)
		_isFiring = true;
}

void Ship::warp()
{
    int newX = randomInt(640-10);
    int newY = randomInt(480-10);
    setPosition(newX, newY);
    setVelocity(0.f, 0.f);
}

void Ship::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Enemies try to fire all the time

	// Check for automatic gunfire, allow only in intervals
	if (_isFiring && _fireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(_fireCommand);
		_fireCountdown += Table[_type].fireInterval / (2.f);
		_isFiring = false;
	}
	else if (_fireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		_fireCountdown -= dt;
		_isFiring = false;
	}
}

void Ship::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::AlliedBullet;

	createProjectile(node, type, 0.0f, 0.0f, textures);
}

void Ship::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * _sprite.getGlobalBounds().width, yOffset * _sprite.getGlobalBounds().height);
	sf::Vector2f velocity(projectile->getMaxSpeed(), projectile->getMaxSpeed());
    float rotation =  getRotation();
    
    velocity.x *= (float)std::cos((rotation-90.0f) * (3.1415926 / 180.0f));
    velocity.y *= (float)std::sin((rotation-90.0f) * (3.1415926 / 180.0f));

	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(velocity);
    projectile->setRotation(rotation);
	node.attachChild(std::move(projectile));
}
