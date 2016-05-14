#ifndef WORLD_H
#define WORLD_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Ship.h"
#include "CommandQueue.h"
#include "Command.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);
	void update(sf::Time dt);
	void draw();

	CommandQueue& getCommandQueue();

private:
	void loadTextures();
	void buildScene();
	void adaptPlayerPosition();
	void adaptPlayerVelocity();

private:
	enum Layer
	{
		Background,
		ObjectLayer,
		LayerCount
	};

private:
	sf::RenderWindow& _window;
	sf::View _worldView;
	TextureHolder _textures;

	SceneNode _sceneGraph;
	std::array<SceneNode*, LayerCount> _sceneLayers;
	CommandQueue _commandQueue;

	sf::FloatRect _worldBounds;
	sf::Vector2f _spawnPosition;
	float _scrollSpeed;
	Ship* _playerShip;

};

#endif //WORLD_H
