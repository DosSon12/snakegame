#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <random>
#include <string>
#include <stack>
#include <queue>
#include "renderwindow.hpp"
#include "snake.hpp"
#include "food.hpp"
#include "config.h"

class Game{
private:
    bool            isRunning;
    int             score;
    int             bestScore;
    bool canSwap = true;
    bool canSwap1 = true;
    unsigned int    volume;

    Snake*          snake;
    Apple*          food;
    RenderWindow*   window;

    const Uint8     *keys = SDL_GetKeyboardState(NULL);

    enum            overlayType{
        PauseScreen,
        Credits,
        Controls,
        GameOver
    };

    std::stack<overlayType> overlayStack;

    SDL_Event       event;

    // random 
    std::mt19937*   randomGenerator;

    
    SDL_Texture*    T_field;
    SDL_Texture*    foodTexture;
    SDL_Texture*    bestScoreTexture;
    SDL_Texture*    gameOverTexture;
    SDL_Texture*    vignetteTexture;
    SDL_Texture*    controlsTexture;
    SDL_Texture*    creditsTexture;

    
    Mix_Chunk*      eatSound;
    Mix_Chunk*      hitSound;
    Mix_Chunk*      soundtrack;
    
    SDL_Texture*    headTexture;
    SDL_Texture*    bodyTexture;
    SDL_Texture*    tailTexture; 

    TTF_Font*       font;

    void            checkCollision();    
    void            handleEvents();
    void            handleControl();

    void            drawSnake();    
    void            drawUI();
    void            reset();

    std::string     getScore(int score) const;

    void            respawnFood();

public:
    Game();
   
    bool            isActive() const;
    void            update();

    ~Game();
};
