
#include "Actor.h"
#include "draw_shape.h"
#include "glpp.h"
#include "math\Vector.h"

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

template< size_t N >
class Playfield : public Actor<float,2>
{
    typedef Actor<float,2> parrent;

    Segment segments[N];

  public:
    Playfield( const vector_type& pos, const value_type radius )
        : parrent( pos )
    {
        scale = radius;
    }

    void draw()
    {
        // TODO: This is a straight copy/paste of draw_loop. The vertices only
        // need to be calculated once (like in the ctor). Move this code there
        // and clean it up.
        float rad1 = scale - scale / 30;
        float rad2 = scale;
        float nVertecies = N;
        const float ROTATION = 2 * 3.145 / nVertecies;
        const float TANGENTAL_FACTOR = std::tan( ROTATION );
        const float RADIAL_FACTOR    = std::cos( ROTATION );

        // Radial vectors.
        Vector<float,2> rv1;
        Vector<float,2> rv2;

        rv1.x( rad1 ); rv1.y( 0.0f );
        rv2.x( rad2 ); rv1.y( 0.0f );

        std::vector< Vector<float,2> > verteces;
        // Two extra vertices are needed since the start vertices are repeated
        // at the end.
        verteces.reserve( nVertecies * 2 + 2 );

        // Note that these points should be repeated at the end.
        verteces.push_back( rv1 );
        verteces.push_back( rv2 );
        for( unsigned int i=0; i < nVertecies; i++ )
        {
            // Counter-clockwise tangents to the circle.
            Vector<float,2> tv1 = vector( rv1.y(), -rv1.x() );
            Vector<float,2> tv2 = vector( rv2.y(), -rv2.x() );

            rv1 += tv1 * TANGENTAL_FACTOR;
            rv1 *= RADIAL_FACTOR;

            rv2 += tv2 * TANGENTAL_FACTOR;
            rv2 *= RADIAL_FACTOR;

            verteces.push_back( rv1 );
            verteces.push_back( rv2 );
        }

        using namespace glpp;
        translate( s.x(), s.y(), 0 );

        glEnableClientState( GL_VERTEX_ARRAY );
        {
            // Draw each segment separately as a rectangle.
            for( int i=0; i < N; i++ ) 
            {

                Vector<GLfloat,3>& c = segments[i].actualColor;
                glColor3f( c.x(), c.y(), c.z() );
                glVertexPointer( 2, GL_FLOAT, 0, &verteces[2*i] );

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
        static int i = 0;
        if( --segments[i].health <= 0 ) {
            segments[i].health = 0;
            if( ++i == N )
                i = 0;
        }
        segments[i].calculate_color();
    }

    void move( int quantum )
    {
        segment_test( quantum );
    }
};
