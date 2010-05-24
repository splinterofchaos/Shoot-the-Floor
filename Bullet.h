
#include "Actor.h"
#include "Collision.h"
#include "draw_shape.h"

class Bullet : public Actor<float,2>, public PointCollisionData
{
    typedef Actor<float,2> parent;

    static const value_type BULLET_SPEED = 0.2;

  public:
    Bullet( const vector_type& pos, value_type direction )
        : parent( pos )
    {
        v = vector( std::cos(direction), std::sin(direction) ) * BULLET_SPEED;
        scale = 2;
    }

    void move( int quantum )
    {
        parent::move( quantum );
    }

    void draw()
    {
        glTranslatef( s.x(), s.y(), 0 );
        glColor3f( 0, 0, 0 );
        draw_circle( scale, 6 );
        glLoadIdentity();
    }

    void collide( const vector_type& )
    {
        killMe = true;
    }

    // For PointCollisionData
    CollisionData& collision_data() 
    {
        return *this;
    }

    Vector<float,2> pos()
    {
        return s;
    }
};
