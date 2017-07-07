#include "TimtrisScore.h"
#include "../Engine/ErrOut.h"

TimtrisScore::TimtrisScore(GfxScreen * screen, GfxMapImageID textTiles)
{
    score = 0;
    lines = 0;
    GfxPixel colorKey[1];
    colorKey[0].blue = 128;
    colorKey[0].green = 0;
    colorKey[0].red = 255;
    colorKey[0].alpha = 0;
    GfxPixel stdAlpha;
    stdAlpha.alpha = 255;

    //16 x 5
    // 14 x 3
    int width = 8;// of score box
    int height = 8;
    layer = new GfxLayer(screen, textTiles,
                         16, 16, 16 * width, 16 * height);
    //96
    layer->SetTile(0, 0, 96 + 9);
    layer->SetTile(width - 1, 0, 96 + 4);
    layer->SetTile(0, height - 1, 96 + 2);
    layer->SetTile(width - 1, height - 1, 96 + 7);
    for (int i = 1; i < width - 1; i ++)
    {
        layer->SetTile(i, 0, 96 + 5);
        layer->SetTile(i, height - 1, 96 + 5);
    }
    for (int i = 1; i < height - 1; i ++)
    {
        layer->SetTile(0, i, 96 + 0);
        layer->SetTile(width - 1, i, 96 + 0);
    }
    for (int i = 1; i < width - 1; i ++)
    {
        for (int j = 1; j < height - 1; j ++)
        {
            layer->SetTile(i, j, 0);
        }
    }
        // ErrOutWriteLine(0, "we we");
                     // 01234567
    layer->Write(1, 1, "Score ", 6);
    layer->Write(1, 2, "000000", 6);
    layer->Write(1, 3, "Level ", 6);
    layer->Write(1, 4, "  Easy", 6);
    layer->Write(1, 5, "Lines ", 6);
    layer->Write(1, 6, "     0", 6);
}

TimtrisScore::~TimtrisScore()
{
    layer->DeleteImage();
    delete layer;
}

void TimtrisScore::AddLinePoints()
{
    AddPoints(100);
    lines ++;
    layer->WriteNumber(1,6, lines, 6);
}

void TimtrisScore::AddPoints(int points)
{
    //char buf[14];
    int rem;
    int cursor; // Goes from right to left
    int tempScore;
    score += points;
    tempScore = score;
    for(cursor = 5; tempScore > 0 && cursor >= 0; cursor --)
    {
        rem = tempScore % 10;
        layer->SetTile(1 + cursor, 2, rem + 16);//rem + 16 + 32);
        tempScore /= 10;
    }
    for ( ; cursor >= 0; cursor --)
    {
        layer->SetTile(1 + cursor, 2, 0);//buf[cursor] = 32;
    }
//    layer->Write(1, 3, buf, 13);
}

GfxImageID TimtrisScore::GetImageID()
{
    return layer->GetImageId();
}

int TimtrisScore::GetScore()
{
    return score;
}
