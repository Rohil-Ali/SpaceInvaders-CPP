#include "game.hpp"
#include <fstream>

Game::Game()
{
    InitGame();
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
}

Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update()
{
    //if game is not running there will be no more updates.
    if(isGameRunning)
    {
        double currentTime = GetTime();
        if (currentTime - timeLastSpawned > mysteryShipSpawnInterval)
        {
            mysteryship.Spawn();
            timeLastSpawned = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }
        
        for(auto& laser: spaceship.lasers)
        {
            laser.Update();
        }
        DeleteInactiveLasers();
        AlienShootLaser();

        for(auto& laser: alienLasers){
            laser.Update();
        }

        MoveAliens();
        mysteryship.Update();
    }
    else
    {
        if(IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }
    }

    
}

//stores all objects draw functions
void Game::Draw()
{
    spaceship.Draw();
    //loops through lasers vector and draws each laser
    for(auto& laser: spaceship.lasers)
    {
        laser.Draw();
    }

    for(auto& obstacle:obstacles)
    {
        obstacle.Draw();
    }

    for(auto& alien:aliens)
    {
        alien.Draw();
    }

    for(auto& laser:alienLasers)
    {
        laser.Draw();
    }

    mysteryship.Draw();

    CheckForCollisions();
}

//handles players inputs
void Game::HandleInput()
{
    if(isGameRunning)
    {
        if(IsKeyDown(KEY_LEFT))
        {
            spaceship.MoveLeft();
        }
        else if(IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();
        }
        else if(IsKeyDown(KEY_SPACE))
        {
            spaceship.FireLaser();
        }
    }
    
}

void Game::DeleteInactiveLasers()
{
    for(auto  it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    {
        if(!it -> active)
        {
            it = spaceship.lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for(auto  it = alienLasers.begin(); it != alienLasers.end();)
    {
        if(!it -> active)
        {
            it = alienLasers.erase(it);
        }
        else
        {
            ++it;
        }
    }

}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth))/5;

    for(int i = 0; i < 4; i++)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth; //evenly spaces each obstacle
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens; //temp vector 
    for(int row = 0; row < 5; row++)
    {
        for(int column = 0; column < 11; column++)
        {
            int alienType;
            if(row == 0 ){
                alienType = 3;
            } else if(row == 1 || row == 2) {
                alienType = 2;
            } else {
                alienType = 1;
            }
            
            float x = 75 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for(auto& alien:aliens)
    {
        //this will check if the alien image has gone out of the screen and will then adjust the aliens direction accordingly.
        if(alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25){
            aliensDirection = -1;
            MoveAliensDown(4);
        }
        else if(alien.position.x < 25){
            aliensDirection = 1;
            MoveAliensDown(4);
        }

        alien.Update(aliensDirection);
    }
}

void Game::MoveAliensDown(int distance)
{
    for(auto& alien:aliens)
    {
          alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()){
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width/2, 
                                    alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = GetTime();
    }
    
}

void Game::CheckForCollisions()
{
    //spaceship lasers
    //it will point at start of aliens vect. it will check if the laser and alien rectangles have overlapped, if true then it will remove alien from rect and it will point to next alien.
    for(auto& laser: spaceship.lasers){   
        auto it = aliens.begin();
        while(it != aliens.end()){
            if (CheckCollisionRecs(it -> getRect(), laser.getRect())){

                PlaySound(explosionSound);
                if(it -> type == 1){
                    score += 100;
                } else if(it -> type == 2){
                    score += 200;
                } else if(it -> type == 3){
                    score += 300;
                }
                CheckForHighScore();

                it = aliens.erase(it);
                laser.active = false;
            }
            else{
                ++it;
            }
        }

        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else{
                    ++it;
                }
            }
        }

        if(CheckCollisionRecs(mysteryship.getRect(), laser.getRect())){
            mysteryship.alive = false;
            laser.active = false;
            score += 500;
            CheckForHighScore();
            PlaySound(explosionSound);
        }
    }

    //alien lasers
    for(auto& laser: alienLasers){
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())){
            laser.active = false;
            lives --;
            if(lives == 0)
            {
                GameOver();
            }
        }

        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else{
                    ++it;
                }
            }
        }
    }

    //Alien Collision with obstacles
    for(auto& alien: aliens){
        for(auto& obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(), alien.getRect())){
                    it = obstacle.blocks.erase(it);
                }
                else{
                    it++;
                }
            }
        }

        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())){
            GameOver();
        }
    }
}

void Game::GameOver()
{
    isGameRunning = false;
}

//will reset all objecets to their original state adn delete any vectors.
void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawned = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    lives = 3;
    isGameRunning = true;
    score = 0;
    highScore = LoadHighscoreFromFile();
}

void Game::CheckForHighScore()
{
    if(score > highScore){
        highScore = score;
        SaveHighscoreToFile(highScore);
    }
}

//these two methods will save and load the highscore from file so it is not lost when the game is shut.
void Game::SaveHighscoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()){
        highscoreFile << highScore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::LoadHighscoreFromFile()
{
    int loadedHighScore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()){
        highscoreFile >> loadedHighScore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to load highscore from file" << std::endl;
    }
    return loadedHighScore;
}
