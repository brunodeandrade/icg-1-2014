#include "vec.h"
#include "CG.h"
 

int main(int argc, char** argv)
{
        
        leituraVertices();

        float angle = 0.0;
        const int triangle = 1;
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Surface *screen;
        screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE|SDL_OPENGL);
        bool running = true;
        const int FPS = 100;
        Uint32 start;
        SDL_Event event;

        init();
        while(running) {
                start = SDL_GetTicks();
                while(SDL_PollEvent(&event)) {
                        switch(event.type) {
                                case SDL_QUIT:
                                        running = false;
                                        break;
                        }
                }
 
                display(angle);
                SDL_GL_SwapBuffers();
                angle += 0.5;
                if(angle > 360)
                        angle -= 360;
                if(1000/FPS > SDL_GetTicks()-start)
                        SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
        }
        SDL_Quit();
        return 0;
}