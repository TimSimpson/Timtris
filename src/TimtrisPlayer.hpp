#ifndef TimtrisPLAYER_HPP
#define TimtrisPLAYER_HPP

#include "GameOverBox.h"
#include "TimtrisGrid.h"
#include "TimtrisPlayer.h"
#include "TimtrisScore.h"
#include "TimtrisTetrad.h"

namespace lp3 { namespace timtris {

class TimtrisPlayer
{
public:
    TimtrisPlayer(int playerIndex,
                 TimtrisScore * playerScore,
                 GameOverBox * goBox);
    void SetControl(LpInputControl * control);
    void SetGrid(TimtrisGrid * grid);
    void Update();
private:
    bool allowGameOnPressStart;
    bool checkForLine(bool countPlayerTetrads);
    bool checkForLines;
    int colorTile;
    LpInputControl * control;
    void DestroyPlayerSquares();
    TimtrisGrid * grid;
    ClockTimeInt fallTime;
    ClockTimeInt fallSpeed;
    bool gameOver;
    GameOverBox * gameOverBox;
    ClockTimeInt gameOverTime;
    ClockTimeInt moveTime;
    ClockTimeInt moveSpeed;
    int playerIndex;
    TimtrisScore * score;
    void startNewPiece();
    TimtrisTetrad * tetrad;
    ClockTimeInt timeToDie;
    int x;
    int y;

    SfxChannel * sfxChannel;
    SfxSample * soundRotate;
    SfxSample * soundHit;
    SfxSample * soundBottom;
    SfxSample * soundLine;
    SfxSample * soundGameover;
    SfxSample * soundPeur;
    SfxSample * soundStp;
};

}   }   // end lp3::timtris

#endif
