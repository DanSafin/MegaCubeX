#pragma once

#include "EntityManager.h"
#include "Position.h"
#include "velocity.h"
#include "Projectile.h"

void UpdateProjectiles(EntityManager& em, float dt)
{
	std::vector<Entity> entitiesToDestroy;
	
	for (Entity e : em.getEntitiesWith<Position, Velocity, Projectile>())
	{
		auto pos = em.getComponent<Position>(e);
		auto vel = em.getComponent<Velocity>(e);
		auto proj = em.getComponent<Projectile>(e);

		// Ensure all components are valid before accessing
		if (!pos || !vel || !proj) continue;

		// Move projectile
		pos->x += vel->dx * dt;
		pos->y += vel->dy * dt;

		// Reduce lifetime
		proj->lifetime -= dt;
		if (proj->lifetime <= 0.f)
		{
			entitiesToDestroy.push_back(e);
		}
	}

	// After processing all projectiles, destroy them
	for (Entity e : entitiesToDestroy)
	{
		em.destroyEntity(e);
	}
}