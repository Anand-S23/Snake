#include <SDL.h>
#include <Primitive.h> 
#include <stdint.h>
#include <stdio.h>

#define global static 
#define internal static 

#define ROW 16
#define COL 16

enum Direction 
{
    Up, 
    Down, 
    Left, 
    Right
}; 

struct Body
{
    int x; 
    int y; 
};

struct Game_State
{
    bool running = true; 
    bool game_over = false; 
    int len = 1;
    Direction snake_direction = Left; 
    Body snake[ROW][COL];
};

global Game_State Game; 

internal void ResetGame(Game_State *game) 
{
    game->game_over = false;
}

internal void DrawSnake(SDL_Renderer *renderer, Game_State *game)
{
    Primitive_Color color = { 0, 255, 0, 255 };
};

internal void HandleEvent(SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_QUIT: 
        {
            Game.running = false; 
        };
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

                DrawSnake(renderer, &Game);

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