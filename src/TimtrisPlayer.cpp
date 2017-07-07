#include "TimtrisPlayer.h"
#include "../Engine/LpInput.h"
#include "../Engine/ErrOut.h"
#include "../Engine/Sfx.h"

TimtrisPlayer::TimtrisPlayer(int playerIndex,
                           TimtrisScore * playerScore,
                           GameOverBox * goBox)
{
    control = nullptr;
    x = 0;
    y = 0;
    colorTile = 2;
    tetrad = new TimtrisTetrad(1, 0);
    grid = nullptr;
    fallTime = 0;
    moveTime = 0;
    fallSpeed = 750;
    moveSpeed = 100;
    timeToDie = 0;
    checkForLines = false;
    startNewPiece();
    this->score = playerScore;
    this->gameOverBox = goBox;
    allowGameOnPressStart = true;//false;
    gameOverBox->StartWillContinue();
    gameOver = false;

    // Sounds

     sfxChannel = new SfxChannel();

     soundRotate = new SfxSample();
     soundRotate->Load("Timtris\\rotate.wav");
     soundHit = new SfxSample();
     soundHit->Load("Timtris\\hit.wav");
     soundBottom = new SfxSample();
     soundBottom->Load("Timtris\\bottom.wav");
     soundLine = new SfxSample();
     soundLine->Load("Timtris\\line.wav");
     soundGameover = new SfxSample();
     soundGameover->Load("Timtris\\gameover.wav");
     soundPeur = new SfxSample();
     soundPeur->Load("Timtris\\peur.wav");
     soundStp = new SfxSample();
     soundStp->Load("Timtris\\startToPlay.wav");
}

void TimtrisPlayer::SetControl(LpInputControl * control)
{
    this->control = control;
}

void TimtrisPlayer::SetGrid(TimtrisGrid * grid)
{
    this->grid = grid;
}

bool TimtrisPlayer::checkForLine(bool countPlayerTetrads)
{
    bool hasLine;
    for (int j = 0; j < grid->GridHeight(); j ++)
    {
        hasLine = true;
        for (int i = 0; i < grid->GridWidth(); i ++)
        {
            if (
                (countPlayerTetrads && grid->GetGrid(i, j).Color == 0)
                ||
                (!countPlayerTetrads && grid->GetGrid(i, j).PlayerIndex < 0)
               )
            {
                hasLine = false;
                break;
            }
        }
        if (hasLine)
        {
            sfxChannel->SetToSource(soundLine);
            sfxChannel->Play(true);
            grid->EliminateLine(j);
            return true;
        }
    }
    return false;
}

void TimtrisPlayer::startNewPiece()
{
    if (tetrad != nullptr && grid != nullptr)
    {
        int tetradIndex = (int)SysClockGetRandom(7);
        int rotIndex = (int)SysClockGetRandom(4);
        colorTile = tetradIndex + 1;//(int)SysClockGetRandom(7);
        tetrad->SetTetrad(tetradIndex);
        tetrad->SetRotation(rotIndex);
        x = (grid->GridWidth() - tetrad->Width()) / 2;
        y = (tetrad->Height() * -1);
    }
}

void TimtrisPlayer::DestroyPlayerSquares()
{
    for (int j = 0; j < grid->GridHeight(); j ++)
    {
         for (int i = 0; i < grid->GridWidth(); i ++)
         {
             if (grid->GetGrid(i, j).PlayerIndex == playerIndex)
             {
                 TimtrisSquare s;
                 s.PlayerIndex = -1;
                 s.Stopped = true;
                 s.Color = 0;
                 grid->SetGrid(i, j, s);
             }
         }
    }
}

