
#include "Actor.h"

class Gunman : public Actor<float,2>
{
    typedef Actor<float,2> parent;

  public:
    Gunman( const vector_type& pos )
        : parent( pos )
    {
        scale = 10;
    }

    void move()
    {
        // TODO
    }

    void draw()
    {
        /* POINTS MAP
         *      O    HEAD: not drawn here.
         *      |    SHOULDER: 3, WAIST: 2
         *     / \   LEFT FOOT: 0, RIGHT: 1
         *
         * Draw order: SHOULDER, WAIST, LEFT FOOT, WAIST, RIGHT FOOT.
         */
        enum { SHOULDER, WAIST1, LEFT_FOOT, WAIST2, RIGHT_FOOT, N_POINTS };
        static vector_type points[ N_POINTS ];
        points[ SHOULDER   ] = vector( value_type(0),      -3*scale );
        points[ WAIST1     ] = vector( value_type(0),        -scale );
        points[ RIGHT_FOOT ] = vector(         scale, value_type(0) );
        points[ LEFT_FOOT  ] = vector(        -scale, value_type(0) );
        points[ WAIST2 ] = points[ WAIST1 ];

        glTranslatef( s.x(), s.y(), 0 );

        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_INDEX_ARRAY );
        {
            glColor3f( 0, 0, 0 );
            glVertexPointer( 2, GL_FLOAT, 0, points );
            glDrawArrays( GL_LINE_STRIP, 0, N_POINTS );
        }
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_INDEX_ARRAY );

        glLoadIdentity();
    }
};
