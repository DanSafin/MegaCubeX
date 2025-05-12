#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Position.h"
#include "velocity.h"
#include "MovementSystem.h"
#include "GravitySystem.h"
#include "InputSystem.h"
#include "CollisionSystem.h"
#include "level.h"
#include "ProjectileSystem.h"
#include "Enemy.h"
#include "ProjectileCollisionSystem.h"

int main()
{
	// Window
	sf::RenderWindow window(sf::VideoMode(800, 600), "MX_C");
	window.setFramerateLimit(60);

	// Camera
	sf::View camera(sf::FloatRect(0, 0, 800, 600));

	// Player
	EntityManager em;
	Entity player = em.createEntity();
	em.addComponent(player, Position{ 100.f, 100.f });
	em.addComponent(player, Velocity{ 50.f, 50.f });

	sf::RectangleShape playerShape(sf::Vector2f(50.f, 50.f));
	playerShape.setFillColor(sf::Color::Blue);

	bool canJump = true;

	// Enemy
	for (int row = 0; row < MAP_HEIGHT; ++row)
	{
		for (int col = 0; col < MAP_WIDTH; ++col)
		{
			if (levelData[row][col] == 2)
			{
				Entity enemy = em.createEntity();
				em.addComponent(enemy, Position{ (float)col * TILE_SIZE, (float)row * TILE_SIZE });
				em.addComponent(enemy, Velocity{ 0.f, 0.f });
				em.addComponent(enemy, Enemy{ 1 });
			}
		}
	}

	sf::RectangleShape enemyShape(sf::Vector2f{ 50.f, 50.f });
	enemyShape.setFillColor(sf::Color::Red);

	// Tile
	sf::RectangleShape tileShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	tileShape.setFillColor(sf::Color::Green);

	// Bullet
	sf::RectangleShape bulletShape(sf::Vector2f(10.f, 5.f));
	bulletShape.setFillColor(sf::Color::Yellow);

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();

		PlayerInput(em, player, canJump);
		Gravity(em, dt);
		Movement(em, dt);
		TileCollision(em, player, canJump);
		UpdateProjectiles(em, dt);
		ProjectileCollisions(em);

		auto pos = em.getComponent<Position>(player);
		if (pos)
		{
			float camX = pos->x + 25 - 400; // center on player (assuming 50x50)
			float camY = pos->y + 25 - 300;

			// Clamp camera to level bounds
			float levelWidth = MAP_WIDTH * TILE_SIZE;
			float levelHeight = MAP_HEIGHT * TILE_SIZE;

			camX = std::max(0.f, std::min(camX, levelWidth - 800.f));
			camY = std::max(0.f, std::min(camY, levelHeight - 600.f));

			camera.setCenter(camX + 400, camY + 300); // recenters camera
			window.setView(camera);

			playerShape.setPosition(pos->x, pos->y);
		}

		window.clear();
		window.draw(playerShape);
		for (int row = 0; row < MAP_HEIGHT; ++row)
		{
			for (int col = 0; col < MAP_WIDTH; ++col)
			{
				if (levelData[row][col] == 1)
				{
					tileShape.setPosition(col * TILE_SIZE, row * TILE_SIZE);
					window.draw(tileShape);
					
				}
			}
		}
		for (Entity e : em.getEntitiesWith<Position, Projectile>())
		{
			auto pos = em.getComponent<Position>(e);
			if (pos)
			{
				bulletShape.setPosition(pos->x, pos->y);
				window.draw(bulletShape);
			}
		}
		for (Entity e : em.getEntitiesWith<Enemy, Position>())
		{
			auto pos = em.getComponent<Position>(e);
			if (pos)
			{
				enemyShape.setPosition(pos->x, pos->y);
				window.draw(enemyShape);
			}
		}
		window.display();
	}

	return 0;
}