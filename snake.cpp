#include <SDL.h>
#include <Primitive.h> 
#include <stdint.h>
#include <stdio.h>

#define global static 
#define internal static 

#define SNAKE_SIZE 20
#define ROW_COL_SIZE (480 / SNAKE_SIZE)

enum Direction 
{
    Up, 
    Down, 
    Left, 
    Right
}; 

struct Entity
{
    int x; 
    int y; 
};

struct Game_State
{
    bool running = true; 
    int len = 1;
    Direction snake_direction = Left; 
    Entity snake[512];
    Entity food;
};

global Game_State Game; 

internal void ResetGame(Game_State *game) 
{
    game->len = 1; 
    game->snake_direction = Left;
    game->snake[0] = { 1, 1 };
    game->food = { ROW_COL_SIZE - 2, 1 };
}

internal void DrawBoard(SDL_Renderer *renderer, Game_State *game)
{
    Primitive_Color green = { 0, 255, 0, 255 };
    Primitive_Color red   = { 255, 0, 0, 255 };

    // Draw Food 
    Primitive_DrawFilledRect(renderer, game->food.x * SNAKE_SIZE, game->food.y *SNAKE_SIZE,
                             SNAKE_SIZE, SNAKE_SIZE, red);

    // Draw Snake
    for (int i = 0; i < game->len; ++i)
    {
        Primitive_DrawFilledRect(renderer, 
                                 game->snake[i].x * SNAKE_SIZE, game->snake[i].y * SNAKE_SIZE, 
                                SNAKE_SIZE, SNAKE_SIZE, green);
    }
}

internal void CheckCollisions(Game_State *game)
{
}

internal void Move(Game_State *game)
{
    switch (game->snake_direction)
    {
        case Up: 
        {
            game->snake[game->len - 1].y -= 1;
        } break;

        case Down: 
        {
            game->snake[game->len - 1].y += 1;
        } break;

        case Left: 
        {
            game->snake[game->len - 1].x += 1;
        } break;

        case Right: 
        {
            game->snake[game->len - 1].x -= 1;
        } break;
    }

    // Moves the snake so the part of the snake was in the postion of the one after
    for (int i = 1; i < game->len; ++i)
    {
        game->snake[i - 1] = game->snake[i];
    }

    SDL_Delay(250);
}

internal void GenerateFood(Game_State *game)
{
}

internal void HandleEvent(SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_QUIT: 
        {
            Game.running = false; 
        } break;
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Snake",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          480, 480, 0);

    if (window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer)
        {
            uint32_t last_tick_time = 0;
            uint32_t delta = 0;

            ResetGame(&Game);

            while(Game.running)
            {
                uint32_t tick_time = SDL_GetTicks();
                delta = tick_time - last_tick_time;
                last_tick_time = tick_time;

                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                    HandleEvent(&event);
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                DrawBoard(renderer, &Game);
                Move(&Game);

                SDL_RenderPresent(renderer);
            }
        }
        else 
        {
            printf("Could not create window: %s\n", SDL_GetError());
            return 1;
        }
    }
    else 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Quit(); 
    return 0;
}