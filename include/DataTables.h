#ifndef DATATABLES_H
#define DATATABLES_H

#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>


class Ship;

struct Direction
{
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct ShipData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::Time						fireInterval;
	std::vector<Direction>			directions;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	Textures::ID					texture;
};

struct AsteroidData
{
    int hitpoints;
    int damage;
    float speed;  
    Textures::ID texture;
    std::vector<Direction> directions;
};


std::vector<ShipData>	initializeShipData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<AsteroidData> initializeAsteroidData();

#endif // DATATABLES_H
