#include "laser.hpp"
#include <iostream>

Laser::Laser(Vector2 position, int speed)
{
    this->position = position;
    this->speed = speed;
    active = true; 
}

void Laser::Draw()
{
    if(active) //will draw laser if it is active(inside the game window)
    {
        DrawRectangle(position.x, position.y, 4, 15, {243, 216, 63, 255});
    }
        
}
//moves laser across y axis, - speed to move up, + speed to move down
void Laser::Update()
{
    position.y += speed;
    //if laser is out of screen then active will be set to false, which will delete the object.
    if(active)
    {
        if(position.y < 25 || position.y > GetScreenHeight() - 100)
        {
            active = false;
            std::cout << "Laser inactive" << std::endl;
        }
    }
        
}

Rectangle Laser::getRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 4;
    rect.height = 15;
    return rect;
}
