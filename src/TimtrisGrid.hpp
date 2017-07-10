#ifndef TimtrisGRID_H
#define TimtrisGRID_H

#include "../Engine/Gfx.h"
#include "../Engine/SysClock.h"
#include "TimtrisTetrad.h"

class TimtrisSquare
{
public:
    int PlayerIndex;
    int Color;
    bool Stopped;

    TimtrisSquare()
    {
        PlayerIndex = -1;
        Stopped = false;
    }
};

class TimtrisGrid
{
public:
    TimtrisGrid(int gridWidth, int gridHeight,
               int squareWidth, int squareHeight,
               GfxMapImageID tiles, int tileCount);
    void Clear();
    bool CheckForSpace(int x, int y, TimtrisTetrad * tetrad, int playerIndex);
    void EliminateLine(int line);
    TimtrisSquare GetGrid(int x, int y);
    GfxSprite * GetSprite();
    int GridHeight();
    int GridWidth();
    void PlaceTetrad(int x, int y, TimtrisTetrad * tetrad, TimtrisSquare square);
    void RemoveTetrad(int x, int y, TimtrisTetrad * tetrad);
    void SetGrid(int x, int y, TimtrisSquare tile);
    // If this function returns false, it's not safe to proceed.
    // So don't.
    bool Update();
    ~TimtrisGrid();
private:
    ClockTimeInt finishFlashTime;
    int flashLine;
    int flashColor;
    TimtrisSquare * grid;//[17][10];
    int gridWidth;
    int gridHeight;
    int squareWidth;
    int squareHeight;
    GfxSprite * sprite;
    GfxMapImageID tiles;
    int tileCount;

};
#endif
