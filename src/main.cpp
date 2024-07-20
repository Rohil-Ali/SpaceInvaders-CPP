#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number, int width){
    std::string numberText = std::to_string(number); //converts score to string
    int leadingZeros = width - numberText.length();
    return numberText = std::string(leadingZeros, '0') + numberText;
}

int main()
{
    Color grey = Color{29, 29, 27, 255};
    Color blue = {52, 152, 219, 255};

    int offset = 50;
    int screenWidth = 750;
    int screenHeight = 700;

    InitWindow(screenWidth + offset, screenHeight + 2*offset, "Space invaders by Rohail");
    InitAudioDevice();

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

    SetTargetFPS(60);
    Game game;

    //Game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(game.music);
        //Handles the players inputs
        game.HandleInput();
        game.Update();

        //draws objects
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, blue);
        DrawLineEx({25, 730}, {775, 730}, 3, blue);

        if(game.isGameRunning){
            DrawTextEx(font, "Space Invaders", {500, 740}, 34, 2, blue);
        }else{
             DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, blue);
        }

        float x = 50.0;
        for(int i = 0; i < game.lives; i++)
        {

            DrawTextureV(spaceshipImage, {x, 735}, WHITE);
            x += 50;
        }

        DrawTextEx(font, "SCORE  ", {50, 15}, 34, 2, blue);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, blue);

        DrawTextEx(font, "HIGHSCORE", {570, 15}, 34, 2, blue);
        std::string highScoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highScoreText.c_str(), {655, 40}, 34, 2, blue);



        game.Draw();
        EndDrawing(); 
    }

    CloseWindow();
    CloseAudioDevice();

    return 0;
}