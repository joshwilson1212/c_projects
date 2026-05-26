#include <raylib.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define COUNT 10



int numbers[COUNT];


void draw_bars(){
    for (int i = 0; i < COUNT; ++i) {
        
        int value = numbers[i];

        DrawRectangle(20+(20*i),20,10,100,WHITE);


    }




}

int main(int argc, char *argv[])
{
    for (int i = 0; i < COUNT; ++i) {
        numbers[i] = i;     
        printf("numbers[i] = %d\n", numbers[i] );
        
    }
    
    InitWindow(WIDTH,HEIGHT,"Sorting Visualizer");

    while(!WindowShouldClose()){
        BeginDrawing();
                
        draw_bars();
        
        EndDrawing();
    }
        

    


    CloseWindow();
}

