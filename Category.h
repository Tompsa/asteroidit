#ifndef CATEGORY_H
#define CATEGORY_H

// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		PlayerSpaceship = 1 << 1,
        EnemySpaceship = 1 << 2,
        AlliedProjectile	= 1 << 3,
		EnemyProjectile		= 1 << 4,
        SpaceDebris         = 1 << 5,
        
        Ship = PlayerSpaceship | EnemySpaceship,
		Projectile = AlliedProjectile | EnemyProjectile,
	};
}

#endif //CATEGORY_H
