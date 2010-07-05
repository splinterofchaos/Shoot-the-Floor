
#include "Actor.h"
#include "Playfield.h"

#pragma once

struct GunmanController
{ 
    virtual void reload() = 0;

    virtual unsigned int move_left() = 0;
    virtual unsigned int move_right() = 0;

    virtual bool jump() = 0;
    virtual bool shoot() = 0;

    virtual Actor::vector_type pointing_target() = 0;
};

struct HumanGunmanController : GunmanController
{
    static Uint8* keyStates;
    static Uint8 mouseState;
    static Actor::vector_type cursorPos;

    void reload();

    unsigned int move_left();
    unsigned int move_right();

    bool jump();
    bool shoot();
    
    virtual Actor::vector_type pointing_target();
};

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
    unsigned int timeTillNextShot;

    vector_type shoulder_point();
    vector_type arm_vector();
    value_type normal_angle();
    vector_type normal_to( const vector_type& V );

  public:
    Playfield& playfield;

    typedef GunmanController Controller;
    typedef Controller* ControllerPointer;
    ControllerPointer controller;

    bool isGrounded;

    Gunman( const vector_type& pos, Playfield& p, const ControllerPointer& controller);
    ~Gunman();

    void move( int quantum );
    void draw();

    void collide( Actor& collider );

    // For PointCollisionData
    CollisionData& collision_data();
    Vector<float,2>& pos();
};
