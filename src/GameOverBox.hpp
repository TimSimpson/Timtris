#ifndef TIMTRIS_GAMEOVERBOX_HPP
#define TIMTRIS_GAMEOVERBOX_HPP

#include <lp3/gfx.hpp>

namespace lp3 { namespace timtris {

class GameOverBox
{
public:
    GameOverBox(gfx::TileMap & textTiles);
    GfxImageID GetImageID();
    int GetState();
    void MakeInvisible();
    void StartCountDown(int initial);
    void StartGameOver();
    void StartPressStart();
    void StartWillContinue();
    bool Update(); // returns true if count down is over.
    ~GameOverBox();
private:
    ClockTimeInt animationTimer;
    int countDown;
	gfx::TileMap layer;
    int state;
};

}   }   // end lp3::timtris

#endif
