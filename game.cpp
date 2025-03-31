#include "game.hpp"

Game::Game()
: isRunning(true), score(0), bestScore(0)
{

    window = new RenderWindow("Snake Trun", WINDOW_WIDTH, WINDOW_HEIGHT);

    /* --------------------=LOADING FILE=--------------------- */
    font = window->loadTTF("./assets/atariclassic.ttf", 32);
    T_field = window->loadPNG("./assets/field.png");
    foodTexture = window->loadPNG("./assets/apple.png");

    headTexture = window->loadPNG("./assets/head.png");
    bodyTexture = window->loadPNG("./assets/body.png");
    tailTexture = bodyTexture;

    bestScoreTexture = window->loadPNG("./assets/bestscore.png");

    gameOverTexture = window->loadPNG("./assets/gameover.png");
    vignetteTexture = window->loadPNG("./assets/pausescreen.png");
    controlsTexture = window->loadPNG("./assets/controls.png");
    creditsTexture = window->loadPNG("./assets/credits.png");

    eatSound = window->loadWAV("./assets/eat.wav");
    hitSound = window->loadWAV("./assets/hit.wav"); 
	soundtrack = window->loadWAV("./assets/soundtrack.wav");

    window->setWindowIcon("./assets/icon.png");

    /* --------------------------------------------------------- */

    snake = new Snake( headTexture, bodyTexture, tailTexture );

    food = new Apple( foodTexture , BLOCK_SIZE*12, BLOCK_SIZE*7 ); 


    std::random_device randDev;
    randomGenerator = new std::mt19937(randDev());

    volume = 200;
    snake->step();
    overlayStack.push(overlayType::Controls);
}

// Trả về trạng thái trò chơi bắt đầu   
bool Game::isActive() const{
    return isRunning;
}

// xử lý các phím điều khiển
void Game::handleControl(){

    if ( keys[SDL_SCANCODE_RIGHT] ){
        if ( overlayStack.empty() ){
            snake->setDirection(1, 0);
        } else if ( overlayStack.top() == overlayType::Controls ){
            overlayStack.pop();
            snake->setDirection(1, 0);
        }
    }

    if ( keys[SDL_SCANCODE_LEFT] ){
        if ( overlayStack.empty() ){
            snake->setDirection(-1, 0);
        } else if ( overlayStack.top() == overlayType::Controls ){
            overlayStack.pop();
            snake->setDirection(-1, 0);
        }
    }

    if ( keys[SDL_SCANCODE_DOWN] ){
        if ( overlayStack.empty() ){
            snake->setDirection(0, 1);
        } else if ( overlayStack.top() == overlayType::Controls ){
            overlayStack.pop();
            snake->setDirection(0, 1);
        }
    }

    if ( keys[SDL_SCANCODE_UP] ){
        if ( overlayStack.empty() ){
            snake->setDirection(0, -1);
        } else if ( overlayStack.top() == overlayType::Controls ){
            overlayStack.pop();
            snake->setDirection(0, -1);
        }
    }
   
   if (!keys[SDL_SCANCODE_SPACE])canSwap = true;
 if (keys[SDL_SCANCODE_SPACE]) 
     if (canSwap) {
         snake->swapHeadTail();
         canSwap = false; 
 }

	if (!keys[SDL_SCANCODE_M]) canSwap1 = true;
 if (keys[SDL_SCANCODE_M])
     if (canSwap1) {
        if (Mix_Playing(-1)) 
         if (Mix_Paused(-1)) {Mix_Resume(-1);}
          else Mix_Pause(-1); 
         else{ Mix_VolumeChunk(soundtrack, MIX_MAX_VOLUME / 8); Mix_PlayChannel(-1, soundtrack, 0);}
        canSwap = false;
     }
}   

