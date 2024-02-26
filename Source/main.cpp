#include "raylib.h"
#include "game.h"


int main(void)
{    
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "SPACE INVADERS");
    SetTargetFPS(60);

    Game game = { State::STARTSCREEN };   
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        game.Render();
        EndDrawing();
    }
    CloseWindow();        
    std::string filename = "level.txt";  

    return 0;
}