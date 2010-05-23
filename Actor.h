
#pragma once

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

template< typename T=float, size_t N_DIMENTIONS=3 >
class Actor;

class CollisionData;

typedef std::tr1::shared_ptr< Actor<float,2> > ActorPointer;
typedef std::vector< ActorPointer > ActorList;

// Any object that can be drawn, moved, and graphed can use Actor as its base.
// It does not determine how its derivative is drawn, moved, or graphed.
template< typename T, size_t N_DIMENTIONS >
class Actor 
{
    void init()
    {
        std::fill( v.begin(), v.end(), 0 );
        std::fill( a.begin(), a.end(), 0 );

        killMe = false;
    }
   
protected:
    Actor()
    {
    }

public:
    static void (*inserter)( ActorPointer ); // A function for inserting an Actor.

    static const int DIMENTIONS = N_DIMENTIONS;

    typedef Vector<T,N_DIMENTIONS> vector_type;
    typedef T                      value_type;

    // Single-letter vars used in order to be close to the mathematical 
    // equations.
    vector_type s; // State, or position.
    vector_type v; // Velocity.
    vector_type a; // Acceleration.

    vector_type previousS;

    value_type maxSpeed;

    value_type scale; // At what magnification to draw the Actor.

    bool killMe; // True if this should die.

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
    virtual void move( int quantum )
    {
        previousS = s;

        value_type speed = magnitude( v );
        if( false )
        {
            value_type k = maxSpeed / speed;
            v *= k;
        }

        v += a * quantum;
        s += 0.5 * a * quantum * quantum + v * quantum;
    }

    // Whether all Actors should have collision is undecided.
    virtual void collide( const vector_type& intersection ) = 0;

    virtual ~Actor()
    {
    }

    virtual CollisionData& collision_data() = 0;
};

template< typename T, size_t N >
void (*Actor<T,N>::inserter)( ActorPointer ) = 0;