void TimtrisPlayer::Update()
{
    if (allowGameOnPressStart)
    {
        gameOverBox->Update();
        if (control->Button(0))
        {
            gameOver = false;
            allowGameOnPressStart = false;
            gameOverBox->MakeInvisible();
        }
        return;
    }
         ErrOutWriteLine("********* b");
    if (gameOver)
    {
        switch(gameOverBox->GetState())
        {
            case 1:
            {
                 if (gameOverBox->Update())
                 {
                      gameOverBox->StartCountDown(10);
                 }
                 break;
            }
            case 2:
            {
                 if (!gameOverBox->Update())
                 {
                    if (control->Button(0))
                    {
                        gameOverBox->StartWillContinue();
                        sfxChannel->SetToSource(soundPeur);
                        sfxChannel->Play(true);
                    }
                    return;
                 }
                 else
                 {
                     DestroyPlayerSquares();
                     gameOverBox->StartPressStart();
                     allowGameOnPressStart = true;
                     gameOverBox->StartPressStart();
                     sfxChannel->SetToSource(soundStp);
                     sfxChannel->Play();
                 }
                 break;
            }
            case 3:
            {
                 if (gameOverBox->Update())
                 {
                     // Put func here to restart everything.
                     // By clearing all of the player's tetrads from the screen.
                     DestroyPlayerSquares();
                     gameOver = false;
                 }
                 break;
            }
            default:
                break;
        }
             ErrOutWriteLine("********* c");
        return;
    }

     ErrOutWriteLine("********* d");

    if (checkForLines)
    {
        if (checkForLine(true))
        {
            score->AddLinePoints();
            return;
        }
        checkForLines = false;
        startNewPiece();
        // Check here to see if that new piece will fall into trouble...
        // If there's no space, it's game over time baby.
        if (!grid->CheckForSpace(x, y + 1, tetrad, playerIndex))
        {
            gameOver = true;
            sfxChannel->SetToSource(soundGameover);
            sfxChannel->Play();
            gameOverBox->StartGameOver();
        }
        return;
    }

    // This is to eliminate the flicker problem on DC.
    bool move;
    int oldX, oldY, oldRotation;
    move = false;
    oldX = x; oldY = y;
    oldRotation = tetrad->GetRotation();

    if (SysClockGetTime() >= moveTime)
    {
        //grid->RemoveTetrad(x, y, tetrad);
        if (control->Left())
        {
            if (grid->CheckForSpace(x - 1, y, tetrad, playerIndex))
            {
                //grid->RemoveTetrad(x, y, tetrad);
                x --;
                move = true;
            }
            else
            {
                sfxChannel->SetToSource(soundHit);
                sfxChannel->Play();
            }
            moveTime = SysClockGetTime() + moveSpeed;
        }
        if (control->Right())
        {
            if (grid->CheckForSpace(x + 1, y, tetrad, playerIndex))
            {
                //grid->RemoveTetrad(x, y, tetrad);
                x ++;
                move = true;
            }
            else
            {
                sfxChannel->SetToSource(soundHit);
                sfxChannel->Play();
            }
            moveTime = SysClockGetTime() + moveSpeed;
        }
        if (control->Button(0))
        {
            move = true;
            // If no space, rotate back.
            tetrad->Rotate(true);
            if (!grid->CheckForSpace(x, y, tetrad, playerIndex))
            {
                tetrad->Rotate(false);
            }
            else
            {
                sfxChannel->SetToSource(soundRotate);
                sfxChannel->Play();
            }
            moveTime = SysClockGetTime() + moveSpeed;
        }

        if (control->Down())
        {
            fallTime = SysClockGetTime();
            moveTime = SysClockGetTime() + moveSpeed;
        }
    }

    bool setNewPiece = false;
    if (SysClockGetTime() >= fallTime)
    {
       // Let tetrad fall down and set flag to check for lines.
       grid->RemoveTetrad(x, y, tetrad);
       if (grid->CheckForSpace(x, y + 1, tetrad, playerIndex))
       {
            move = true;
            y ++;
       }
       else
       {
           // Here, they've noticed that there's no room right below.
           // So we will see if they're being stopped due to a player block
           // and not a block that's already there.  If it's just a player block
           // we will keep them up there and not move them down, not do
           // anything.
           if (!grid->CheckForSpace(x, y + 1, tetrad, playerIndex))
           {
               //Get next piece.
               setNewPiece = true;
               sfxChannel->SetToSource(soundBottom);
                sfxChannel->Play();
           }
       }
       TimtrisSquare square;
       square.Color = colorTile;
       square.PlayerIndex = playerIndex;
       grid->PlaceTetrad(x, y, tetrad, square);
       fallTime = SysClockGetTime() + fallSpeed;
       if (setNewPiece)
       {
           checkForLines = true;
           square.PlayerIndex = playerIndex;
           square.Stopped = true;
           square.Color = colorTile;
           //grid->PlaceTetrad(x, y, tetrad, square);
           move = true;
       }
    }
    if (move)
    {
        int newRotation = tetrad->GetRotation();
        tetrad->SetRotation(oldRotation);
        grid->RemoveTetrad(oldX, oldY, tetrad);
        TimtrisSquare square;
        square.PlayerIndex = playerIndex;
        square.Color = colorTile;
        if (setNewPiece)
        {
            square.Stopped = true;
        }
        tetrad->SetRotation(newRotation);
        grid->PlaceTetrad(x, y, tetrad, square);
    }
}
