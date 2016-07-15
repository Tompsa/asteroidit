#include "World.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Foreach.h"
#include "TextNode.h"
#include "Utility.h"

#include "SFML/Graphics/RenderWindow.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

World::World(sf::RenderWindow& window, FontHolder& fonts)
	: _window(window)
	, _worldView(window.getDefaultView())
	, _textures()
    , _fonts(fonts)
	, _sceneGraph()
	, _sceneLayers()
	, _worldBounds(0.f, 0.f, _worldView.getSize().x, _worldView.getSize().y)
	, _spawnPosition(_worldView.getSize().x / 2.f, _worldBounds.height - _worldView.getSize().y / 2.f)
	, _playerShip(nullptr)
    , _playerScore(0)
    , _playerLives(3)
    , _activeAsteroids()
    , _scoreDisplay(nullptr)
    , _livesDisplay(nullptr)
{
	loadTextures();
	buildScene();
    
    std::unique_ptr<TextNode> scoreDisplay(new TextNode(fonts, ""));
	_scoreDisplay = scoreDisplay.get();
	_sceneLayers[ObjectLayer]->attachChild(std::move(scoreDisplay));
	
    updateTexts();
}

void World::update(sf::Time dt)
{
	// Forward commands to scene graph
	while (!_commandQueue.isEmpty())
		_sceneGraph.onCommand(_commandQueue.pop(), dt);

	// Collision detection and response (may destroy entities)
	handleCollisions();

    // Remove all destroyed entities, create new ones
	_sceneGraph.removeWrecks();
    spawnAsteroids();
    
    
    
    // Command that stores all asteroids on screen in _activeAsteroids
    Command asteroidCollector;
	asteroidCollector.category = Category::SpaceDebris;
	asteroidCollector.action = derivedAction<Asteroid>([this] (Asteroid& asteroid, sf::Time)
	{
		if (!asteroid.isDestroyed())
			_activeAsteroids.push_back(&asteroid);
	});
    
    // Push command 
    _commandQueue.push(asteroidCollector);
    
    // If all asteroids are destroyed, add more to spawn
    if(_activeAsteroids.empty())
        addAsteroids();
    _activeAsteroids.clear();
    
    

	// Regular update step, adapt position (correct if outside view)
	_sceneGraph.update(dt, _commandQueue);
	adaptPlayerPosition();
    adaptAsteroidPosition();
    
    // Update score display
    updateTexts();
}

void World::draw()
{
	_window.setView(_worldView);
	_window.draw(_sceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return _commandQueue;
}

bool World::hasAlivePlayer() const
{
	return !_playerShip->isMarkedForRemoval();
}

void World::loadTextures()
{
	_textures.load(Textures::Space, "Media/Textures/spacebg.jpg");
	_textures.load(Textures::Spaceship, "Media/Textures/rocket.png");
	_textures.load(Textures::Asteroid, "Media/Textures/rock.png");
    _textures.load(Textures::AsteroidShard, "Media/Textures/rock2.png");

	_textures.load(Textures::Bullet, "Media/Textures/Bullet.png");
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	_sceneGraph.checkSceneCollision(_sceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerSpaceship, Category::EnemySpaceship))
		{
			auto& player = static_cast<Ship&>(*pair.first);
			auto& enemy = static_cast<Ship&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}
        
        else if(matchesCategories(pair, Category::PlayerSpaceship, Category::SpaceDebris))
        {
            auto& player = static_cast<Ship&>(*pair.first);
            auto& asteroid = static_cast<Asteroid&>(*pair.second);
            
            player.damage(asteroid.getHitpoints());
            asteroid.destroy();
        }
        
        else if(matchesCategories(pair, Category::AlliedProjectile, Category::SpaceDebris))
        {
            auto& projectile = static_cast<Projectile&>(*pair.first);
            auto& asteroid = static_cast<Asteroid&>(*pair.second);
            
            sf::Vector2f pos = asteroid.getPosition();
            
            if(!asteroid.isShard())
            {            
            addAsteroid(Asteroid::Asteroid2, pos.x, pos.y);
            addAsteroid(Asteroid::Asteroid2, pos.x, pos.y);
            }

            asteroid.damage(projectile.getDamage());
            _playerScore += 100;
                
            projectile.destroy();
        }

		else if (matchesCategories(pair, Category::EnemySpaceship, Category::AlliedProjectile)
			|| matchesCategories(pair, Category::PlayerSpaceship, Category::EnemyProjectile))
		{
			auto& ship = static_cast<Ship&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			// Apply projectile damage to aircraft, destroy projectile
			ship.damage(projectile.getDamage());
			projectile.destroy();
		}
	}
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == ObjectLayer) ? Category::Scene : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		_sceneLayers[i] = layer.get();

		_sceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = _textures.get(Textures::Space);
	sf::IntRect textureRect(_worldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);
	_sceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add player's spaceship
	std::unique_ptr<Ship> ship(new Ship(Ship::Spaceship, _textures));
	_playerShip = ship.get();
	_playerShip->setPosition(_spawnPosition);
	_sceneLayers[ObjectLayer]->attachChild(std::move(ship));

	// Add space debris
	addAsteroids();
}

