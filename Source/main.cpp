#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)

#include "raylib.h"
#include "game.h"


int main(void)
{    
    constexpr int screenWidth = 1920;
    constexpr int screenHeight = 1080;

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