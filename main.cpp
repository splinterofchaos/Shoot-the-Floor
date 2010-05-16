
#include <iostream>
using namespace std;

#include "math/Vector.h"
#include "ScopeGuard.h"
#include "Collision.h"
#include "Controller.h"
#include "functional_plus.h"

#include "Actor.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

// include shared ptr.
#if defined( __GNUC__ )
    // tr1/memory is required to include tr1/shared_ptr.h... Dunno why.
    #include <tr1/memory> 
    #include <tr1/shared_ptr.h>
#elif defined( __MSVC__ )
    #error "Insert whatever you have to to use shared_ptr here!"
#endif

#include <algorithm> // For for_each().
#include <functional> // For mem_fun_ptr.

GLenum init_gl( int w, int h )
{
    glClearColor( 1.0f, 0.0f, 1.0f, 1.0f );

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
    glClear( GL_COLOR_BUFFER_BIT );
}

int main()
{
    // Portably suppresses unused variable compiler warnings.
    #define NOT_USED(var) ((void)(var))

    typedef std::tr1::shared_ptr< Actor<float> > ActorPointer;
    typedef std::vector< ActorPointer > ActorList;

    typedef std::tr1::shared_ptr< Controller > ControllerPointer;
    typedef std::vector< ControllerPointer > ControlerList;

    const int MAX_FRAME_TIME = 10;

    bool quit = false;
    SDL_Event event;

    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
        return 1;
    make_sdl_gl_window( 1200, 800 );
    ScopeGuard quitSdl = scope_guard( SDL_Quit ); NOT_USED( quitSdl ); 

    ActorList actors;
    ControlerList controlers;

    int frameStart=SDL_GetTicks(), frameEnd=frameStart, frameTime=0;
    while( quit == false )
    {
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )
                quit = true;
		}

        Uint8* keyState = SDL_GetKeyState( 0 );
        if( keyState[SDLK_ESCAPE] )
            quit = true;

        Keyboard::refresh();

        for_each_ptr (
            controlers.begin(), controlers.end(), 
            std::mem_fun_ref( &Controller::update ) 
        );

        // TODO: Fixed time-step. 
        //for_each_ptr ( 
        //    actors.begin(), actors.end(), 
        //    std::bind2nd( std::mem_fun_ref( &Actor<float,2>::move ), frameTime )
        //);

        //for_each_ptr ( 
        //    actors.begin(), actors.end(), 
        //    std::mem_fun_ref( &Actor<float,2>::draw ) 
        //);

        update_screen();

        frameStart = frameEnd;
        frameEnd = SDL_GetTicks();
        frameTime = frameEnd - frameStart;
        if( frameTime > MAX_FRAME_TIME )
            frameTime = MAX_FRAME_TIME;
    }
}


