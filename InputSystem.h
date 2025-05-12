#pragma once

#include <SFML/Window.hpp>
#include "EntityManager.h"
#include "velocity.h"
#include "Projectile.h"

void PlayerInput(EntityManager& em, Entity player, bool canJump)
{
	auto vel = em.getComponent<Velocity>(player);
	if (!vel) return;
	static bool canShoot = true;

	vel->dx = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		vel->dx = -200.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		vel->dx = 200.0f;
	}
	if (canJump && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		vel->dy = -400.0f; // jump strength
	}
	if (canShoot && sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		Position* pos = em.getComponent<Position>(player);
		if (pos)
		{
			Entity bullet = em.createEntity();
			em.addComponent(bullet, Position{ pos->x + 45.f, pos->y + 20.f }); // offset from player
			em.addComponent(bullet, Velocity{ 500.f, 0.f }); // move right
			em.addComponent(bullet, Projectile{});
			
		}
		canShoot = false;
	}
	if (!canShoot && !sf::Keyboard::isKeyPressed(sf::Keyboard::J))
	{
		canShoot = true;
	}
}