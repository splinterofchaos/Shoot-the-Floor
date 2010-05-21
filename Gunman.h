
#include "Actor.h"

class Gunman : public Actor<float,2>
{
    typedef Actor<float,2> parent;

    value_type pointingDirection; // The angle of the gunman's arm.

    vector_type hand_point()
    {
        static const value_type ARM_LENGTH = scale;

        vector_type unit = vector (
            std::cos( pointingDirection ), std::sin( pointingDirection )
        );

        return unit * ARM_LENGTH;
    }

  public:
    Gunman( const vector_type& pos )
        : parent( pos ), pointingDirection(0)
    {
        scale = 10;
    }

    void move( int quantum )
    {
        // TODO: This.
        pointingDirection += 0.001;
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
        points[ RIGHT_FOOT ] = vector(       scale/2, value_type(0) );
        points[ LEFT_FOOT  ] = vector(      -scale/2, value_type(0) );
        points[ WAIST2 ] = points[ WAIST1 ];

        vector_type armPoints[2] = {
            points[ SHOULDER ], points[ SHOULDER ] + hand_point()
        };

        glTranslatef( s.x(), s.y(), 0 );

        glEnableClientState( GL_VERTEX_ARRAY );
        {
            glColor3f( 0, 0, 0 );
            
            glVertexPointer( 2, GL_FLOAT, 0, points );
            glDrawArrays( GL_LINE_STRIP, 0, N_POINTS );

            glVertexPointer( 2, GL_FLOAT, 0, armPoints );
            glDrawArrays( GL_LINE_STRIP, 0, 2 );

            const int CIRCLE_PIXELS = 10;
            glTranslatef( 0, -3*scale - CIRCLE_PIXELS, 0 );
            draw_circle( CIRCLE_PIXELS, 11 );
        }
        glDisableClientState( GL_VERTEX_ARRAY );

        glLoadIdentity();
    }
};
