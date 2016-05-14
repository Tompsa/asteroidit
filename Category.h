#ifndef CATEGORY_H
#define CATEGORY_H

// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		PlayerSpaceShip = 1 << 1,
        EnemySpaceShip = 1 << 2,
	};
}

#endif //CATEGORY_H
