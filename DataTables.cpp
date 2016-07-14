#include "DataTables.h"
#include "Ship.h"
#include "Projectile.h"
#include "Asteroid.h"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<ShipData> initializeShipData()
{
	std::vector<ShipData> data(Ship::TypeCount);

	data[Ship::Spaceship].hitpoints = 1;
	data[Ship::Spaceship].speed = 200.f;
	data[Ship::Spaceship].fireInterval = sf::seconds(1);
	data[Ship::Spaceship].texture = Textures::Spaceship;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Bullet;

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Bullet;

	return data;
}

std::vector<AsteroidData> initializeAsteroidData()
{
    std::vector<AsteroidData> data(Asteroid::TypeCount);
    
    data[Asteroid::Asteroid1].hitpoints = 1;
    data[Asteroid::Asteroid1].damage = 10;
    data[Asteroid::Asteroid1].speed = 50.f;
    data[Asteroid::Asteroid1].texture = Textures::Asteroid;
    
    data[Asteroid::Asteroid2].hitpoints = 1;
    data[Asteroid::Asteroid2].damage = 10;
    data[Asteroid::Asteroid2].speed = 25.f;
    data[Asteroid::Asteroid2].texture = Textures::AsteroidShard;
    
    
    return data;
}
