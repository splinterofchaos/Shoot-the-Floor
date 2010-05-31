
#include "math/Vector.h"
#include "glpp.h"
#include "draw_shape.h"

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <algorithm>
#include <vector>

// include shared ptr.
#if defined( __GNUC__ )
    // tr1/memory is required to include tr1/shared_ptr.h... Dunno why.
    #include <tr1/memory> 
    #include <tr1/shared_ptr.h>
#elif defined( __MSVC__ )
    #error "Insert whatever you have to to use shared_ptr here!"
#endif

#pragma once

struct CollisionData; // Prototype; defined elsewhere.

// Any object that can be drawn, moved, and graphed can use Actor as its base.
// It does not determine how its derivative is drawn, moved, or graphed.
class Actor
{
    void init()
    {
        std::fill( v.begin(), v.end(), 0 );
        std::fill( a.begin(), a.end(), 0 );

        actors.push_back( ActorPointer(this) );
    }


    typedef std::tr1::shared_ptr< Actor > ActorPointer;
    typedef std::vector< ActorPointer > ActorList;

protected:
    Actor()
    {
    }

public:
    static ActorList actors;

    typedef float value_type;
    typedef Vector<float,2> vector_type;

    // Single-letter vars used in order to be close to the mathematical 
    // equations.
    vector_type s; // State, or position.
    vector_type v; // Velocity.
    vector_type a; // Acceleration.

    vector_type previousS;

    value_type maxSpeed;

    value_type scale; // At what magnification to draw the Actor.

    bool isSurface;

    Actor( const vector_type& pos )
        : s(pos), previousS(s), maxSpeed(0), scale( 1 )
    {
        init();
    }

    // This class does not care whether its derivatives draw with vectors,
    // bitmaps, or what-have-you.
    virtual void draw() = 0;

    // This class does not care about how the object is moved, but does set
    // up a simple, generic state integration.
    virtual void move( int dt )
    {
        v += a * dt;
        s += v*dt + 0.5*a*dt*dt;
    }

    virtual ~Actor()
    {
    }
};

