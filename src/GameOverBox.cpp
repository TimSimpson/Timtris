#ifndef GAMEOVERBOX_C
#define GAMEOVERBOX_C

#include "GameOverBox.h"

GameOverBox::GameOverBox(GfxScreen * screen, GfxMapImageID textTiles)
{
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
    layer->SetBoxTiles(96 + 9, 96 + 5, 96 + 4,
                       96 + 0, 96 + 0,
                       96 + 2, 96 + 5, 96 + 7);
    // Make invisible.
    layer->Clear(0, 0, 16, 16, 96 + 10);
    
    
    countDown = 0;
    state = 0;
}

GameOverBox::~GameOverBox()
{
    layer->DeleteImage();
    delete layer;
}

GfxImageID GameOverBox::GetImageID()
{
    return layer->GetImageId();
}

int GameOverBox::GetState()
{
    return state;
}

void GameOverBox::MakeInvisible()
{
    layer->Clear(0, 0, 16, 16, 96 + 10);
}

void GameOverBox::StartCountDown(int initial)
{
    countDown = initial;
    animationTimer = SysClockGetTime() + 1000;
    state = 2;
    layer->Clear(1, 1, 15, 15, 0);
    layer->SetBox(0, 0, 16, 16);
    layer->Write(1, 2, "Play  ", 6);
    layer->Write(1, 3, "again?", 6);
    layer->WriteNumber(3, 5, countDown, 2);
}

void GameOverBox::StartGameOver()
{
    state = 1;
    layer->Clear(1, 1, 15, 15, 0);
    layer->SetBox(0, 0, 16, 16);
    layer->Write(2, 2, "GAME", 4);
    layer->Write(2, 3, "OVER", 4);
    animationTimer = SysClockGetTime() + 3000;
}

void GameOverBox::StartPressStart()
{
    state = 6;
    animationTimer = SysClockGetTime() + 1000;
}

void GameOverBox::StartWillContinue()
{
    state = 3;
    layer->Clear(1, 1, 15, 15, 0);
    layer->SetBox(0, 0, 16, 16);
    layer->Write(1, 2, "Do not", 6);
    layer->Write(1, 3, "let me", 6);
    layer->Write(1, 4, "down. ", 6);
    layer->Write(1, 5, "      ", 6);
    layer->Write(1, 5, " ~Peur", 6);
    animationTimer = SysClockGetTime() + 3000;
}

// Return true if the state is finished, false otherwise.
bool GameOverBox::Update()
{
     switch(state)
     {
         case 1:
         {
              return (animationTimer < SysClockGetTime());
         }
         case 2:
         {
               if (countDown <= 0)
                     return true;
                 if (SysClockGetTime() > animationTimer)
                 {
                     animationTimer = SysClockGetTime() + 1000;
                     countDown --;
                     layer->Write(3, 5, "  ", 2);
                     layer->WriteNumber(3, 5, countDown, 2);
                 }
                 return false;
              break;
         }
         case 3:
         {
              if (animationTimer < SysClockGetTime())
              {
                  layer->Clear(0, 0, 16, 16, 96 + 10);
                  state = 5;
                  return true;
              }
              return false;
              break;
         }
         case 6:
         {
             if (animationTimer < SysClockGetTime())
             {
                layer->Clear(1, 1, 15, 15, 96 + 10);
                layer->SetBox(0, 2, 8, 3);
                layer->Write(1, 3, " start", 6);
                state = 7;
                animationTimer = SysClockGetTime() + 1000;
             }
             return false;
         }
         case 7:
         {
             if (animationTimer < SysClockGetTime())
             {
                layer->Clear(1, 1, 15, 15, 96 + 10);
                layer->SetBox(0, 2, 8, 3);
                layer->Write(1, 3, "Press ", 6);
                state = 6;
                animationTimer = SysClockGetTime() + 1000;
             }
             return false;
         }
         default:
         {
                 return true;
         }
     }
}

#endif
