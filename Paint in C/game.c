#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define START_RADIUS 20
#define START_COLOR 0x00000000

#define TARGET_FPS 240
#define COLOR_RECT_SIZE 50

Uint32 color = START_COLOR;
Uint32 color_palette[] = {0x000000, 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF};
const int color_palette_size = 8;
int cur_color_index = 0;

bool inside_color_pallette(int x, int y){
    return x <= color_palette_size*COLOR_RECT_SIZE && y <= COLOR_RECT_SIZE;
}

bool inside_clear_button(int x, int y){
    return x >= WIDTH-COLOR_RECT_SIZE && y <= COLOR_RECT_SIZE;
}

 //checks if user clicked color palette and updates color if so
 void check_color_palette_chosen(int x,int y){
    int i;

    if(inside_color_pallette(x,y)){
        //mouse is inside of xy range of color palette
        i = x / COLOR_RECT_SIZE;
        color = color_palette[i];
        cur_color_index = i;
    }

    

}

void draw_color_selection_boarder(SDL_Surface *surface){
    SDL_Rect boarder;
    SDL_Rect inside;


    boarder = (struct SDL_Rect){cur_color_index * COLOR_RECT_SIZE, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE};
    SDL_FillRect(surface, &boarder,0x111111);
    

    inside = (struct SDL_Rect){cur_color_index * COLOR_RECT_SIZE + 5, 5, COLOR_RECT_SIZE-10, COLOR_RECT_SIZE-10};
    SDL_FillRect(surface, &inside,color);
}


void draw_clear_button(SDL_Surface *surface, int size){
    SDL_Rect boarder;
    SDL_Rect inside;
    SDL_Rect pixel = {0,0,1,1};

    boarder = (struct SDL_Rect){WIDTH-size, 0, size, size};
    SDL_FillRect(surface, &boarder, 0x000000);

    inside = (struct SDL_Rect){WIDTH-size + 1, 1, size-2, size-2};
    SDL_FillRect(surface, &inside,0xFFFFFF);
    
    //makeing an x
    for(int x = WIDTH-size; x<WIDTH+size; x++){
        for(int y = 0; y<size; y++){
            if(x-850 == y){
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(surface, &pixel, color_palette[0]);
            }
        }
    }
    // adding 1 to the y becuase of the WIDTH max 899 on screen
    //we add plus one to y to shift the matching of x values over to the left
    //because the corner of the screen is out of bounds making the x off centered and below the left bound box
    for(int x = WIDTH; x>WIDTH-size; x--){
        for(int y = 0; y<size; y++){
            if(-(x-WIDTH) == y+1){
                pixel.x = x;
                pixel.y = y;
                //SDL_FillRect(surface, &pixel, color_palette[0]);
                SDL_FillRect(surface, &pixel, color_palette[0]);
                
            }


        }
    }
    

}

// draws the color pallette
void draw_pallette(SDL_Surface *surface, Uint32 *colors, int size){
    SDL_Rect color_rect;
    for (int i = 0; i < size; i++){

        color_rect = (struct SDL_Rect){i * COLOR_RECT_SIZE, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE};
        SDL_FillRect(surface, &color_rect, colors[i]);
    }

}

// draws a circle at center coordinates with given radius and color
void draw_circle(SDL_Surface *surface, int x_center, int y_center, int radius, Uint32 color){

    SDL_Rect pixel = {0, 0, 1, 1};
    for (int x = x_center - radius; x < x_center + radius; x++){
        for (int y = y_center - radius; y < y_center + radius; y++){

            // is this pixel part of the circle?
            int distance_from_center = sqrt(pow(x - x_center, 2) + pow(y - y_center, 2));
            if (distance_from_center < radius){
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

int main(int argc, char *argv[]){
    bool done = false;
    SDL_Init(SDL_INIT_VIDEO);

    //makes a window
    SDL_Window *window = SDL_CreateWindow(
        "An SDL3 window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0);
    
    //creates the surface
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    
    float delay_millis = (1.0 / TARGET_FPS) * 1000;

    bool draw = false;
    int x, y;
    int brush_size = START_RADIUS;
    bool mouseisdown = false;
    
    //draws color pallette and sets surface to white
    SDL_FillRect(surface, NULL, 0xFFFFFF);
    draw_pallette(surface, color_palette, color_palette_size);
    SDL_UpdateWindowSurface(window);
    

    //logic loop that updates the surface every frame
    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type){
            case SDL_QUIT:
                done = true;
                break;

            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                if(inside_color_pallette(x,y) || inside_clear_button(x,y)){
                    draw = false;
                    break; 
                }
                if(mouseisdown)
                    draw = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseisdown = true;
                x = event.motion.x;
                y = event.motion.y;
                draw_pallette(surface, color_palette, color_palette_size);
                check_color_palette_chosen(x,y);
                if((inside_color_pallette(x,y) || inside_clear_button(x,y)) == false){
                    draw = true;
                }
                if(inside_clear_button(x,y)== true){

                    //clears the surface
                    SDL_FillRect(surface, NULL, 0xFFFFFF);
                }
                break;

            case SDL_MOUSEBUTTONUP:
            mouseisdown = false;
                draw = false;
                break;
            case SDL_MOUSEWHEEL:
                brush_size +=event.wheel.preciseY;
                if(brush_size < 1)
                    brush_size = 1;
                break;
            }

        } // logic goes here
        if (draw){
            draw_circle(surface, x, y, brush_size, color);
            SDL_UpdateWindowSurface(window);
        }
        draw_clear_button(surface,COLOR_RECT_SIZE);
        draw_pallette(surface, color_palette, color_palette_size);
        draw_color_selection_boarder(surface);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(delay_millis);
    }

    return 0;
}