#ifndef GAMEOVERBOX_H
#define GAMEOVERBOX_H

#include "../Engine/GfxLayer.h"
#include "../Engine/SysClock.h"

namespace lp3 { namespace timtris {

class GameOverBox
{
public:
    GameOverBox(GfxScreen * screen, GfxMapImageID textTiles);
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
    GfxLayer * layer;
    int state;
};

}   }   // end lp3::timtris

#endif
