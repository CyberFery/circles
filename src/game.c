#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "game.h"

struct game *game_initialize(SDL_Renderer *renderer) {
    struct game *game;
    game = malloc(sizeof(struct game));
    game->renderer = renderer;
    game->state = GAME_ON;
    game->difficulty = EASY;
    return game;
}

void game_run(struct game *game){
    SDL_Surface *fontSurface = IMG_Load(TIMER_FILENAME);
    SDL_Texture *fontTexture = SDL_CreateTextureFromSurface(game->renderer, fontSurface);
    SDL_FreeSurface(fontSurface);

    SDL_Surface *imgSurface = IMG_Load(G_OVER_FILENAME);
    SDL_Texture *imgTexture = SDL_CreateTextureFromSurface(game->renderer, imgSurface);
    SDL_FreeSurface(imgSurface);

    int charWidth = 32; // Width of a character in the bitmap font image
    int charHeight = 32; // Height of a character in the bitmap font image
    int charsPerRow = 11; // Number of characters per row in the bitmap font image
    Uint32 startTime = SDL_GetTicks();
    SDL_Event event;
    SDL_Color timerColor = {0, 183, 255,255};
    int circle_radius = 45;
    int circle_x = SCREEN_WIDTH / 2;
    int circle_y = SCREEN_HEIGHT / 2;
    int triangle_size = 15;
    int triangle_x;
    int triangle_y;
    float angle = 0;
    update_triangle_position(circle_x, circle_y, circle_radius, triangle_size, angle, &triangle_x, &triangle_y);

    srand(time(NULL));

    ArcObstacle obstacles[NUM_SECTORS] = {0};
    for (int i = 0; i < NUM_SECTORS; i++) {
        spawn_obstacle(obstacles, i, circle_x, circle_y);
    }

    while (game->state == GAME_ON) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game->state=GAME_OVER;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    angle += ANGLE_STEP;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    angle -= ANGLE_STEP;
                }
                update_triangle_position(circle_x, circle_y, circle_radius, triangle_size, angle, &triangle_x, &triangle_y);
            }
        }


        SDL_RenderClear(game->renderer);

        draw_angular_sectors(game->renderer, circle_x, circle_y, circle_radius, NUM_SECTORS);
        draw_circle(game->renderer, circle_x, circle_y, circle_radius);
        draw_triangle(game->renderer, circle_x, circle_y, circle_radius, triangle_x, triangle_y);

        for (int i = 0; i < NUM_SECTORS; i++) {
            switch(game->difficulty){
                case EASY:
                    update_obstacle(&obstacles[i], 2.0f);
                    break;
                case MEDIUM:
                    update_obstacle(&obstacles[i], 4.0f);
                    break;
                case HARD:
                    update_obstacle(&obstacles[i], 6.0f);
                    break;
            }

            if (obstacles[i].active) {
                draw_obstacle(game->renderer, circle_x, circle_y, &obstacles[i]);
            }

            if (obstacles[i].outer_radius <= circle_radius) {
                // Obstacle collided with the circle, destroy the obstacle
                obstacles[i].active = false;
                spawn_obstacle(obstacles, i, circle_x, circle_y);
            } else if (check_collision(triangle_x, triangle_y, triangle_size, circle_x, circle_y, &obstacles[i])) {
                // Obstacle collided with the triangle, stop the game
                game->state=GAME_OVER;
                break;
            }
        }


        render_timer(game->renderer, startTime, fontTexture, charWidth, charHeight, charsPerRow, timerColor);

        SDL_RenderPresent(game->renderer);
    }


    render_game_over(game->renderer, imgTexture);
    SDL_RenderPresent(game->renderer);


    SDL_Delay(2000);

}

void game_delete(struct game *game) {
    if (game != NULL) {
        free(game);
    }
}

void draw_circle(SDL_Renderer *renderer, int x, int y, int radius) {
    filledCircleRGBA(renderer, x, y, radius, 58, 58, 58, 255);
}


void draw_triangle(SDL_Renderer *renderer, int circle_x, int circle_y, int circle_radius, int triangle_x, int triangle_y) {
    Sint16 vx[3], vy[3];

    int vertex1_x = circle_x + circle_radius * cosf(atan2f(triangle_y - circle_y, triangle_x - circle_x) - M_PI / 8);
    int vertex1_y = circle_y + circle_radius * sinf(atan2f(triangle_y - circle_y, triangle_x - circle_x) - M_PI / 8);
    int vertex2_x = circle_x + circle_radius * cosf(atan2f(triangle_y - circle_y, triangle_x - circle_x) + M_PI / 8);
    int vertex2_y = circle_y + circle_radius * sinf(atan2f(triangle_y - circle_y, triangle_x - circle_x) + M_PI / 8);

    vx[0] = vertex1_x;
    vy[0] = vertex1_y;
    vx[1] = triangle_x;
    vy[1] = triangle_y;
    vx[2] = vertex2_x;
    vy[2] = vertex2_y;

    filledPolygonRGBA(renderer, vx, vy, 3, 0, 183, 255, 255);
}


void draw_angular_sectors(SDL_Renderer *renderer, int x, int y, int inner_radius, int num_sectors) {
    float angle_step = 2 * M_PI / num_sectors;
    int outer_radius = (int)sqrt(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT);

    for (int i = 0; i < num_sectors; i++) {
        float start_angle = i * angle_step;
        float end_angle = (i + 1) * angle_step;
        Uint8 bg_r = (i % 2 == 0) ? 176 : 224;
        Uint8 bg_g = (i % 2 == 0) ? 176 : 224;
        Uint8 bg_b = (i % 2 == 0) ? 176 : 224;

        filledPieRGBA(renderer, x, y, outer_radius, (int)(start_angle * 180 / M_PI), (int)(end_angle * 180 / M_PI), bg_r, bg_g, bg_b, 255);
        filledCircleRGBA(renderer, x, y, inner_radius, 255, 255, 255, 255);
    }
}

