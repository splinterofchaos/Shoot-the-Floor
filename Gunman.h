
#include "Actor.h"

class Gunman : public Actor<float,2>
{
    typedef Actor<float,2> parent;

    value_type pointingDirection; // The angle of the gunman's arm.

    /* POINTS MAP
     *      O    Head drawn elsewhere.
     *      |    
     *     / \   
     *
     * Draw order: SHOULDER, WAIST, LEFT FOOT, WAIST, RIGHT FOOT.
     */
    enum { SHOULDER, WAIST1, LEFT_FOOT, WAIST2, RIGHT_FOOT, N_POINTS };
    vector_type points[ N_POINTS ];

    vector_type arm_vector();

  public:
    bool controlledByPlayer; // True if this is a pawn of the player.

    Gunman( const vector_type& pos, bool controlledByPlayer=false );

    void move( int quantum );
    void draw();
};
