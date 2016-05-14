#ifndef SHIP_H
#define SHIP_H

#include "Entity.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>


class Ship : public Entity
{
public:
	enum Type
	{
		Spaceship,
		AlienShip,
		TypeCount
	};


public:
	Ship(Type type, const TextureHolder& textures);
	virtual unsigned int	getCategory() const;


private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	Type					_type;
	sf::Sprite				_sprite;
};

#endif // SHIP_H
