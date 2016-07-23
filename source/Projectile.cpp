#include "Projectile.h"
#include "DataTables.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>


namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
	: Entity(1)
	, _type(type)
	, _sprite(textures.get(Table[type].texture))
	, _targetDirection()
{
	centerOrigin(_sprite);
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	_targetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
	return _type == Missile;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

unsigned int Projectile::getCategory() const
{
	if (_type == EnemyBullet)
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[_type].speed;
}

int Projectile::getDamage() const
{
	return Table[_type].damage;
}
