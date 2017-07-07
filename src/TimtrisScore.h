#ifndef TimtrisSCORE_H
#define TimtrisSCORE_H


#include "../Engine/GfxLayer.h"

class TimtrisScore
{
public:
    TimtrisScore(GfxScreen * screen, GfxMapImageID textTiles);
    void AddLinePoints();
    void AddPoints(int number);
    int GetScore();
    GfxImageID GetImageID();
    ~TimtrisScore();
private:
    GfxLayer * layer;
    int score;
    int lines;
};

#endif
