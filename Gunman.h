
#include "Actor.h"
#include "Playfield.h"

#pragma once

class Gunman : public Actor<float,2>
{
    typedef Actor<float,2> parent;

    value_type pointingDirection; // The angle of the gunman's arm.

    /* POINTS MAP
     *      O    Head drawn elsewhere.
     *      |    
     *     / \   
     *
     * Draw order: SHOULDER, WAIST, LEFT FOOT, WAIST, RIGHT FOOT.
     */
    enum { SHOULDER, WAIST1, LEFT_FOOT, WAIST2, RIGHT_FOOT, N_POINTS };
    vector_type points[ N_POINTS ];

    vector_type arm_vector();

    vector_type normal_to( const vector_type& V )
    {
        vector_type diff = playfield.s - s;
        value_type nAngle = atan2( diff.y(), diff.x() );
        vector_type n = vector( std::cos(nAngle), std::sin(nAngle) );
        n = -( V * normalize(n) ) * normalize(n);
        return n;
    }

  public:
    Playfield& playfield;

    bool controlledByPlayer; // True if this is a pawn of the player.
    bool isGrounded;

    Gunman( const vector_type& pos, Playfield& p, bool controlledByPlayer=false );

    void move( int quantum );
    void draw();

    void collide( const vector_type& intersection )
    {
        // TODO: Should s change here?

        v += normal_to( v );
        isGrounded = true;
    }
};
