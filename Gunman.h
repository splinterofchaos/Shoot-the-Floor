
#include "Actor.h"

class Gunman : public Actor<float>
{
    typedef Actor<float> parent;

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
         */
        enum { SHOULDER, WAIST, LEFT_FOOT, RIGHT_FOOT, N_POINTS };
        static const vector_type points[ N_POINTS ];
        points[ SHOULDER ]   = vector( value_type(0),      -3*scale );
        points[ WAIST ]      = vector( value_type(0),        -scale );
        points[ RIGHT_FOOT ] = vector(         scale, value_type(0) );
        points[ LEFT_FOOT ]  = vector(        -scale, value_type(0) );
        static const GLint indices[] = { 
            WAIST, SHOULDER, WAIST, LEFT_FOOT, WAIST, RIGHT_FOOT
        };

        glTranslatef( s.x(), s.y(), 0 );

        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_INDEX_ARRAY );
        {
            glColor3f( 0, 0, 0 );
            glVertexPointer( 2, GL_FLOAT, 0, points );
            glIndexPointer( GL_INT, 0, indices );
            glDrawArrays( GL_LINES, 0, N_POINTS );
        }
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_INDEX_ARRAY );

        glLoadIdentity();
    }
};