void World::addAsteroids()
{
    for(int i = 0; i < 2 ; i++)
        addAsteroid(Asteroid::Asteroid1, 0.f, randomInt(480));    
    for(int i = 0; i < 2 ; i++)
        addAsteroid(Asteroid::Asteroid1, 640.f, randomInt(480));    
    
    std::sort(_asteroidSpawnPoints.begin(), _asteroidSpawnPoints.end(), [] (SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addAsteroid(Asteroid::Type type ,float relX, float relY)
{
    //SpawnPoint spawn(type, _spawnPosition.x + relX, _spawnPosition.y - relY);
    SpawnPoint spawn(type, relX, relY);
	_asteroidSpawnPoints.push_back(spawn);  
}

void World::spawnAsteroids()
{
    while (!_asteroidSpawnPoints.empty()
		&& _asteroidSpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = _asteroidSpawnPoints.back();
        
        // A random point to which an asteroid floats towards 
        sf::Vector2f pos; 
        pos.x = randomInt(640);
        pos.y = randomInt(480);
		
		std::unique_ptr<Asteroid> asteroid(new Asteroid(spawn.type,_textures));
		asteroid->setPosition(spawn.x, spawn.y);
        // Set velocity towards random point
        asteroid->setDirection(pos);        

		_sceneLayers[ObjectLayer]->attachChild(std::move(asteroid));

		// Asteroid is spawned, remove from the list to spawn
		_asteroidSpawnPoints.pop_back();
	}
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds
	sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());

	sf::Vector2f position = _playerShip->getPosition();
    
    // Wrap position around screen, 
    // i.e. ship crossing screen boundary ends up on the other side of screen
    if(position.x > viewBounds.left + viewBounds.width )
        position.x = 0;
    if(position.x < viewBounds.left)
        position.x = viewBounds.width;
    if(position.y < viewBounds.top )
        position.y = viewBounds.top + viewBounds.height;
    if(position.y > viewBounds.top + viewBounds.height)
        position.y = viewBounds.top;   
        
	_playerShip->setPosition(position);
}

void World::adaptAsteroidPosition()
{
    Command command;
	command.category = Category::SpaceDebris;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
	{
        sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
        
		sf::Vector2f position = e.getPosition();
    
        // Wrap position around, 
        // i.e. ship crossing screen boundary ends up on other side of screen
        if(position.x > viewBounds.left + viewBounds.width )
            position.x = 0;
        if(position.x < viewBounds.left)
            position.x = viewBounds.width;
        if(position.y < viewBounds.top )
            position.y = viewBounds.top + viewBounds.height;
        if(position.y > viewBounds.top + viewBounds.height)
            position.y = viewBounds.top;   
            
        e.setPosition(position);
	});

	_commandQueue.push(command);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::updateTexts()
{
    _scoreDisplay->setString(toString(_playerScore));
    _scoreDisplay->setPosition(_worldView.getSize().x / 2.f, 50.f);
}
