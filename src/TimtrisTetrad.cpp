#include "TimtrisTetrad.hpp"


namespace lp3 {	namespace timtris {

//10 x 17
// []              []      []
// [] [][][] [][]  []      []   [][] [][]
// []   []   [][]  [][]  [][] [][]     [][]
// []

static const char tetradI[4 * 1 + 1] =
    "X"
    "X"
    "X"
    "X"
;
static const char tetradT[2 * 3 + 1] =
    "XXX"
    " X "
;
static const char tetradO[2 * 2 + 1] =
    "XX"
    "XX"
;
static const char tetradL[3 * 2 + 1] =
    "X "
    "X "
    "XX"
;
static const char tetradJ[3 * 2 + 1] =
    " X"
    " X"
    "XX"
;
static const char tetradS[2 * 3 + 1] =
    " XX"
    "XX "
;
static const char tetradZ[2 * 3 + 1] =
    "XX "
    " XX"
;

TimtrisTetrad::TimtrisTetrad(int tetradIndex, int rotationIndex)
{
   SetTetrad(tetradIndex);
   SetRotation(rotationIndex);
}


int TimtrisTetrad::Height()
{
    if (rotateIndex == 0 || rotateIndex == 2)
       return height;
    else
        return width;
}

int TimtrisTetrad::Width()
{
    if (rotateIndex == 0 || rotateIndex == 2)
       return width;
    else
        return height;
}

/* Properties. */
int TimtrisTetrad::GetTetrad()
{
    return tetradIndex;
}

void TimtrisTetrad::SetTetrad(int tetradIndex)
{
    this->tetradIndex = tetradIndex;
    switch(tetradIndex)
    {
       case 0:
       {
            blocks = (char *) tetradI;
            width = 1;
            height = 4;
            break;
       }
       case 1:
       {
            blocks = (char *) tetradT;
            width = 3;
            height = 2;
            break;
       }
       case 2:
       {
            blocks = (char *) tetradO;
            width = 2;
            height = 2;
            break;
       }
       case 3:
       {
            blocks = (char *) tetradL;
            width = 2;
            height = 3;
            break;
       }
       case 4:
       {
            blocks = (char *) tetradJ;
            width = 2;
            height = 3;
            break;
       }
       case 5:
       {
            blocks = (char *) tetradS;
            width = 3;
            height = 2;
            break;
       }
       case 6:
       {
            blocks = (char *) tetradZ;
            width = 3;
            height = 2;
            break;
       }
       default:
       {
            blocks = (char *) tetradI;
            width = 0;
            height = 0;
            break;
       }
    }

}

int TimtrisTetrad::GetRotation()
{
    return rotateIndex;
}

void TimtrisTetrad::SetRotation(int rotateIndex)
{
     this->rotateIndex = rotateIndex;
}

void TimtrisTetrad::Rotate(bool clockWise)
{
     if (clockWise)
        rotateIndex ++;
     else
        rotateIndex --;
     while (rotateIndex < 0)
        rotateIndex += 4;
     while (rotateIndex > 3)
        rotateIndex -= 4;
}

// Functions to "Get" the actual data on how the block maps out.
bool TimtrisTetrad::Block(int x, int y)
{
    // Traverse from 0 to height
    //                0 to width
    // According to what width and height props return.
    // However, rotateDir 0, 1 traverse normal
    // but 2, 3 traverse in reverse order.
    int i, j;
    if (rotateIndex == 0 || rotateIndex == 2)
    {
        i = x;
        j = y;
    }
    else
    {
        i = y;
        j = x;
    }

    if (rotateIndex == 1 || rotateIndex == 2)
    {
        i = width - 1 - i;//(Width() - 1) - i;
    }
    if (rotateIndex == 2 || rotateIndex == 3)
    {
        j = height -1 - j;//(Height() - 1) - j;
    }
    return (blocks[(width * j) + i] == 'X');
}

}	}