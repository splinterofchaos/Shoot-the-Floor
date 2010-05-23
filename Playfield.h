
#include "Actor.h"
#include "Collision.h"
#include "draw_shape.h"
#include "glpp.h"
#include "math\Vector.h"

#pragma once

class Segment
{
    static const int MAX_HEALTH = 100;

    typedef GLfloat value_type;

  public:
    int health;

    Vector<value_type,3> baseColor;
    Vector<value_type,3> actualColor; // Calculated in Segment::color().

    Segment()
        : health( MAX_HEALTH ), baseColor( vector<value_type>(0,0,1) ),
        actualColor( baseColor )
    {
    }

    Vector<value_type,3> calculate_color()
    {
        float healthRatio = float(health) / float(MAX_HEALTH);
        for( int i=0; i < 3; i++ ) 
            actualColor[i] = baseColor[i] * healthRatio;
        return actualColor;
    }
};

// Playfield is the arena on which gunmen will face. It is a segmented circle.
class Playfield : public Actor<float,2>, public LoopCollisionData
{
    typedef Actor<float,2> parrent;

    // Playfield is made of two parallel arrays, one handling each of its
    // segments health, the other handling the actual locations of the
    // vertices. The vertices of segment S_i are [V_2i,V_(2i+4)].
    std::vector< Segment >         segments;
    std::vector< Vector<float,2> > vertices;

    void init_vertices( float nVertices );

  public:
    Playfield( const vector_type& pos, value_type radius, unsigned int nVertices );

    void draw();

    void segment_test( int quantum );

    void move( int quantum );

    void collide( const vector_type& );

    // For LoopCollisionData
    CollisionData& collision_data();
    Vector<float,2> pos();
    value_type inner_radius();
    value_type outer_radius();
};
