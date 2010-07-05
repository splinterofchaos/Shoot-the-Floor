
#include "Gunman.h"

#include "math/Vector.h"
#include "Bullet.h"

#include "Collision.h"

#include <SDL/SDL.h> // For Mouse IO.
#include <cmath>

Uint8* HumanGunmanController::keyStates  = 0;
Uint8  HumanGunmanController::mouseState = 0;
Actor::vector_type HumanGunmanController::cursorPos;

void HumanGunmanController::reload()
{
    keyStates = SDL_GetKeyState( 0 );

    Vector<int,2> cPosInt;
    mouseState = SDL_GetMouseState( &cPosInt.x(), &cPosInt.y() );
    cursorPos = cPosInt;
}

unsigned int HumanGunmanController::move_left()
{
    return keyStates[ SDLK_a ];
}

unsigned int HumanGunmanController::move_right()
{
    return keyStates[ SDLK_d ];
}

bool HumanGunmanController::jump()
{
    return keyStates[ SDLK_w ] || keyStates[ SDLK_SPACE ];
}

bool HumanGunmanController::shoot()
{
    return mouseState & SDL_BUTTON(1);
}

Actor::vector_type HumanGunmanController::pointing_target()
{
    return cursorPos;
}

Gunman::vector_type Gunman::points[ Gunman::N_POINTS ];
Gunman::vector_type Gunman::headPoints[ Gunman::N_HEAD_POINTS ];

Gunman::Gunman( const Gunman::vector_type& pos, Playfield& p, const ControllerPointer& controller )
    : parent( pos ), pointingDirection( 0 ), playfield( p ), controller( controller )
{
    if( ! controller )
        ; // TODO: Error handling.

    scale = 10;

    static bool firstInit = true;
    if( ! firstInit )
        return;
    else
        firstInit = false;

    points[ SHOULDER   ] = vector( value_type(0),      -3*scale );
    points[ WAIST1     ] = vector( value_type(0),        -scale );
    points[ RIGHT_FOOT ] = vector(       scale/2, value_type(0) );
    points[ LEFT_FOOT  ] = vector(      -scale/2, value_type(0) );
    points[ WAIST2 ] = points[ WAIST1 ];

    // Initialize headPoints.
    const float ROTATION = 2 * 3.145 / N_HEAD_POINTS;
    const float TANGENTAL_FACTOR = std::tan( ROTATION );
    const float RADIAL_FACTOR    = std::cos( ROTATION );

    // Radial vectors.
    Vector<float,2> rv = vector<value_type>( HEAD_RADIUS, 0 );

    // Note that this point should be repeated at the end.
    headPoints[0] = rv;
    for( unsigned int i=1; i < N_HEAD_POINTS; i++ )
    { 
        Vector<float,2> tv = vector( -rv.y(), rv.x() );

        rv += tv * TANGENTAL_FACTOR;
        rv *= RADIAL_FACTOR;

        headPoints[i] = rv;
    }
}

Gunman::~Gunman()
{
    delete controller;
}

Gunman::value_type Gunman::normal_angle()
{
    vector_type diff = playfield.s - s;
    return atan2( diff.y(), diff.x() );
}

Gunman::vector_type Gunman::normal_to( const vector_type& V )
{
    value_type nangle = normal_angle();
    vector_type n = vector( std::cos(nangle), std::sin(nangle) );
    n = -( V * unit(n) ) * unit(n);
    return n;
}

Gunman::vector_type Gunman::shoulder_point()
{
    // Since the shoulder is only offset in the Y direction, just consider that magnitude.
    const value_type& magnitude = points[ SHOULDER ].y();

    value_type angle = normal_angle();

    return vector( -std::cos(angle), -std::sin(angle) ) * magnitude;
}

void Gunman::move( int dt )
{
    controller->reload();

    std::fill( a.begin(), a.end(), 0 );

    vector_type diff = controller->pointing_target() - ( s + shoulder_point() );
    pointingDirection = std::atan2( diff.y(), diff.x() );

    Uint32 time = SDL_GetTicks();
    if( controller->shoot() && timeTillNextShot <= time )
    {
        timeTillNextShot = time + SHOOT_DELAY;

        new Bullet ( 
            s + shoulder_point() + arm_vector(),
            v
        );
    }

    // Body movement:
    vector_type forceSum; // The sum of all forces acting on this.

    // Gravity:
    static const vector_type GRAVITY = vector<value_type>( 0, 0.0005 );
    forceSum = GRAVITY;

    // For now, ignore the difference between force and acceleration; 
    // there is no mass.
    if( isGrounded ) 
    {
        static const value_type WALK_ACCEL = 0.001;

        forceSum += normal_to( GRAVITY );

        if( controller->jump() )
        {
            value_type jAngle = normal_angle();
            vector_type jump = vector( std::cos(jAngle), std::sin(jAngle) ) * 0.5f;
            v += jump;
            isGrounded = false;
        }

        if( controller->move_left() )
        {
            vector_type diff = s - playfield.s;
            vector_type walkAccel = unit( clockwise_tangent(diff) ) * WALK_ACCEL;
            a += walkAccel;
        }

        if( controller->move_right() )
        {
            vector_type diff = s - playfield.s;
            vector_type walkAccel = unit( counter_clockwise_tangent(diff) ) * WALK_ACCEL;
            a += walkAccel;
        }
    } 
    else
    {
        static const value_type AIR_ACCEL = 0.0005;

        if( controller->move_right() )
            a += vector_type( AIR_ACCEL, value_type(0) );
        if( controller->move_left() )
            a -= vector_type( AIR_ACCEL, value_type(0) );
    }

    a += forceSum;
    Actor::move( dt, 1.0 );

    static int x = 100;
    if( isGrounded && x-- < 0 ) {
        float angle = normal_angle() + 3.14;
        s = vector( std::cos(angle), std::sin(angle) ) * playfield.inner_radius() + playfield.s;

        x = 100;
    }
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
        shoulder_point(), shoulder_point() + arm_vector()
    };

    // TODO: Rotation.
    // Rotation will involve not only the graphical rotation of the gunman, but
    // since the location of a bullet firing will be affected by the hand's
    // position, thus the body's rotation, and even how the direction the arm
    // points in must change, it cannot be done as-of-yet. I don't know enough
    // math.

    glEnableClientState( GL_VERTEX_ARRAY );
    {
        glColor3f( 0, 0, 0 );
        glTranslatef( s.x(), s.y(), 0 );

        // The arm is calculated without rotation, so draw it,
        glVertexPointer( 2, GL_FLOAT, 0, armPoints );
        glDrawArrays( GL_LINE_STRIP, 0, 2 );

        // and rotate AFTER.
        float angle = normal_angle();
        glRotatef( angle * (180/3.145) + 90, 0, 0, 1 );

        glVertexPointer( 2, GL_FLOAT, 0, points );
        glDrawArrays( GL_LINE_STRIP, 0, N_POINTS );

        glTranslatef( 0, -3*scale - HEAD_RADIUS, 0 );
        glVertexPointer( 2, GL_FLOAT, 0, headPoints );
        glDrawArrays( GL_LINE_LOOP, 0, N_HEAD_POINTS );
    }
    glDisableClientState( GL_VERTEX_ARRAY );

    glLoadIdentity();
}

void Gunman::collide( Actor& collider )
{
    // TODO: Should s change here?

    v += normal_to( v );
    isGrounded = true;
}

CollisionData& Gunman::collision_data()
{
    return *this;
}

Vector<float,2>& Gunman::pos()
{
    return s;
}
