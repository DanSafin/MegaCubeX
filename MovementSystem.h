#pragma once

#include "EntityManager.h"
#include "Position.h"
#include "velocity.h"

void Movement(EntityManager& em, float dt)
{
	auto& velMap = em.getAllComponents<Velocity>();
	for (auto& [entity, velPtr] : velMap)
	{
		auto pos = em.getComponent<Position>(entity);
		auto vel = static_cast<Velocity*>(velPtr.get());
		if (pos && vel)
		{
			pos->x += vel->dx * dt;
			pos->y += vel->dy * dt;
		}
	}
}