#include "snake.hpp"
/* --------------------------------------=Chỉnh số của snake =----------------------------------- */
Snake::Snake(SDL_Texture* headTexture, SDL_Texture* bodyTexture, SDL_Texture* tailTexure)
: _bodyTexture(bodyTexture), _headTexture(headTexture),
 _tailTexture(tailTexure)
{   
    _segmentIndent = 4;
    _segmentsPerScore = 6;
    _speed = 4.f;

    _initSegments = _segmentsPerScore * 3;
    _initDirection = {1, 0};
    _initPosition = {288, 336};

    reset();

}
/* ---------------------------------------------------------------------------------------------- */
// Cập nhật vị trí rắn 
void Snake::step() {
    
    for (auto iter = _segments.begin(); iter != _segments.end(); iter++) {
        SnakeSegment* segment = *iter;

       
        if (int(segment->position.x) % _segmentIndent == 0 &&
            int(segment->position.y) % _segmentIndent == 0) {
            if (segment != _segments.back()) {
                (*std::next(iter))->buffdirection = segment->direction;
            }

            
            if (int(segment->position.x) % BLOCK_SIZE == 0 &&
                int(segment->position.y) % BLOCK_SIZE == 0) {
                segment->direction = segment->buffdirection;
            }
        }
    }
    for (auto segment : _segments) {
        segment->position.x += (_speed * segment->direction.x);
        segment->position.y += (_speed * segment->direction.y);
    }

    updateTextures();
}

// Cập nhật kết cấu cho tất cả các phân đoạn rắn
void Snake::updateTextures() {
    for (auto segment : _segments) {
        if (segment == _segments.front()) {
            segment->texture = _headTexture;
        }
        else if (segment == _segments.back()) {
            segment->texture = _tailTexture;
        }
        else {
            segment->texture = _bodyTexture;
        }
        segment->angle = segment->direction.getAngle();
    }
}

// Đặt lại vị trí và chiều dài của rắn
void Snake::reset(){

    _segments.clear();

    for (int i = 0; i < _initSegments; i++){
       addSegment( {_initPosition.x - (_segmentIndent * i), _initPosition.y}, _initDirection);
    } 

    _segments.front()->buffdirection = _initDirection;
    _segments.front()->direction  = _initDirection;
}

// Thêm phân đoạn mới từ cuối
void Snake::addScore(){ 
    
    for (int i = 0; i < _segmentsPerScore; i++){
        addSegment( _segments.back()->position, _segments.back()->direction);
    }
}

// Thêm phân khúc mới
void Snake::addSegment(Vector2f position, Vector2f direction){ 

    // phân bổ bộ nhớ cho phân đoạn mới
    SnakeSegment* pNewSegment = new SnakeSegment();

    //tính toán thụt lề từ phân đoạn cuối cùng
    position.x += _segmentIndent * -direction.x;
    position.y += _segmentIndent * -direction.y;

    pNewSegment->position = position;
    pNewSegment->direction = direction;

    pNewSegment->texture = _tailTexture;
    pNewSegment->angle = direction.getAngle();

    _segments.push_back(pNewSegment);
}

// Kiểm tra xem hướng này có hợp pháp không
void Snake::setDirection(float x, float y){
    
    if ( _segments.front()->direction.x != (x*-1) && _segments.front()->direction.y != (y*-1)){ 
        _segments.front()->buffdirection = {x, y};
    }
}

// Hoán đổi đầu và đuôi
void Snake::swapHeadTail() {
    if (_segments.size() < 2) return;  // Không thể swap nếu rắn quá ngắn
 
    // Lưu hướng đi của từng đoạn thân trước khi đảo ngược
    std::vector<Vector2f> oldDirections;
    for (auto segment : _segments) {
        oldDirections.push_back(segment->direction);
    }

    // Đảo ngược danh sách _segments
    _segments.reverse();

    // Cập nhật lại hướng đi theo thứ tự ngược lại
    auto iter = _segments.begin();
    for (size_t i = 0; i < oldDirections.size(); i++, iter++) {
        (*iter)->direction = -oldDirections[oldDirections.size() - 1 - i]; // Đảo ngược hướng đi
        (*iter)->buffdirection = (*iter)->direction;
    }

    // Cập nhật kết cấu (textures) để hiển thị đúng
    updateTextures();
}

// Xóa phân đoạn phía trước
void Snake::removeSegment(){
    delete _segments.front();
    _segments.pop_front();
}

// Nhận vị trí đầu
Vector2f Snake::getPosition() const{
    return _segments.front()->position;
}

// Nhận danh sách các phân đoạn rắn
std::list<SnakeSegment*> Snake::getSegments(){
    return _segments;
}

// lấy con trỏ đến đối tượng của head
SnakeSegment* Snake::getHead(){
    return _segments.front();
}

// trình hủy
Snake::~Snake(){
    _segments.clear(); 
}

