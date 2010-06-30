
#include "Actor.h"

#pragma once

class Segment
{
    static const int MAX_HEALTH = 100;

    typedef GLfloat value_type;
    typedef Vector<value_type,3> Color;

    int hp;

    Color baseColor;
    Color actualColor;

  public:
    Segment()
        : hp( MAX_HEALTH ), baseColor( vector<value_type>(0,0,1) ),
        actualColor( baseColor )
    {
    }

    int health()
    {
        return hp;
    }

    int health_rel( int dHealth )
    {
        hp += dHealth;
        if( hp < 0 )
            hp = 0;

        for( int i=0; i < Color::static_size; i++ ) 
            actualColor[i] = baseColor[i] * ((float)hp/MAX_HEALTH);

        return hp;
    }

    Color& color()
    {
        return actualColor;
    }
};

class Playfield : public Actor, LoopCollisionData
{
    typedef Actor parrent;

    std::vector< Segment > segments;
    std::vector< Vector<float,2> > vertices;

    void init_vertices( float nVertices );

  public:
    Playfield( const vector_type& pos, value_type radius, unsigned int nVertices );

    void draw();
    void move( int quantum );

    void segment_test( int quantum );

    void collide( Actor& collider );
    CollisionData& collision_data();
    Vector<float,2>& pos();
    float inner_radius();
    float outer_radius();
};
