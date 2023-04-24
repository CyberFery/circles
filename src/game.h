#ifndef GAME_H
#define GAME_H

#include "sdl2.h"
#include "constants.h"
#include <stdbool.h>


// --------------- //
// Data structures //
// --------------- //

typedef struct {
    bool active;
    float distance;
    int sector;
    int inner_radius;
    int outer_radius;
    int spawn_delay;
} ArcObstacle;

enum game_state{
    GAME_ON,
    GAME_OVER
};

struct game{
    enum game_state state;
    SDL_Renderer *renderer;
};

// --------- //
// Functions //
// --------- //

/**
 * Initializes a new game structure with the provided SDL_Renderer and sets its
 * state to GAME_ON.
 * @param  renderer    A pointer to the SDL_Renderer to be used for rendering the
 * game.
 * @return A pointer to the newly created game structure with the initialized
 * renderer and state.
 */
struct game *game_initialize(SDL_Renderer *renderer);
struct game *game_initialize(SDL_Renderer *renderer);
/**
 * Runs the main game loop.
 *
 * @param game         A pointer to the game structure.
 */
void game_run(struct game *game);

/**
 * Draws a circle on the renderer.
 *
 * @param renderer  The SDL_Renderer to draw on.
 * @param x         The x-coordinate of the circle's center.
 * @param y         The y-coordinate of the circle's center.
 * @param radius    The radius of the circle.
 */
void draw_circle(SDL_Renderer *renderer, int x, int y, int radius);

/**
 * Draws a triangle on the renderer, given the circle and triangle positions.
 *
 * @param renderer      The SDL_Renderer to draw on.
 * @param circle_x      The x-coordinate of the circle's center.
 * @param circle_y      The y-coordinate of the circle's center.
 * @param circle_radius The radius of the circle.
 * @param triangle_x    The x-coordinate of the triangle's center.
 * @param triangle_y    The y-coordinate of the triangle's center.
 */
void draw_triangle(SDL_Renderer *renderer, int circle_x, int circle_y, int circle_radius, int triangle_x, int triangle_y);

/**
 * Draws angular sectors on the renderer.
 *
 * @param renderer    The SDL_Renderer to draw on.
 * @param x           The x-coordinate of the circle's center.
 * @param y           The y-coordinate of the circle's center.
 * @param inner_radius The inner radius of the circle.
 * @param num_sectors The number of angular sectors.
 */
void draw_angular_sectors(SDL_Renderer *renderer, int x, int y, int inner_radius, int num_sectors);



/**
 * Draws an obstacle on the renderer.
 *
 * @param renderer  The SDL_Renderer to draw on.
 * @param circle_x  The x-coordinate of the circle's center.
 * @param circle_y  The y-coordinate of the circle's center.
 * @param obstacle  A pointer to the ArcObstacle struct.
 */
void draw_obstacle(SDL_Renderer *renderer, int circle_x, int circle_y, ArcObstacle *obstacle);

/**
 * Spawns an obstacle.
 *
 * @param obstacles  An array of ArcObstacle structs.
 * @param sector     The sector in which the obstacle will be spawned.
 * @param circle_x   The x-coordinate of the circle's center.
 * @param circle_y   The y-coordinate of the circle's center.
 */
void spawn_obstacle(ArcObstacle *obstacles, int sector, int circle_x, int circle_y);

/**
 * Updates the state of an ArcObstacle based on its speed.
 *
 * @param obstacle     A pointer to an ArcObstacle structure representing the arc obstacle.
 * @param speed        The speed at which the obstacle should move.
 */
void update_obstacle(ArcObstacle *obstacle, float speed);

/**
 * Updates the triangle's position according to the provided angle.
 *
 * @param circle_x      The x-coordinate of the circle's center.
 * @param circle_y      The y-coordinate of the circle's center.
 * @param circle_radius The radius of the circle.
 * @param triangle_size The size of the triangle.
 * @param angle         The angle for the new position of the triangle.
 * @param triangle_x    A pointer to the x-coordinate of the triangle's center.
 * @param triangle_y    A pointer to the y-coordinate of the triangle's center.
 */
void update_triangle_position(int circle_x, int circle_y, int circle_radius, int triangle_size, float angle, int *triangle_x, int *triangle_y);

/**
 * Checks if a triangle is colliding with an arc obstacle.
 *
 * @param triangle_x   The x-coordinate of the center of the triangle.
 * @param triangle_y   The y-coordinate of the center of the triangle.
 * @param triangle_size The length of a side of the triangle.
 * @param circle_x     The x-coordinate of the center of the obstacle's bounding circle.
 * @param circle_y     The y-coordinate of the center of the obstacle's bounding circle.
 * @param obstacle     A pointer to an ArcObstacle structure representing the arc obstacle.
 * @return             True if the triangle is colliding with the obstacle, false otherwise.
 */
bool check_collision(int triangle_x, int triangle_y, int triangle_size, int circle_x, int circle_y, ArcObstacle *obstacle);

/**
 * Renders a timer on the screen.
 *
 * @param renderer     A pointer to the SDL_Renderer.
 * @param startTime    The start time in milliseconds for the timer.
 * @param fontTexture  A pointer to the SDL_Texture for the font.
 * @param charWidth    The width of a single character in the font texture.
 * @param charHeight   The height of a single character in the font texture.
 * @param charsPerRow  The number of characters per row in the font texture.
 * @param color        The color of the timer text.
 */
void render_timer(SDL_Renderer *renderer, Uint32 startTime, SDL_Texture *fontTexture, int charWidth, int charHeight, int charsPerRow, SDL_Color color);

/**
 *
 * Renders the game over screen using the provided SDL_Renderer and
 * game_over_texture.
 * @param renderer A pointer to the SDL_Renderer used for rendering the game over     * screen.
 * @param game_over_texture A pointer to the SDL_Texture containing the game over    * image.
 */
void render_game_over(SDL_Renderer *renderer, SDL_Texture *game_over_texture);

#endif // GAME_H
