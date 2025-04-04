﻿/* Screen---chịu trách nhiệm hiển thị cửa sổ, hiển thị mọi thứ và tải kết cấu*/

#include "renderwindow.hpp"

RenderWindow::RenderWindow(std::string title, int screen_width, int screen_lenght)
: _window(nullptr), _renderer(nullptr)
{
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_lenght, SDL_WINDOW_SHOWN);
	if (_window == nullptr){
		std::cout << "Failed to create window. Error: " << SDL_GetError() << std::endl;
	}

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == nullptr){
        std::cout << "Failed to create renderer. Error: " << SDL_GetError() << std::endl;
    }
}

// nhận đường dẫn đến tệp PNG và không trả về gì
void RenderWindow::setWindowIcon(std::string path){
    SDL_Surface* icon = IMG_Load( path.c_str() );
    SDL_SetWindowIcon(_window, icon);
    SDL_FreeSurface(icon); // ?
}

// nhận đường dẫn đến tệp png và trả về SDL_Texture*
SDL_Texture* RenderWindow::loadPNG(std::string path){
    SDL_Texture* pNewTexture = nullptr;

	pNewTexture = IMG_LoadTexture( _renderer, path.c_str() );

	if ( pNewTexture == nullptr )
		std::cout << "Failed to load texture. Error: "<< SDL_GetError() << std::endl;

	return pNewTexture;
}

// nhận đường dẫn đến tệp ttf và trả về TTF_Font*
TTF_Font* RenderWindow::loadTTF(std::string path, int size){

    TTF_Font* pNewFont = TTF_OpenFont(path.c_str(), size);

    if ( !pNewFont ){
        std::cout << "Failed to load font. Error: " << SDL_GetError() << std::endl;
    }

    return pNewFont;
}

// nhận đường dẫn đến tệp wav và trả về Mix_Chunk*
Mix_Chunk* RenderWindow::loadWAV(std::string path){
    Mix_Chunk* pNewSound = Mix_LoadWAV( path.c_str() );

    if ( !pNewSound ){
        std::cout << "Failed to load wav. Error: " << SDL_GetError() << std::endl;
    }

    return pNewSound;
}

// Render sprite
void RenderWindow::render(SDL_Texture* texture, int w, int h, float x, float y, float angle){
	SDL_Rect src; 
	src.x = 0;
	src.y = 0;


    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w; 
	dst.h = h;

    SDL_RenderCopyEx( _renderer, texture, &src, &dst, angle, NULL, SDL_FLIP_NONE);
}

// Render  text
void RenderWindow::render(TTF_Font* font, std::string text, SDL_Color color, int w, int h, float x, float y){

    // Tạo văn bản bề mặt
    SDL_Surface* pSurfaceText = TTF_RenderText_Solid(font, text.c_str(), color); 
    // Chuyển đổi bề mặt thành kết cấu
    SDL_Texture* pText = SDL_CreateTextureFromSurface(_renderer, pSurfaceText);

    SDL_Rect rect; //Tạo một chữ nhật
    rect.x = x;  
    rect.y = y; 
    rect.w = w; 
    rect.h = h; 

    SDL_RenderCopy(_renderer, pText, NULL, &rect); 

    SDL_FreeSurface(pSurfaceText); 
    SDL_DestroyTexture(pText); 
}

void RenderWindow::clear(){
    SDL_RenderClear( _renderer );
}

void RenderWindow::update(){
    SDL_RenderPresent( _renderer );
}

RenderWindow::~RenderWindow(){
    SDL_DestroyRenderer( _renderer );
    SDL_DestroyWindow( _window );
}