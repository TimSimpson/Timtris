#ifndef TimtrisSCORE_H
#define TimtrisSCORE_H

#include <lp3/gfx.hpp>

namespace lp3 { namespace timtris {

class TimtrisScore
{
public:
    TimtrisScore();
    void AddLinePoints();
    void AddPoints(int number);
    int GetScore();
    gfx::TileMap & get_layer();
private:
    gfx::TileMap layer;
    int score;
    int lines;
};

}   }

#endif
