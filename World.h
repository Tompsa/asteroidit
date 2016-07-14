#ifndef WORLD_H
#define WORLD_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Ship.h"
#include "Asteroid.h"
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
		explicit							World(sf::RenderWindow& window, FontHolder& fonts);
		void								update(sf::Time dt);
		void								draw();
		
		CommandQueue&						getCommandQueue();

		bool 								hasAlivePlayer() const;


	private:
		void								loadTextures();
		void								adaptPlayerPosition();
		void								handleCollisions();
		
		void								buildScene();
        void                                addAsteroids();
        void                                addAsteroid(Asteroid::Type type, float relX, float relY);
        void                                spawnAsteroids();
        void                                adaptAsteroidPosition();
		sf::FloatRect						getViewBounds() const;
		sf::FloatRect						getBattlefieldBounds() const;


	private:
		enum Layer
		{
			Background,
			ObjectLayer,
			LayerCount
		};

		struct SpawnPoint 
		{
			SpawnPoint(Asteroid::Type type, float x, float y)
			: type(type)
            , x(x)
			, y(y)

			{
			}

            Asteroid::Type type;
			float x;
			float y;

		};


	private:
		sf::RenderWindow&					_window;
		sf::View							_worldView;
		TextureHolder						_textures;
		FontHolder&							_fonts;

		SceneNode							_sceneGraph;
		std::array<SceneNode*, LayerCount>	_sceneLayers;
		CommandQueue						_commandQueue;

		sf::FloatRect						_worldBounds;
		sf::Vector2f						_spawnPosition;
		Ship*							    _playerShip;

		std::vector<SpawnPoint>				_asteroidSpawnPoints;
        std::vector<Asteroid*>				_activeAsteroids;
};

#endif //WORLD_H
