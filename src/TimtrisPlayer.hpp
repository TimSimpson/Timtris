#ifndef TimtrisPLAYER_H
#define TimtrisPLAYER_H

#include <TimtrisScore.hpp>
#include <TimtrisTetrad.hpp>

class TimtrisPlayer
{
public:

	TimtrisPlayer(int playerIndex);
                  //TimtrisScore * playerScore,
                  //GameOverBox * goBox);
    //void SetControl(LpInputControl * control);
    //void SetGrid(TimtrisGrid * grid);
    void Update();
private:
    // bool allowGameOnPressStart;
    // bool checkForLine(bool countPlayerTetrads);
    // bool checkForLines;
    // int colorTile;
    // LpInputControl * control;
    // void DestroyPlayerSquares();
    // TimtrisGrid * grid;
    // ClockTimeInt fallTime;
    // ClockTimeInt fallSpeed;
    // bool gameOver;
    // GameOverBox * gameOverBox;
    // ClockTimeInt gameOverTime;
    // ClockTimeInt moveTime;
    // ClockTimeInt moveSpeed;
    // int playerIndex;
    // TimtrisScore * score;
    // void startNewPiece();
    // TimtrisTetrad * tetrad;
    // ClockTimeInt timeToDie;
    // int x;
    // int y;

    // SfxChannel * sfxChannel;
    // SfxSample * soundRotate;
    // SfxSample * soundHit;
    // SfxSample * soundBottom;
    // SfxSample * soundLine;
    // SfxSample * soundGameover;
    // SfxSample * soundPeur;
    // SfxSample * soundStp;
};

#endif