// xử lý các sự kiện 
void Game::handleEvents(){
	while (SDL_PollEvent(&event)){  
		switch(event.type){
		case SDL_QUIT:     // nếu bấm nút thoát
			    isRunning = false;
			    break;
            case SDL_KEYDOWN:
                switch ( event.key.keysym.sym ){
                case SDLK_ESCAPE:
                    if ( overlayStack.empty() ){
                        overlayStack.push( overlayType::PauseScreen );
                    } else if ( overlayStack.top() == overlayType::PauseScreen  ) {    
                        overlayStack.pop();
                    }
                    break;

                case SDLK_F1:
                    if ( overlayStack.empty() || overlayStack.top() != overlayType::Credits ){
                        overlayStack.push( overlayType::Credits );
                    } else if ( overlayStack.top() == overlayType::Credits ){
                        overlayStack.pop();
                    }
                    break;
                
                case SDLK_SPACE:
                    if ( !overlayStack.empty() && overlayStack.top() == overlayType::GameOver ){
                        reset();
                        snake->step();
                        overlayStack.pop();
                        overlayStack.push( overlayType::Controls );
                    }
                    break; 
                }
			case SDL_MOUSEBUTTONDOWN: // nếu bấm chuột thoát    
                switch ( event.button.button ){
                case SDL_BUTTON_LEFT:
                    if ( !overlayStack.empty() && overlayStack.top() == overlayType::GameOver ){
                        reset();
                        snake->step();
                        overlayStack.pop();
                        overlayStack.push( overlayType::Controls );
                    }
                    break;
                }
        }
    }
}

// Đặt lại trò chơi 
void Game::reset(){ 
    food->position = food->initPosition;
    snake->reset();
    score = 0;
}

// Xuất hiện thức ăn ngẫu nhiên
void Game::respawnFood(){

    std::uniform_int_distribution<> generateX(BLOCK_SIZE, WINDOW_WIDTH-BLOCK_SIZE*2);
    std::uniform_int_distribution<> generateY(BLOCK_SIZE, WINDOW_HEIGHT-BLOCK_SIZE*2);

    // random x and y
    float random_x = ( generateX(*randomGenerator) / BLOCK_SIZE ) * BLOCK_SIZE;
    float random_y = ( generateY(*randomGenerator) / BLOCK_SIZE ) * BLOCK_SIZE;

    // random position
    food->position = {random_x, random_y};
}

// Kiểm tra va chạm và một số logic
void Game::checkCollision(){
/*-----------Ý tưởng va chạm-------------
    rect1.x < rect2.x + rect2.width &&
    rect1.x + rect1.width > rect2.x &&
    rect1.y < rect2.y + rect2.height &&
    rect1.y + rect1.height > rect2.y
-------------------------------------------*/

    for (auto segment : snake->getSegments() ){
        if ( snake->getHead() != segment ){
            
			// Nếu đầu rắn va chạm với thân rắn
            if ( snake->getHead()->position == segment->position ){
                Mix_PlayChannel( -1, hitSound, 0 ); // Phát âm thanh hit
                overlayStack.push( overlayType::GameOver ); 

            // nếu thức ăn sinh ra trong rắn
            } else if ( segment->position == food->position){
                respawnFood();
            }
        }
    }

    // Kiểm tra sự va chạm với đường viền
    if ( snake->getHead()->position.x > (WINDOW_WIDTH - BLOCK_SIZE*2) ||
         snake->getHead()->position.x < BLOCK_SIZE ||
         snake->getHead()->position.y > (WINDOW_HEIGHT - BLOCK_SIZE*2) ||
         snake->getHead()->position.y < BLOCK_SIZE
         ){
            Mix_PlayChannel( -1, hitSound, 0 ); // Phát âm thanh hit
            overlayStack.push( overlayType::GameOver ); 
    }   

    // Kiểm tra va chạm rắn với táo
    if ( snake->getHead()->position == food->position ){     
        snake->addScore();

        score++;
        if ( score >= bestScore ){
            bestScore = score;
        }

        Mix_PlayChannel( -1, eatSound, 0 ); // Phát âm thanh ăn
        respawnFood();
    }    
}

