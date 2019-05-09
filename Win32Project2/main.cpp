#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>
#include <ctime>
#include <string.h>
#include <math.h>

#include "life.h"


const uint32_t WIN_W = 900;
const uint32_t WIN_H = 660;


SDL_Window* window = nullptr;
SDL_Window* w2 = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Renderer* renderer2 = nullptr;
TTF_Font* font;



int main(int argc, char** args) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    atexit(SDL_Quit);

    window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }
 
    w2 = SDL_CreateWindow("menu", 200, 200 , 200, 200, SDL_WINDOW_SHOWN);
    renderer2 = SDL_CreateRenderer(w2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    font = TTF_OpenFont("C:\\dev\\Roboto-Regular.ttf", 20);

    bool quit = false;
    SDL_Event event;


    Life game(300, 220, 4);
    char cell;

    SDL_Rect r = { 0, 0, 2, 2 };
    char stepDurationBuffer[5];
    SDL_Rect fps_rect = { 0, 0, 50, 200 };
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* surf;
    SDL_Texture* texture;
    auto last_tick = std::chrono::high_resolution_clock::now();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
            }
        }

        //int start = SDL_GetTicks();

        game.step();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        auto drawStart = std::chrono::high_resolution_clock::now();
        int x, y;
        for (x = 0; x < game.mapWidth; x++) {
            r.x = x * 3;
            r.y = 0;
            for (y = 0; y < game.mapHeight; y++) {
                //cell = game.map[x][y];

                //if (cell != game.newMap[x][y]) {
                if (game.map[x][y]) {
                    SDL_RenderFillRect(renderer, &r);
                }

                r.y += 3;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        unsigned long drawDuration = std::chrono::duration_cast<std::chrono::microseconds>(now - drawStart).count();
        SDL_RenderPresent(renderer);
        _itoa(drawDuration, stepDurationBuffer, 10);
        std::string fpss = "draw: ";
        fpss.append(stepDurationBuffer);
        surf = TTF_RenderText_Solid(font, fpss.c_str(), textColor);
        fps_rect.y = 25;
        fps_rect.w = surf->w;
        fps_rect.h = surf->h;
        texture = SDL_CreateTextureFromSurface(renderer2, surf);
        SDL_RenderClear(renderer2);
        SDL_RenderCopy(renderer2, texture, nullptr, &fps_rect);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(texture);


        _itoa(game.durationStep, stepDurationBuffer, 10);
        fpss = "step:  ";
        fpss.append(stepDurationBuffer);
        surf = TTF_RenderText_Solid(font, fpss.c_str(), textColor);
        fps_rect.y = 50;
        fps_rect.w = surf->w;
        fps_rect.h = surf->h;
        texture = SDL_CreateTextureFromSurface(renderer2, surf);
        SDL_RenderCopy(renderer2, texture, nullptr, &fps_rect);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(texture);

        now = std::chrono::high_resolution_clock::now();
        _itoa(std::chrono::duration_cast<std::chrono::microseconds>(now - last_tick).count(), stepDurationBuffer, 10);
        last_tick = now;
        fpss = "fps:    ";
        fpss.append(stepDurationBuffer);
        surf = TTF_RenderText_Solid(font, fpss.c_str(), textColor);
        fps_rect.y = 0;
        fps_rect.w = surf->w;
        fps_rect.h = surf->h;
        texture = SDL_CreateTextureFromSurface(renderer2, surf);
        SDL_RenderCopy(renderer2, texture, nullptr, &fps_rect);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(texture);
        SDL_RenderPresent(renderer2);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window);
    SDL_DestroyWindow(w2);
    SDL_Quit();

    return 0;
}


void init() {

}

