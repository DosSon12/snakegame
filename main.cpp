﻿#include "game.hpp"

bool init() {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL failed to init. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cerr << "SDL_image failed to init. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf failed to init. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer failed to init. Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {

    if (!init()) {
        std::cerr << "Exiting..." << std::endl;
        return 1;
    }

    // Thời gian khung hình tối đa
    const int       FRAME_DELAY = 1000 / FPS;
    Uint32          frameStart;
    int             deltaTime;
    
    Game* game = new Game();
    while (game->isActive()) {
        frameStart = SDL_GetTicks(); // Nhận dấu tích khung bắt đầu

        game->update();

        deltaTime = SDL_GetTicks() - frameStart; // Nhận thời gian khung hình
        if (FRAME_DELAY > deltaTime) {
            SDL_Delay(FRAME_DELAY - deltaTime);
        }
    }

    delete game;

    return 0;
}