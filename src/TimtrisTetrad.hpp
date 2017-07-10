#ifndef TimtrisTETRAD_H
#define TimtrisTETRAD_H

#include "../LE.h"

class TimtrisTetrad
{
public:
    TimtrisTetrad(int tetradIndex, int rotationIndex);
    /* Functions to "Get" the actual data on how the block maps out. */
    bool Block(int x, int y);
    int Height();
    int Width();
    /* Properties. */
    int GetRotation();
    int GetTetrad();
    void Rotate(bool clockWise);
    void SetRotation(int rotateIndex);
    void SetTetrad(int tetradIndex);
private:
    charptr blocks;
    u32 height;
    int rotateIndex;
    int tetradIndex;
    u32 width;
};

#endif
