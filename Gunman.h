
#include "Actor.h"
#include "Playfield.h"

#pragma once

class Gunman : public Actor, PointCollisionData
{
    typedef Actor parent;

    value_type pointingDirection; // The angle of the gunman's arm.

    /* POINTS MAP
     *      O    Head drawn elsewhere.
     *      |    
     *     / \   
     *
     * Draw order: SHOULDER, WAIST, LEFT FOOT, WAIST, RIGHT FOOT.
     */
    enum { SHOULDER, WAIST1, LEFT_FOOT, WAIST2, RIGHT_FOOT, N_POINTS };
    static vector_type points[ N_POINTS ];

    static const unsigned int N_HEAD_POINTS = 11;
    static const unsigned int HEAD_RADIUS = 10;
    static vector_type headPoints[ N_HEAD_POINTS ];

    // How long (in miliseconds) to wait between shots.
    static const int SHOOT_DELAY = 10; 
    int timeTillNextShot;

    vector_type shoulder_point();
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

    void collide( Actor& collider );

    // For PointCollisionData
    CollisionData& collision_data();
    Vector<float,2>& pos();
};
