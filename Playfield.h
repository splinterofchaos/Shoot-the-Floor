
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

    void init_vertices( float nVertices )
    {
        static const float ROTATION = 2 * 3.145 / nVertices;
        static const float TANGENTAL_FACTOR = std::tan( ROTATION );
        static const float RADIAL_FACTOR    = std::cos( ROTATION );

        // Two extra vertices are needed since the start vertices are repeated
        // at the end.
        vertices.reserve( nVertices * 2 + 2 );

        // Radial vectors.
        Vector<float,2> rv1 = vector( scale, 0.0f );
        Vector<float,2> rv2 = vector( scale - scale/30, 0.0f );

        // Note that these points should be repeated at the end.
        vertices.push_back( rv1 );
        vertices.push_back( rv2 );
        for( unsigned int i=0; i < nVertices; i++ )
        {
            // Clockwise tangents to the circle. Since the Y-axis is reversed,
            // starting at angle zero to the X-axis, counter-clockwise is a
            // positive rotation.
            Vector<float,2> tv1 = vector( -rv1.y(), rv1.x() );
            Vector<float,2> tv2 = vector( -rv2.y(), rv2.x() );

            rv1 += tv1 * TANGENTAL_FACTOR;
            rv1 *= RADIAL_FACTOR;

            rv2 += tv2 * TANGENTAL_FACTOR;
            rv2 *= RADIAL_FACTOR;

            vertices.push_back( rv1 );
            vertices.push_back( rv2 );
        }
    }

  public:
    Playfield( const vector_type& pos, value_type radius, unsigned int nVertices )
        : parrent( pos ), segments( nVertices, Segment() )
    {
        scale = radius;

        init_vertices( nVertices );
    }

    void draw()
    {

        using namespace glpp;
        translate( s.x(), s.y(), 0 );

        glEnableClientState( GL_VERTEX_ARRAY );
        {
            // Draw each segment separately as a rectangle.
            for( unsigned int i=0; i < segments.size(); i++ ) 
            {

                Vector<GLfloat,3>& c = segments[i].actualColor;
                glColor3f( c.x(), c.y(), c.z() );
                glVertexPointer( 2, GL_FLOAT, 0, &vertices[2*i] );

                /* Note drawing order:
                 * 2-3 <- drawing order used   3-2
                 *  \                            |
                 * 0-1    quad drawing order-> 0-1
                 * 
                 * Using triangle strip, (0,1,2) makes a triangle, 
                 * then (1,2,3).
                 */
                glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
            }
        }
        glDisableClientState( GL_VERTEX_ARRAY );
        glLoadIdentity();
    }

    void segment_test( int quantum )
    {
        static unsigned int i = 0;
        if( --segments[i].health <= 0 ) {
            segments[i].health = 0;
            if( ++i >= segments.size() )
                i = 0;
        }
        segments[i].calculate_color();
    }

    void move( int quantum )
    {
        segment_test( quantum );
    }

    void collide( const vector_type& ) 
    {
    }

    // For LoopCollisionData
    CollisionData& collision_data() 
    {
        return *this;
    }

    Vector<float,2> pos() const
    {
        return s;
    }

    value_type inner_radius()
    {
        return scale - scale/30;
    }

    value_type outer_radius()
    {
        return scale;
    }
};
