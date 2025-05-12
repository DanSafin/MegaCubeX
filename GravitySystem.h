#pragma once

#include "EntityManager.h"
#include "velocity.h"
#include "Projectile.h"

const float GRAVITY = 800.0f; // pixels per second squared

void Gravity(EntityManager& em, float dt)
{
	auto& velocities = em.getAllComponents<Velocity>();
	for (auto& [entity, velPtr] : velocities)
	{
		if (em.getComponent<Projectile>(entity)) continue; // skip projectiles

		auto vel = static_cast<Velocity*>(velPtr.get());
		vel->dy += GRAVITY * dt;
	}
}