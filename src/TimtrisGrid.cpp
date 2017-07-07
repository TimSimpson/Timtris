#ifndef TimtrisGRID_CPP
#define TimtrisGRID_CPP

#include "TimtrisGrid.h"
#include "../Engine/ErrOut.h"

static GfxScreen * screen = nullptr;

TimtrisGrid::TimtrisGrid(int gridWidth, int gridHeight,
                       int squareWidth, int squareHeight,
                       GfxMapImageID tiles, int tileCount)
{
   this->gridWidth = gridWidth;
   this->gridHeight = gridHeight;
   this->squareWidth = squareWidth;
   this->squareHeight = squareHeight;
   sprite = new GfxSprite();
   sprite->Width = gridWidth * squareWidth;
   sprite->Height = gridHeight * squareHeight;
   GfxPixel pixel;
   pixel.blue = 255;
   pixel.red  = 0;
   pixel.green = 0;
   pixel.alpha = 255;
   sprite->ImageID = GfxGetScreen(0)->GetImageID(pixel,
                   (int)sprite->Width, (int)sprite->Height);
   sprite->Frame.x1 = 0;
   sprite->Frame.y1 = 0;
   sprite->Frame.x2 = (int)sprite->Width - 1;
   sprite->Frame.y2 = (int)sprite->Height - 1;

   this->tiles = tiles;
   finishFlashTime = SysClockGetTime();
   this->tileCount = tileCount;

   flashLine = 0;

   grid = new TimtrisSquare[gridWidth * gridHeight];
   if (screen == nullptr)
   {
       screen = GfxGetScreen(0);
   }
}

TimtrisGrid::~TimtrisGrid()
{
    delete [] grid;
    GfxGetScreen(0)->DeleteImageID(sprite->ImageID);
    delete sprite;
}

GfxSprite * TimtrisGrid::GetSprite()
{
    return sprite;
}

int TimtrisGrid::GridHeight()
{
    return gridHeight;
}

int TimtrisGrid::GridWidth()
{
    return gridWidth;
}

void TimtrisGrid::Clear()
{
    TimtrisSquare square;
    square.PlayerIndex = -1;
    square.Color = 0;
    for (int j = 0; j < GridHeight(); j ++)
    {
        for (int i = 0; i < GridWidth(); i ++)
        {
            SetGrid(i, j, square);
        }
    }
}

TimtrisSquare TimtrisGrid::GetGrid(int x, int y)
{
    TimtrisSquare square;
    square.Color = -1;
    square.PlayerIndex = -1;

    if (x < 0 || x >= gridWidth)
    {
        ErrOutWriteLine("X is invalid in TetridGrid::GetGrid.");
        ErrOutWriteNumber(x);
        return square;
    }
    // why -5? Because tetrads start above top of grid.
    if (y < 0 || y >= gridHeight)
    {
        if (y < -5)
        {
            ErrOutWriteLine("Y is invalid in TetridGrid::GetGrid.");
            ErrOutWriteNumber(y);
        }
        return square;

    }
    return grid[(y * gridWidth) + x];
}

void TimtrisGrid::SetGrid(int x, int y, TimtrisSquare square)
{

    if (x < 0 || x >= gridWidth)
    {
        ErrOutWriteLineP(0, "X is invalid in TetridGrid::SetGrid.");
        ErrOutWriteNumber(x);
        return;
    }
    if (y < 0 || y >= gridHeight)
    {
        if (y < -5)
        {
            ErrOutWriteLineP(0, "Y is invalid in TetridGrid::SetGrid.");
            ErrOutWriteNumber(y);
        }
        return;
    }
    screen->DrawMap(sprite->ImageID, tiles,
            x * squareWidth,
            y * squareHeight,
            (u16)(square.Color));
    grid[(y * gridWidth) + x] = square;
}

void TimtrisGrid::PlaceTetrad(int x, int y, TimtrisTetrad * tetrad, TimtrisSquare square)
{
    for (int j = y; j < y + tetrad->Height() && j < gridHeight; j++)
    {
        for (int i = x; i < x + tetrad->Width() && i < gridWidth; i ++)
        {
            if (tetrad->Block(i - x, j - y))
            {
                SetGrid(i, j, square);
            }
        }
    }
}

void TimtrisGrid::RemoveTetrad(int x, int y, TimtrisTetrad * tetrad)
{
    // - 1 is invisible.
    TimtrisSquare square;
    square.Color = 0;
    square.PlayerIndex = -1;
    PlaceTetrad(x, y, tetrad, square);
}

bool TimtrisGrid::CheckForSpace(int x, int y, TimtrisTetrad * tetrad, int playerIndex)
{
    for (int j = y; j < y + tetrad->Height(); j++)
    {
        for (int i = x; i < x + tetrad->Width(); i ++)
        {
            if (tetrad->Block(i - x, j - y))
            {
               // Again, there's imaginary space at the top of the grid.
               // If the area isn't outside the grid;
               // The color is less than zero & we're counting moving tetrads
               //    or the color is less than zero and has STOPPED
               //
               if (i < 0 || i >= gridWidth || j < -5 || j >= gridHeight
                   ||
                   ((GetGrid(i, j).Stopped || GetGrid(i, j).PlayerIndex != playerIndex)
                     && GetGrid(i, j).PlayerIndex != -1)
                   )
               {
                   return false;
               }
            }
        }
    }
    return true;
}


void TimtrisGrid::EliminateLine(int line)
{
     // Turn on flash time so the grid will handle flashing.
     // Set it to stop flashing in some time + SysClockGetTime().
     finishFlashTime = SysClockGetTime() + 1000;
     flashLine = line;
     flashColor = 1;
}

bool TimtrisGrid::Update()
{
    TimtrisSquare square;
    if (SysClockGetTime() > finishFlashTime)
    {   // If a line was flashing and the time for flashing has ended,
        // eliminate the line and move everything down.
        if (flashColor > 0)
        {
            flashColor = 0;
            for (int i = 0; i < GridWidth(); i ++)
            {
                square = GetGrid(i, flashLine);
                square.Color = flashColor;
                SetGrid(i, flashLine, square);
            }
            // Move line up.
            for (int j = flashLine; j > 0; j --)
            {
                for (int i = 0; i < GridWidth(); i ++)
                {
                    SetGrid(i, j, GetGrid(i, j - 1));
                }
            }
        }
        return true;
    }
    for (int i = 0; i < GridWidth(); i ++)
    {
        square = GetGrid(i, flashLine);
        square.Color = flashColor;
        SetGrid(i, flashLine, square);
    }
    flashColor ++;
    if (flashColor >= tileCount)
       flashColor = 1;
    return false;
}

#endif