// Cập nhật tất cả các đối tượng trò chơi
void Game::update(){
   
    window->clear();

    handleEvents();

    handleControl();

    Mix_Volume(-1, volume); 
   
    drawUI();
    drawSnake();

    if ( overlayStack.empty() )
   {
        snake->step();
        checkCollision();
    } else if ( overlayStack.top() == overlayType::GameOver ) {
        window->render(vignetteTexture, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, 0.f); // Lớp phủ tối
        window->render(gameOverTexture, 403, 235, 279, 184); // Lớp phủ menu
        window->render(food->texture, 64, 64, 320, 244, 0); // Biểu tượng táo
        window->render(font, getScore(score), {233, 249, 217}, 90, BLOCK_SIZE, 384, 256); // Văn bản điểm hiện tại
        window->render(bestScoreTexture, 64, 64, 480, 244); // Biểu tưởng cúp   
        window->render(font, getScore(bestScore), {233, 249, 217}, 90, BLOCK_SIZE, 480+64, 256); // Văn bản điểm cao nhất   
    } else if ( overlayStack.top() == overlayType::PauseScreen ){
        window->render(vignetteTexture, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, 0.f); // Lớp phủ tối
    } else if ( overlayStack.top() == overlayType::Controls ){
        window->render(controlsTexture, 256, 163, WINDOW_WIDTH/3, BLOCK_SIZE*3);
    } else if ( overlayStack.top() == overlayType::Credits ){
        window->render(vignetteTexture, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, 0.f); // Lớp phủ tối
        window->render(creditsTexture, 647, 383, 148, 63);
    }
    
    window->update();
}

// Chuỗi điểm được định dạng tùy vào số điểm    
std::string Game::getScore(int score) const{
    if ( score == 0 ){
        return "000";
    } else if (  score <= 9 ){
        return "00" + std::to_string( score );
    } else if ( score <= 99 ){ 
        return "0" + std::to_string( score );
    } else {
        return std::to_string( score );
    }
}

// vẽ rắn và táo
void Game::drawSnake(){
    //               texture,               width,        height,       position
    window->render(food->texture, BLOCK_SIZE, BLOCK_SIZE, food->position.x, food->position.y);

    // render body
    for ( const auto segment : snake->getSegments() ){
        if ( segment != snake->getHead() ){
            //               texture,          width,      height,     position,                                 angle
            window->render(segment->texture, BLOCK_SIZE, BLOCK_SIZE, segment->position.x, segment->position.y, segment->angle);
        }
    }

    // render head
    window->render(snake->getHead()->texture, BLOCK_SIZE, BLOCK_SIZE, snake->getHead()->position.x, snake->getHead()->position.y, snake->getHead()->angle);
}

void Game::drawUI(){
    window->render(T_field, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, 0.f);
    window->render(food->texture, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, 0 );
    window->render(font, getScore(score), {75, 105, 47}, 90, BLOCK_SIZE, BLOCK_SIZE*2, 0);

    window->render(bestScoreTexture, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE*4, 0);
    window->render(font, getScore(bestScore), {75, 105, 47}, 90, BLOCK_SIZE, BLOCK_SIZE*5, 0);
}

// destructor
Game::~Game(){

    SDL_DestroyTexture( T_field );
    SDL_DestroyTexture( foodTexture );
    SDL_DestroyTexture( bestScoreTexture );
    SDL_DestroyTexture( gameOverTexture );
    SDL_DestroyTexture( vignetteTexture );
    SDL_DestroyTexture( controlsTexture );
    SDL_DestroyTexture( creditsTexture );

    SDL_DestroyTexture( headTexture );
    SDL_DestroyTexture( bodyTexture );
    SDL_DestroyTexture( tailTexture );

    Mix_FreeChunk( eatSound );
    Mix_FreeChunk( hitSound );  
	Mix_FreeChunk( soundtrack);

    TTF_CloseFont(font);

    delete window;
    delete snake;
    delete food; 
    delete randomGenerator;

    IMG_Quit();
    SDL_Quit();
}
