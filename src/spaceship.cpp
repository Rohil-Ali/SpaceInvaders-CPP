#include "spaceship.hpp"

Spaceship::Spaceship()
{
    image = LoadTexture("Graphics/spaceship.png");
    position.x = (GetScreenWidth() - image.width)/2;
    position.y = GetScreenHeight() - image.height - 100;
    lastFiredTime = 0;
    laserSound = LoadSound("Sounds/laser.ogg");
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void Spaceship::Draw()
{
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft()
{
    position.x -= 7;
    if(position.x <= 25) //check to ensure spaceship doesnt leave screen
    {
        position.x = 25;
    }
}

void Spaceship::MoveRight()
{
    position.x += 7;
    if(position.x >= GetScreenWidth() - image.width - 25) 
    {
        position.x = GetScreenWidth() - image.width - 25;
    }
}

void Spaceship::FireLaser()
{
    //delay to prevent multiple lasers to spawn at once
    if(GetTime() - lastFiredTime >= 0.35)
    {
        //this add a laser object to the lasers vector. the position of the object gets the origin of the spaceship, adds it to half the image width to get to the center of the spaceship and the final (-2) is to offset the lasers origin point so it spawns at the center of the spaceship.
        lasers.push_back(Laser({position.x + image.width/2 - 2, position.y},-6));
        lastFiredTime = GetTime();
        PlaySound(laserSound);
    }
    
}

Rectangle Spaceship::getRect()
{
    return {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - image.width)/2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear(); 
}
