#include "World.h"

#include "SFML/Graphics/RenderWindow.hpp"

#include <algorithm>
#include <cmath>

World::World(sf::RenderWindow& window)
	: _window(window)
	, _worldView(window.getDefaultView())
	, _textures()
	, _sceneGraph()
	, _sceneLayers()
	, _worldBounds(0.f, 0.f, _worldView.getSize().x, 2000.f)
	, _spawnPosition(_worldView.getSize().x / 2.f, _worldBounds.height - _worldView.getSize().y / 2.f)
	, _scrollSpeed(-50.f)
	, _playerShip(nullptr)
{
	loadTextures();
	buildScene();
	
	_worldView.setCenter(_spawnPosition);
}

void World::update(sf::Time dt)
{
    // Scroll the world, reset player velocity
	//_worldView.move(0.f, _scrollSpeed * dt.asSeconds());	
	//_playerShip->setVelocity(0.f, 0.f);
    
	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!_commandQueue.isEmpty())
		_sceneGraph.onCommand(_commandQueue.pop(), dt);
	adaptPlayerVelocity();

	// Regular update step, adapt position (correct if outside view)
	_sceneGraph.update(dt);
	adaptPlayerPosition();
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

void World::loadTextures()
{
	_textures.load(Textures::Space, "Media/Textures/spacebg.jpg");
	_textures.load(Textures::Spaceship, "Media/Textures/rocket.png");
	//_textures.load(Textures::Asteroid, "Media/Textures/rock.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
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
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
	//const float borderDistance = 40.f;

	sf::Vector2f position = _playerShip->getPosition();
	//position.x = std::max(position.x, viewBounds.left + borderDistance);
	//position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	//position.y = std::max(position.y, viewBounds.top + borderDistance);
	//position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    
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

void World::adaptPlayerVelocity()
{
	//sf::Vector2f velocity = _playerShip->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	//if (velocity.x != 0.f && velocity.y != 0.f)
	//	_playerShip->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	//_playerShip->accelerate(0.f, _scrollSpeed);
}
