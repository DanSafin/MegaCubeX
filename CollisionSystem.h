#pragma once

#include "EntityManager.h"
#include "Position.h"
#include "velocity.h"
#include "level.h"
#include <iostream> // For debugging

void TileCollision(EntityManager& em, Entity player, bool& canJump)
{
    canJump = false;

    // Loop through all entities with Position and Velocity
    for (Entity e : em.getEntitiesWith<Position, Velocity>())
    {
        // Safely get Position and Velocity components
        auto pos = em.getComponent<Position>(e);
        auto vel = em.getComponent<Velocity>(e);

        // Check for null components and skip if any component is missing
        if (!pos || !vel)
        {
            std::cerr << "Error: Missing Position or Velocity component for entity " << e << "\n";
            continue;  // Skip this entity if it doesn't have the necessary components
        }

        sf::FloatRect box(pos->x, pos->y, 50, 50);  // player's bounding box

        // Loop through each solid tile in the level
        for (int row = 0; row < MAP_HEIGHT; ++row)
        {
            for (int col = 0; col < MAP_WIDTH; ++col)
            {
                if (levelData[row][col] != 1) continue;  // Skip non-solid tiles

                sf::FloatRect tilebox(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE);

                // Handle Vertical Collisions (Top and Bottom)
                if (box.intersects(tilebox))
                {
                    // Top Collision (Player falling onto tile)
                    if (vel->dy > 0 && box.top + box.height <= tilebox.top + vel->dy)
                    {
                        pos->y = tilebox.top - box.height;  // Place player on top of the tile
                        vel->dy = 0;  // Stop downward movement

                        if (e == player)
                        {
                            canJump = true;  // Allow jumping
                        }
                    }

                    // Bottom Collision (Player moving up and hitting a tile)
                    if (vel->dy < 0 && box.top >= tilebox.top + TILE_SIZE)
                    {
                        pos->y = tilebox.top + TILE_SIZE;  // Place player just above the tile
                        vel->dy = 0;  // Stop upward movement
                    }
                }
            }
        }

        // Handle Horizontal Collisions (Left and Right)
        for (int row = 0; row < MAP_HEIGHT; ++row)
        {
            for (int col = 0; col < MAP_WIDTH; ++col)
            {
                if (levelData[row][col] != 1) continue;  // Skip non-solid tiles

                sf::FloatRect tilebox(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE);

                // Left Collision (Player moving left)
                if (vel->dx < 0 && box.intersects(tilebox))
                {
                    float verticalOverlap = std::min(box.top + box.height, tilebox.top + tilebox.height) - std::max(box.top, tilebox.top);
                    // Check if the player is moving left and collides with the tile
                    if (verticalOverlap > 5.f)
                    {
                        pos->x = tilebox.left + TILE_SIZE;  // Place player just right of the tile
                        vel->dx = 0;  // Stop horizontal movement
                        std::cout << "Left collision: " << box.left << ", " << tilebox.left << "\n"; // Debug log
                    }
                }

                // Right Collision (Player moving right)
                if (vel->dx > 0 && box.intersects(tilebox))
                {
                    // Check if the player is moving right and collides with the tile
                    float verticalOverlap = std::min(box.top + box.height, tilebox.top + tilebox.height) - std::max(box.top, tilebox.top);
                    if (verticalOverlap > 5.f)
                    {
                        pos->x = tilebox.left - box.width;  // Place player just left of the tile
                        vel->dx = 0;  // Stop horizontal movement
                        std::cout << "Right collision: " << box.left << ", " << tilebox.left << "\n"; // Debug log
                    }
                }
            }
        }
    }
}
