
#include "Gunman.h"
#include "Bullet.h"

#include "Collision.h"

#include <SDL/SDL.h> // For Mouse IO.
#include <cmath>

Gunman::Gunman( const Gunman::vector_type& pos, Playfield& p, bool controlledByPlayer )
    : parent( pos ), pointingDirection( 0 ), playfield( p ),
    controlledByPlayer( controlledByPlayer )
{
    scale = 10;

    points[ SHOULDER   ] = vector( value_type(0),      -3*scale );
    points[ WAIST1     ] = vector( value_type(0),        -scale );
    points[ RIGHT_FOOT ] = vector(       scale/2, value_type(0) );
    points[ LEFT_FOOT  ] = vector(      -scale/2, value_type(0) );
    points[ WAIST2 ] = points[ WAIST1 ];
}

Gunman::value_type Gunman::normal_angle()
{
    vector_type diff = playfield.s - s;
    return atan2( diff.y(), diff.x() );
}

Gunman::vector_type Gunman::normal_to( const vector_type& V )
{
    value_type nAngle = normal_angle();
    vector_type n = vector( std::cos(nAngle), std::sin(nAngle) );
    n = -( V * normalize(n) ) * normalize(n);
    return n;
}

void Gunman::move( int quantum )
{
    // The sum of all forces acting on this.
    vector_type forceSum = vector( 0, 0 ); 

    // Arm movement:
    if( controlledByPlayer ) {
        Uint8 mouseState;

        // Make a vector pointing from shoulder to cursor.
        int x, y;
        mouseState = SDL_GetMouseState( &x, &y );
        vector_type v = vector(x,y) - ( s + points[SHOULDER] );

        pointingDirection = std::atan2( v.y(), v.x() );

        // Shoot a bullet.
        if( mouseState & SDL_BUTTON(1) ) 
        {
            Bullet* p = new Bullet ( 
                s + points[SHOULDER] + arm_vector(),
                pointingDirection 
            );
            parent::inserter( ActorPointer(p) );
        }

        // Move body.
        static const value_type MOVE_FORCE = 0.0001;
        Uint8* keys = SDL_GetKeyState( 0 );
        if( keys[SDLK_a] )
            forceSum += vector<value_type>( -MOVE_FORCE, 0 );
        if( keys[SDLK_d] )
            forceSum += vector<value_type>( MOVE_FORCE, 0 );
    } else {
        pointingDirection += 0.001;
    }

    // Body movement:

    // Gravity:
    static const vector_type GRAVITY = vector<value_type>( 0, 0.00005 );
    forceSum += GRAVITY;

    // For now, ignore the difference between force and acceleration; 
    // there is no mass.
    if( isGrounded ) { 
        // What if i multiplied this normal by a factor to keep the gunman
        // perfectly in the circle, and probably that factor will be
        // proportionate to time. ASK ON GD!
        forceSum += normal_to( GRAVITY );
    }

    a = forceSum;

    parent::move( quantum );
}

Gunman::vector_type Gunman::arm_vector()
{
    const value_type ARM_LENGTH = scale;

    vector_type unit = vector (
        std::cos( pointingDirection ), std::sin( pointingDirection )
    );

    return unit * ARM_LENGTH;
}

void Gunman::draw()
{
    // TODO: Really, the hand arm points could be integrated into points. The
    // hand point would still be updated in this block before continuing.
    vector_type armPoints[2] = {
        points[ SHOULDER ], points[ SHOULDER ] + arm_vector()
    };

    float angle = normal_angle();
    glTranslatef( s.x(), s.y(), 0 );
    glRotatef( angle * (180/3.145) + 90, 0, 0, 1 );

    // TODO: Rotation.
    // Rotation will involve not only the graphical rotation of the gunman, but
    // since the location of a bullet firing will be affected by the hand's
    // position, thus the body's rotation, and even how the direction the arm
    // points in must change, it cannot be done as-of-yet. I don't know enough
    // math.

    glEnableClientState( GL_VERTEX_ARRAY );
    {
        glColor3f( 0, 0, 0 );

        glVertexPointer( 2, GL_FLOAT, 0, points );
        glDrawArrays( GL_LINE_STRIP, 0, N_POINTS );

        glVertexPointer( 2, GL_FLOAT, 0, armPoints );
        glDrawArrays( GL_LINE_STRIP, 0, 2 );

        const int CIRCLE_PIXELS = 10;
        glTranslatef( 0, -3*scale - CIRCLE_PIXELS, 0 );
        draw_circle( CIRCLE_PIXELS, 11 );
    }
    glDisableClientState( GL_VERTEX_ARRAY );

    glLoadIdentity();
}

void Gunman::collide( const vector_type& intersection )
{
    // TODO: Should s change here?

    v += normal_to( v );
    isGrounded = true;
}

CollisionData& Gunman::collision_data()
{
    return *this;
}

Vector<float,2> Gunman::pos()
{
    return s;
}
