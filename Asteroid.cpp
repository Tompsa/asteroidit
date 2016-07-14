#include "Asteroid.h"
#include "DataTables.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace
{
	const std::vector<AsteroidData> Table = initializeAsteroidData();
}

Asteroid::Asteroid(Type type, const TextureHolder& textures)
	: Entity(1)
    , _type(type)
	, _sprite(textures.get(Table[type].texture))
    , _targetDirection()
{
	centerOrigin(_sprite);
}

void Asteroid::setDirection(sf::Vector2f position)
{
    _targetDirection = unitVector(position - getWorldPosition());
    sf::Vector2f newVelocity = unitVector(_targetDirection);
    newVelocity *= getMaxSpeed();
    setVelocity(newVelocity);
}

unsigned int Asteroid::getCategory() const
{
    return Category::SpaceDebris; 
}

sf::FloatRect Asteroid::getBoundingRect() const
{
    return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

float Asteroid::getMaxSpeed() const
{
    return Table[_type].speed;
}

int	Asteroid::getDamage() const
{
    return Table[_type].damage;
}
        
void Asteroid::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    rotate(2);
    Entity::updateCurrent(dt, commands);
}

void Asteroid::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);    
}
