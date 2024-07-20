#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp" 
#include "alien.hpp" 
#include "mysteryship.hpp" 
#include <iostream>

class Game
{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        int lives;
        bool isGameRunning;
        int score;
        int highScore;
        Music music;
    private:
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        Spaceship spaceship;
        std::vector<Obstacle> obstacles; //will hold all 4 obstacle objects
        std::vector<Alien> aliens;
        void AlienShootLaser();
        void MoveAliens();
        void CheckForCollisions();
        void MoveAliensDown(int distance);
        int aliensDirection;
        std::vector<Laser> alienLasers;
        constexpr static float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        MysteryShip mysteryship;
        float mysteryShipSpawnInterval;
        float timeLastSpawned;
        void GameOver();
        void Reset();
        void InitGame();
        void CheckForHighScore();
        void SaveHighscoreToFile(int highscore);
        int LoadHighscoreFromFile();
        Sound explosionSound;
};