#include <SDL.h>
#include <Primitive.h> 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

enum State 
{
    Start, 
    Play, 
    Game_Over
};

struct Game_State
{
    bool running = true; 
    int len = 1;
    State state; 
    Direction snake_direction; 
    Entity snake[576];
    Entity food;
    bool key_down = false; 
};

global Game_State Game; 

internal Entity GenerateFood()
{
    int food_x = rand() % ROW_COL_SIZE;
    int food_y = rand() % ROW_COL_SIZE;

    return { food_x, food_y };
}

internal void ResetGame(Game_State *game) 
{
    game->len = 2; 
    game->state = Play;
    game->snake_direction = Right;
    game->snake[1] = { 1, 1 };
    game->food = GenerateFood();
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
    // Check Edges
    if (game->snake[game->len - 1].x >= ROW_COL_SIZE ||
        game->snake[game->len - 1].x < 0 ||
        game->snake[game->len - 1].y >= ROW_COL_SIZE ||
        game->snake[game->len - 1].y < 0)

    {
        ResetGame(game);
    }

    // Check if head colliding with body
    for (int i = 0; i < game->len - 1; ++i)
    {
        if (game->snake[i].x == game->snake[game->len - 1].x &&
            game->snake[i].y == game->snake[game->len - 1].y)
        {
            ResetGame(game);
        }
        
        break;
    }

    // Check for food
    if (game->snake[game->len - 1].x == game->food.x && 
        game->snake[game->len - 1].y == game->food.y)
    {
        ++game->len; 
        int current_x = game->snake[game->len - 2].x;
        int current_y = game->snake[game->len - 2].y;

        switch (game->snake_direction)
        {
            case Up:
            {
                ++game->snake[game->len - 2].y;
            } break; 

            case Down: 
            {
                --game->snake[game->len - 2].y;
            } break; 

            case Left: 
            {
                ++game->snake[game->len - 2].x;
            } break; 

            case Right: 
            {
                --game->snake[game->len - 2].x;
            } break; 
        }

        game->snake[game->len - 1] = { current_x, current_y };
        game->food = GenerateFood();
    }
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
            game->snake[game->len - 1].x -= 1;
        } break;

        case Right: 
        {
            game->snake[game->len - 1].x += 1;
        } break;

        default:
        {
        } break;
    }

    CheckCollisions(game);

    // Moves the snake so the part of the snake was in the postion of the one after
    for (int i = 1; i < game->len; ++i)
    {
        game->snake[i - 1] = game->snake[i];
    }

    SDL_Delay(150);
}

internal void HandleEvent(SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_QUIT: 
        {
            Game.running = false; 
        } break;

        case SDL_KEYDOWN: 
        {
            if (!Game.key_down)
            {
                switch (event->key.keysym.scancode)
                {
                    case SDL_SCANCODE_UP: 
                    case SDL_SCANCODE_W: 
                    {
                        if (Game.snake_direction != Down)
                        {
                            Game.snake_direction = Up;
                        }
                    } break;

                    case SDL_SCANCODE_DOWN: 
                    case SDL_SCANCODE_S:
                    {
                        if (Game.snake_direction != Up)
                        {
                            Game.snake_direction = Down;
                        }
                    } break; 

                    case SDL_SCANCODE_LEFT: 
                    case SDL_SCANCODE_A:
                    {
                        if (Game.snake_direction != Right)
                        {
                            Game.snake_direction = Left; 
                        }
                    } break; 

                    case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_D:
                    {
                        if (Game.snake_direction != Left)
                        {
                            Game.snake_direction = Right;
                        }
                    } break;
                }
            } break;
            Game.key_down = true; 
        } break; 

        case SDL_KEYUP: 
        {
            Game.key_down = false;
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
            srand(time(NULL));

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

                switch(Game.state)
                {
                    case Start:
                    {
                        DrawBoard(renderer, &Game);
                    } break; 

                    case Play: 
                    {
                        Move(&Game);
                        DrawBoard(renderer, &Game);
                    } break;

                    case Game_Over: 
                    {
                    } break; 
                }

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