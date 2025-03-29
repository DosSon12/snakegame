#pragma once
#include <SDL.h>
#include "vector2f.h"
#include "snakesegment.h"
#include <vector>
#include "config.h"
#include <algorithm>
#include <map>     

#include <list>

class Snake {
private:
    SDL_Texture*    _bodyTexture;
    SDL_Texture*    _headTexture;
    SDL_Texture*    _tailTexture;

    float           _speed;
    // thụt lề bằng pixel giữa các phân đoạn
    int             _segmentIndent;
    // có bao nhiêu phân đoạn được thêm vào cho mỗi điểm
    int             _segmentsPerScore;

    int             _initSegments;
    Vector2f        _initPosition;
    Vector2f        _initDirection;
    Vector2f        _direction;    
    void            addSegment(Vector2f position, Vector2f direction);
    void            removeSegment();
    void            updateTextures();   
  
    std::list<SnakeSegment*> _segments;  // Danh sách các đoạn thân rắn
                 
public:    
    Snake(SDL_Texture* headTexture, SDL_Texture* bodyTexture, SDL_Texture* tailTexure);

    void            step();
    void            setDirection(float x, float y);

    Vector2f        getPosition() const;

    std::list<SnakeSegment*>   getSegments();
    
    SnakeSegment*   getHead();

    void            swapHeadTail();
    void            addScore();
    void            reset();

    ~Snake();
};