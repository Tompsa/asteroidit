#ifndef ASTEROID_H
#define ASTEROID_H

#include "Entity.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Asteroid : public Entity
{
    public:
        enum Type
        {
            Asteroid1,
            Asteroid2,
            TypeCount
        };
    
    public:
        Asteroid(Type type, const TextureHolder& textures);
        
        void                    setDirection(sf::Vector2f position);
        
        virtual unsigned int	getCategory() const;
        virtual sf::FloatRect	getBoundingRect() const;
        bool                    isShard() const;
        float					getMaxSpeed() const;
        int						getDamage() const;

	
	private:
		virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type					_type;
		sf::Sprite				_sprite;
        sf::Vector2f            _targetDirection;

};



#endif //ASTEROID_H
