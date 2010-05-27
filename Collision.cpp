
#include "Collision.h"

Collision CollisionData::no_collision()
{
    Collision c;
    c.isOccuring = false;
    return c;
}

Collision PointCollisionData::visit( CollisionData& other )
{
    return other.visit( *this );
}

Collision PointCollisionData::visit( PointCollisionData& other )
{
    return no_collision();
}

Collision PointCollisionData::visit( LoopCollisionData& other )
{
    return collision_( other, *this );
}

Collision LoopCollisionData::visit( CollisionData& other )
{
    return other.visit( *this );
}

Collision LoopCollisionData::visit( PointCollisionData& other )
{
    return other.visit( *this );
}

Collision LoopCollisionData::visit( LoopCollisionData& other )
{
    return no_collision();
}

Collision collision( CollisionData& a, CollisionData& b )
{
    return a.visit( b );
}

Collision collision_( LoopCollisionData& field, PointCollisionData& gunman )
{
    Collision c;
    c.isOccuring = false;

    const Vector<float,2> dist = gunman.pos() - field.pos();

    if( magnitude(dist) >= field.inner_radius() ) {
        c.isOccuring = true;

        float diff = magnitude( dist ) - field.inner_radius();
        c.intersection = normalize(dist) * diff;
    }

    return c;
}
