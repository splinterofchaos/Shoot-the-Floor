
#include <iostream>
#include <fstream>

#include "math/Vector.h"
#include "ScopeGuard.h"
#include "functional_plus.h"

#include "Actor.h"
#include "Playfield.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <algorithm> // For for_each().
#include <functional> // For mem_fun_ptr.

GLenum init_gl( int w, int h )
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, w, h, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    return glGetError();
}

bool make_sdl_gl_window( int w, int h )
{
    if( ! SDL_SetVideoMode(w, h, 32, SDL_OPENGL) )
        return false;
    init_gl( w, h );
    return true;
}

void update_screen()
{
    SDL_GL_SwapBuffers();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

int main( int argc, char** argv )
{
    // Portably suppresses unused variable compiler warnings.
    #define NOT_USED(var) ((void)(var))

    const int MAX_FRAME_TIME = 10;

    bool quit = false;

    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
        return 1;
    make_sdl_gl_window( 700, 600 );
    //ScopeGuard quitSdl = scope_guard( SDL_Quit ); NOT_USED( quitSdl ); 
    //ScopeGuard flushGl = scope_guard( glFlush ); NOT_USED( flushGl );

    new Playfield( vector<float>(350, 300), 200, 40 );

    int frameStart=SDL_GetTicks(), frameEnd=frameStart, frameTime=0;
    while( quit == false )
    {
        static SDL_Event event;
		while( SDL_PollEvent(&event) )
		{
			if( event.type == SDL_QUIT )
                quit = true;
		}

        Uint8* keyState = SDL_GetKeyState( 0 );
        if( keyState[SDLK_ESCAPE] )
            quit = true;

        // TODO: Fixed time-step. 
        // Note that for_each_ptr is needed here. std::mem_fn should work, but
        // for whatever reason it's not. TODO: fix it?
        for_each_ptr ( 
            Actor::actors.begin(), Actor::actors.end(), 
            std::bind2nd( std::mem_fun_ref(&Actor::move), frameTime )
        );

        for_each ( 
            Actor::actors.begin(), Actor::actors.end(), 
            std::mem_fn( &Actor::draw ) 
        );

        update_screen();

        frameStart = frameEnd;
        frameEnd = SDL_GetTicks();
        frameTime = frameEnd - frameStart;
        if( frameTime > MAX_FRAME_TIME )
            frameTime = MAX_FRAME_TIME;
    }

    SDL_Quit();
    // Without this, on some systems, the process will not close.
    glFlush();

    return 0;
}


