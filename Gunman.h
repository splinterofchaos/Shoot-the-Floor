
#include "Actor.h"
#include "Playfield.h"

#pragma once

class Gunman : public Actor<float,2>, public PointCollisionData
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
    value_type normal_angle();
    vector_type normal_to( const vector_type& V );

  public:
    Playfield& playfield;

    bool controlledByPlayer; // True if this is a pawn of the player.
    bool isGrounded;

    Gunman( const vector_type& pos, Playfield& p, bool controlledByPlayer=false );

    void move( int quantum );
    void draw();

    void collide( const vector_type& intersection );

    // For PointCollisionData
    CollisionData& collision_data() 
    {
        return *this;
    }

    Vector<float,2> pos() const
    {
        return s;
    }
};
