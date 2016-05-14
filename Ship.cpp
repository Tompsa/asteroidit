#include "Ship.h"
#include "ResourceHolder.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


Textures::ID toTextureID(Ship::Type type)
{
	switch (type)
	{
	case Ship::Spaceship:
		return Textures::Spaceship;
	}
	return Textures::Spaceship;
}

Ship::Ship(Type type, const TextureHolder& textures)
	: _type(type)
	, _sprite(textures.get(toTextureID(type)))
{
	centerOrigin(_sprite);
}

void Ship::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

unsigned int Ship::getCategory() const
{
	switch (_type)
	{
	case Spaceship:
		return Category::PlayerSpaceShip;
        
    case AlienShip:
        return Category::EnemySpaceShip;

	default:
		return Category::PlayerSpaceShip;
	}
}