void update_triangle_position(int circle_x, int circle_y, int circle_radius, int triangle_size, float angle, int *triangle_x, int *triangle_y) {
    *triangle_x = circle_x + (circle_radius + triangle_size) * cosf(angle);
    *triangle_y = circle_y + (circle_radius + triangle_size) * sinf(angle);
}


void draw_obstacle(SDL_Renderer *renderer, int circle_x, int circle_y, ArcObstacle *obstacle) {
    float angle_step = 2 * M_PI / NUM_SECTORS;
    float start_angle = obstacle->sector * angle_step;
    float end_angle = (obstacle->sector + 1) * angle_step;

    int thickness = 6;

    for (int r = obstacle->inner_radius; r < obstacle->outer_radius; r += thickness) {
        for (float a = start_angle; a <= end_angle; a += 0.01) {
            int x1 = circle_x + r * cos(a);
            int y1 = circle_y - r * sin(a);
            int x2 = circle_x + (r + thickness) * cos(a);
            int y2 = circle_y - (r + thickness) * sin(a);


            thickLineRGBA(renderer, x1, y1, x2, y2, thickness, 255, 0, 102, 255);
        }
    }
}


void spawn_obstacle(ArcObstacle *obstacles, int sector, int circle_x, int circle_y) {
    ArcObstacle *obstacle = &obstacles[sector];
    obstacle->active = false;
    obstacle->distance = sqrtf(powf(circle_x, 2) + powf(circle_y, 2));
    obstacle->sector = sector;
    obstacle->inner_radius = obstacle->distance - 10;
    obstacle->outer_radius = obstacle->distance;
    obstacle->spawn_delay = rand() % 151 + 50;
}

void update_obstacle(ArcObstacle *obstacle, float speed) {
    if (!obstacle->active) {
        obstacle->spawn_delay--;
        if (obstacle->spawn_delay == 0) {
            obstacle->active = true;
        }
        return;
    }

    obstacle->distance -= speed;
    obstacle->inner_radius = obstacle->distance - 3;
    obstacle->outer_radius = obstacle->distance;
}
bool check_collision(int triangle_x, int triangle_y, int triangle_size, int circle_x, int circle_y, ArcObstacle *obstacle) {
    float angle_step = 2 * M_PI / NUM_SECTORS;
    float start_angle = obstacle->sector * angle_step;
    float end_angle = (obstacle->sector + 1) * angle_step;

    for (int i = 0; i < 3; i++) {
        float angle = i == 0 ? 0.0f : (i == 1 ? 2 * M_PI / 3 : 4 * M_PI / 3);
        int vertex_x = triangle_x + triangle_size * cos(angle);
        int vertex_y = triangle_y - triangle_size * sin(angle);

        float dist_x = vertex_x - circle_x;
        float dist_y = vertex_y - circle_y;
        float distance_from_center = sqrt(dist_x * dist_x + dist_y * dist_y);

        float vertex_angle = atan2(-dist_y, dist_x);
        if (vertex_angle < 0) {
            vertex_angle += 2 * M_PI;
        }

        if (distance_from_center >= obstacle->inner_radius && distance_from_center <= obstacle->outer_radius && vertex_angle >= start_angle && vertex_angle <= end_angle) {
            return true;
        }
    }

    return false;
}

void render_timer(SDL_Renderer *renderer, Uint32 startTime, SDL_Texture *fontTexture, int charWidth, int charHeight, int charsPerRow, SDL_Color color) {
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    int seconds = elapsedTime / 1000;
    int milliseconds = elapsedTime % 1000;

    char timeText[64];
    snprintf(timeText, sizeof(timeText), "%d.%03d", seconds, milliseconds);

    int textWidth = strlen(timeText) * charWidth;

    int xPos = SCREEN_WIDTH - textWidth - 10;
    int yPos = 10;

    SDL_SetTextureColorMod(fontTexture, color.r, color.g, color.b);

    for (char *c = timeText; *c != '\0'; ++c) {
        int index = *c == '.' ? 10 : *c - '0';
        SDL_Rect srcRect = {
            .x = (index % charsPerRow) * charWidth,
            .y = (index / charsPerRow) * charHeight,
            .w = charWidth,
            .h = charHeight,
        };
        SDL_Rect destRect = {
            .x = xPos,
            .y = yPos,
            .w = charWidth,
            .h = charHeight,
        };

        SDL_RenderCopy(renderer, fontTexture, &srcRect, &destRect);

        xPos += charWidth;
    }


    SDL_SetTextureColorMod(fontTexture, 255, 255, 255);
}

// img src: https://www.pngmart.com/image/287289
void render_game_over(SDL_Renderer *renderer, SDL_Texture *game_over_texture) {
    int texture_width, texture_height;
    SDL_QueryTexture(game_over_texture, NULL, NULL, &texture_width, &texture_height);

    SDL_Rect dest_rect = {
        .x = (SCREEN_WIDTH - texture_width) / 2,
        .y = (SCREEN_HEIGHT - texture_height) / 2,
        .w = texture_width,
        .h = texture_height,
    };

    SDL_RenderCopy(renderer, game_over_texture, NULL, &dest_rect);
}
