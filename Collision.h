
#pragma once

#include "math/Vector.h"

#include <algorithm> // for max and min.
#include <cmath>     // for abs.

struct Collision;
struct CollisionData;
struct PointCollisionData;
struct LoopCollisionData;

Collision collision( CollisionData&, CollisionData& );
Collision collision_( LoopCollisionData&, PointCollisionData&);

struct Collision
{
    bool isOccuring;
    Vector<float,2> intersection;

    operator bool ()
    {
        return isOccuring;
    }
};

struct CollisionData
{
    static Collision no_collision();

    virtual Vector<float,2> pos() = 0;

    virtual Collision visit( CollisionData& other ) = 0 ;
    virtual Collision visit( PointCollisionData& other ) = 0;
    virtual Collision visit( LoopCollisionData& other ) = 0;
    
};

struct PointCollisionData : CollisionData
{
    virtual Collision visit( CollisionData& );
    virtual Collision visit( PointCollisionData& );
    virtual Collision visit( LoopCollisionData& );
};

struct LoopCollisionData : CollisionData
{
    virtual float inner_radius() = 0;
    virtual float outer_radius() = 0;

    virtual Collision visit( CollisionData& );
    virtual Collision visit( PointCollisionData& );
    virtual Collision visit( LoopCollisionData& other );
};

Collision collision( CollisionData& a, CollisionData& b );

Collision collision_( LoopCollisionData& field, PointCollisionData& gunman );

