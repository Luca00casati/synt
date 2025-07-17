#include <raylib.h>
#include "note.h"
#include "synt.h"

int main(void){

    InitWindow(800, 600, "HELLO");

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
