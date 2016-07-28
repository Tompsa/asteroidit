#ifndef SHIP_H
#define SHIP_H

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"
#include "Projectile.h"
#include "TextNode.h"

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
    virtual sf::FloatRect	getBoundingRect() const;
	virtual bool 			isMarkedForRemoval() const;
	float					getMaxSpeed() const;
    void                    setGodmode(char var);
    bool                    isGod();

	void 					fire();
    void                    warp();


private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;


private:
	Type					_type;
	sf::Sprite				_sprite;
    Command 				_fireCommand;
	sf::Time				_fireCountdown;
	bool 					_isFiring;
	bool 					_isMarkedForRemoval;

	float					_travelledDistance;
	std::size_t				_directionIndex;
    sf::Time                _invisTimer;
    bool                    _godmode;                   
};

#endif // SHIP_H
