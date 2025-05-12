#pragma once


#include "EntityManager.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Position.h"
#include <SFML/Graphics.hpp>

void ProjectileCollisions(EntityManager& em)
{
	std::vector<Entity> projectilesToRemove;
	std::vector<Entity> enemiesToRemove;

	for (Entity bullet : em.getEntitiesWith<Projectile, Position>())
	{
		auto bulletPos = em.getComponent<Position>(bullet);
		if (!bulletPos) continue;

		for (Entity enemy : em.getEntitiesWith<Enemy, Position>())
		{
			auto enemyPos = em.getComponent<Position>(enemy);
			if (!enemyPos) continue;

			// Simple AABB hitbox logic
			sf::FloatRect bulletRect(bulletPos->x, bulletPos->y, 10.f, 10.f);
			sf::FloatRect enemyRect(enemyPos->x, enemyPos->y, 50.f, 50.f);

			if (bulletRect.intersects(enemyRect))
			{
				projectilesToRemove.push_back(bullet);
				enemiesToRemove.push_back(enemy);
			}
		}
	}

	for (Entity b : projectilesToRemove) em.destroyEntity(b);
	for (Entity e : enemiesToRemove) em.destroyEntity(e);
}