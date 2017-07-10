#include "TimtrisGrid.hpp"


namespace lp3 {	namespace timtris {

TimtrisGrid::TimtrisGrid(int gridWidth, int gridHeight,
                       int squareWidth, int squareHeight,
                       int tileCount)
:	finishFlashTime{0},
	flashLine{0},
	flashColor{0},
	grid{ gridWidth * gridHeight, TimtrisSquare{} },
	gridWidth{ gridWidth },
	gridHeight{ gridHeight },
	squareWidth{squareWidth},
	squareHeight{squareHeight},
	// tiles{ tiles },
	tileCount{ tileCount },
	tile_map{ { squareWidth, squareHeight },{ gridWidth, gridHeight } }
{
	// Original code created a sprite that it drew tiles to,
	// with gridWidth * squareWidth,
	// gridHeight * squardHeight
}


////GfxSprite * TimtrisGrid::GetSprite()
////{
////    return sprite;
////}

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

	// TODO: make asserts?
    if (x < 0 || x >= gridWidth)
    {
        LP3_LOG_ERROR("X is invalid in TetridGrid::GetGrid: %d", x);
		LP3_ASSERT(false); // in old code but seems like it should not happen
        return square;
    }
    // why -5? Because tetrads start above top of grid.
    if (y < 0 || y >= gridHeight)
    {
        if (y < -5)
        {
			LP3_LOG_ERROR("Y is invalid in TetridGrid::GetGrid. %d", y);
        }
		LP3_ASSERT(false); // in old code but seems like it should not happen
        return square;
    }
    return grid[(y * gridWidth) + x];
}

void TimtrisGrid::SetGrid(int x, int y, TimtrisSquare square)
{
	// TODO: Make asserts?
    if (x < 0 || x >= gridWidth)
    {
		LP3_LOG_ERROR("X is invalid in TetridGrid::SetGrid. %d", x);
		LP3_ASSERT(false); // in old code but seems like it should not happen
        return;
    }
    if (y < 0 || y >= gridHeight)
    {
        if (y < -5)
        {
			LP3_LOG_ERROR( "Y is invalid in TetridGrid::SetGrid. %d", y);
			LP3_ASSERT(false); // in old code but seems like it should not happen
        }
        return;
    }
	tile_map.write({ x, y }, square.Color);
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
     finishFlashTime = 1000;
     flashLine = line;
     flashColor = 1;
}

bool TimtrisGrid::Update(const std::int64_t ms)
{
    TimtrisSquare square;
	if (finishFlashTime >= 0) {
		finishFlashTime -= ms;
		if (finishFlashTime <= 0) {
			// If a line was flashing and the time for flashing has ended,
			// eliminate the line and move everything down.
			if (flashColor > 0)
			{
				flashColor = 0;
				for (int i = 0; i < GridWidth(); i++)
				{
					square = GetGrid(i, flashLine);
					square.Color = flashColor;
					SetGrid(i, flashLine, square);
				}
				// Move line up.
				for (int j = flashLine; j > 0; j--)
				{
					for (int i = 0; i < GridWidth(); i++)
					{
						SetGrid(i, j, GetGrid(i, j - 1));
					}
				}
			}
			return true;
		}
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

}	}
