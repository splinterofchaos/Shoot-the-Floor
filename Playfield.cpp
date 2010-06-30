
#include "Playfield.h"

#include <GL/gl.h>

void Playfield::init_vertices( float nVertices )
{
    static const float ROTATION = 2 * 3.145 / nVertices;
    static const float TANGENTAL_FACTOR = std::tan( ROTATION );
    static const float RADIAL_FACTOR    = std::cos( ROTATION );

    // Two extra vertices are needed since the start vertices are repeated
    // at the end.
    vertices.reserve( nVertices * 2 + 2 );

    // Radial vectors.
    Vector<float,2> rv1 = vector( inner_radius(), 0.0f );
    Vector<float,2> rv2 = vector( outer_radius(), 0.0f );

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

Playfield::Playfield( const vector_type& pos, value_type radius, unsigned int nVertices )
    : parrent( pos ), segments( nVertices, Segment() )
{
    scale = radius;

    init_vertices( nVertices );
}

void Playfield::draw()
{
    glTranslatef( s.x(), s.y(), 0 );

    glEnableClientState( GL_VERTEX_ARRAY );
    {
        // Draw each segment separately as a rectangle.
        for( unsigned int i=0; i < segments.size(); i++ ) 
        {

            Vector<GLfloat,3>& c = segments[i].color();
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

void Playfield::segment_test( int quantum )
{
    static unsigned int i = 0;
    if( segments[i].health_rel(-1) == 0 )
        if( ++i >= segments.size() )
            i = 0;
}

void Playfield::move( int quantum ) { segment_test( quantum ); }

void Playfield::collide( Actor& collider ) { }

CollisionData& Playfield::collision_data() { return *this; }

Vector<float,2>& Playfield::pos() { return s; }

Playfield::value_type Playfield::inner_radius() { return scale - scale/30; }
Playfield::value_type Playfield::outer_radius() { return scale; }
