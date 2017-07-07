#ifndef FILE_Timtris_C
#define FILE_Timtris_C

#include "../LE.h"
#include "../Engine/LpInput.h"
#include "../Engine/Gfx.h"
#include "../Engine/GfxLayer.h"
#include "../Engine/ErrOut.h"
#include "../Engine/Sfx.h"
#include "../Engine/SysClock.h"

#include "TimtrisTetrad.h"
#include "TimtrisGrid.h"
#include "TimtrisPlayer.h"

// Methods
static void gameStartUp();
static void titleStartUp();
static void optionsStartUp();
static void gameOverStartUp();
static void TimtrisGameUpdate();
static void TimtrisTitleUpdate();
static void TimtrisOptionsUpdate();
static void TimtrisGameOverUpdate();

// Global entities
static GfxScreen * screen;
static GfxSprite * backgroundSprite;
static LpInputControl * control1;

// Game entities
static TimtrisGrid * grid;
static TimtrisPlayer * players[1];
static TimtrisScore * playerScore[1];
static GameOverBox * gameOverBox[1];
static GfxSprite * scoreSprite[1];
static GfxSprite * gameOverSprite[1];

// Gfx
static GfxImageID titleScreenImage;
static GfxImageID optionScreenImage;
static GfxImageID backgroundImage;
static GfxImageID title;
static GfxImageID gameOver;
static GfxImageID gameBg;
static GfxImageID scoreText;
static GfxMapImageID gridTiles;

// Options
static GfxLayer * optionsLayer;
static int optionsCursor;
static int optionsGridSize;
static ClockTimeInt lastCursorMove;
static int playerCount;


// Sound
static SfxChannel * bgMusic;
static SfxChannel * sfxChannel;
static SfxSample * soundTimtris;
static SfxStream * streamMusic;

static int TimtrisState;

#define TimtrisSTATE_GAME 3
#define TimtrisSTATE_TITLE 1
#define TimtrisSTATE_OPTIONS 2
#define TimtrisSTATE_GAMEOVER 4

void TimtrisStartUp()
{
     GfxPixel colorKey[2];
     colorKey[0].blue = 128;
     colorKey[0].green = 0;
     colorKey[0].red = 255;
     colorKey[0].alpha = 0;
     colorKey[1].blue = 0;//61;
     colorKey[1].green = 0;//39;
     colorKey[1].red = 0;//37;
     colorKey[1].alpha = 50;//100;

     screen = GfxGetScreen(0);
     if ((title = screen->GetImageID("TimtrisTitle.bmp", colorKey, 1)) == -1){//pixel, 128, 128)) == -1){
         ErrOutWriteLineP(0, "Error getting testbm.bmp");
         return;// false;
     }
     if ((gameOver = screen->GetImageID("TimtrisGameOver.bmp", colorKey, 1)) == -1){//pixel, 128, 128)) == -1){
         ErrOutWriteLineP(0, "Error getting testbm.bmp");
         return;// false;
     }
     if ((gameBg = screen->GetImageID("Timtris.bmp", colorKey, 1)) == -1){//pixel, 128, 128)) == -1){
         ErrOutWriteLineP(0, "Error getting testbm.bmp");
         return; //false;
     }
     if ((gridTiles = screen->GetMapImageID("TimtrisTiles.bmp", colorKey, 2, 32, 32)) == -1){//pixel, 128, 128)) == -1){
         ErrOutWriteLineP(0, "Error getting testbm.bmp");
         return; //false;
     }
     if ((backgroundImage = screen->GetImageID("tetBg.bmp", colorKey, 0)) == -1){
         ErrOutWriteLineP(0, "Error getting SlugBearAndTugWatchVegtableStealer.bmp");
         return; //false;
     }
     if ((titleScreenImage = screen->GetImageID("TimtrisTitle.bmp", colorKey, 0)) == -1){
         ErrOutWriteLineP(0, "Error getting TimtrisTitle.bmp");
         return; //false;
     }
     if ((optionScreenImage = screen->GetImageID("OptionScreen.bmp", colorKey, 0)) == -1){
         ErrOutWriteLineP(0, "Error getting SlugBearAndTugWatchVegtableStealer.bmp");
         return; //false;
     }

     if ((scoreText = screen->GetMapImageID("scoreText.bmp", colorKey, 1, 16, 16)) == -1)
     {
         ErrOutWriteLineP(0, "Error getting scoreText.bmp");
         return;
     }

     control1 = LpInputGetControl(0);

      // Set background image.
     backgroundSprite = new GfxSprite();
     backgroundSprite->X = 0;
     backgroundSprite->Y = 0;
     backgroundSprite->Width = 640;
     backgroundSprite->Height = 480;
     backgroundSprite->ImageID = backgroundImage;
     backgroundSprite->Frame.x1 = 0;
     backgroundSprite->Frame.y1 = 0;
     backgroundSprite->Frame.x2 = 640;//1022;//639;
     backgroundSprite->Frame.y2 = 480;//767;//439;
     screen->SetFirstGfxObject(backgroundSprite);

     // Set options layer
     optionsLayer = new GfxLayer(screen, scoreText, 16, 16,
                                 640, 480);
     optionsLayer->Clear(0, 0, 40, 30, 0);
     optionsLayer->SetBox(0, 0, 40, 30, 96 + 9, 96 + 5, 96 + 4,
                                        96 + 0, 96 + 0,
                                        96 + 2, 96 + 5, 96 + 7);

     optionsLayer->SetBox(15, 2, 9, 3, 96 + 9, 96 + 5, 96 + 4,
                                        96 + 0, 96 + 0,
                                        96 + 2, 96 + 5, 96 + 7);
     optionsLayer->Write(16, 3, "OPTIONS", 7);
     optionsLayer->Write(3, 5, "Players");
     optionsLayer->Write(3, 7, " 1   2   3   4");
     optionsLayer->Write(3, 9, "Difficulty", 10);
                              //123456789012345678901
     optionsLayer->Write(3, 11, " Easy   Normal   Hard", 21);
     optionsLayer->Write(3, 13, "Play Field", 10);
     optionsLayer->Write(3, 15, " Standard Fat", 13);

     playerCount = 1;

     // Sounds! YEA!

     sfxChannel = new SfxChannel();
     bgMusic = new SfxChannel();
     soundTimtris = new SfxSample();
     soundTimtris->Load("Timtris\\timtris.wav");
     streamMusic = new SfxStream();
     streamMusic->Open("Timtris\\takecake.wav");//timtrisTheme.wav");
     titleStartUp();
    // gameStartUp();
}

static void titleStartUp()
{
     backgroundSprite->ImageID = titleScreenImage;
     backgroundSprite->Next = nullptr;
     sfxChannel->SetToSource(soundTimtris);
     sfxChannel->Play(true);
     TimtrisState = TimtrisSTATE_TITLE;
}

static void optionStartUp()
{
     backgroundSprite->ImageID = optionsLayer->GetImageId();
     TimtrisState = TimtrisSTATE_OPTIONS;
     optionsGridSize = 10;
     optionsCursor = 0;
     lastCursorMove = SysClockGetTime() + 500;
}

static void gameOverStartUp()
{
     //backgroundSprite->ImageID = gameOverScreenImage;
     TimtrisState = TimtrisSTATE_GAMEOVER;
}

static void gameStartUp()
{
     sfxChannel->Stop();
     sfxChannel->SetToSource(streamMusic);
     sfxChannel->Play();


     ErrOutWriteLine("Game start");
     ErrOutWrite("Players ");
     ErrOutWriteNumber(playerCount);

     /*GfxPixel stdAlpha;
     stdAlpha.alpha = 255;
      GfxPixel colorKey[1];
     colorKey[0].blue = 128;
     colorKey[0].green = 0;
     colorKey[0].red = 255;
     colorKey[0].alpha = 0;

     GfxLayer * layer = new GfxLayer(screen, "yagdText.bmp", stdAlpha,
                                     colorKey, 1,
                                     8, 8,
                                     640, 480);
     backgroundSprite->ImageID = layer->GetImageId();
     layer->SetTile(4, 4, 5);
     layer->Write(4, 4, "Test text.", 20);
     */
      backgroundSprite->ImageID = backgroundImage;

     // Initialize grid.
     grid = new TimtrisGrid(optionsGridSize, 17, 32, 32, gridTiles, 7);

     grid->Clear();
     // Initialize player score.
     for (int i = 0; i < playerCount; i ++)
     {
         playerScore[i] = new TimtrisScore(screen, scoreText);
         gameOverBox[i] = new GameOverBox(screen, scoreText);
         // Initialize player.
         players[i] = new TimtrisPlayer(i, playerScore[0], gameOverBox[0]);
         players[i]->SetGrid(grid);
         players[i]->SetControl(LpInputGetControl(i));
     }

     ErrOutWriteLine("Timtris.c gameStartUp 1");
     /*gridWidth = 10;
     gridHeight = 17;
     squareWidth = 24;
     squareHeight = 24;*/

     // Initialize grid sprite.
     GfxSprite * gridSprite = grid->GetSprite();
     gridSprite->Width = 24 * grid->GridWidth();
     gridSprite->Height = 24 * grid->GridHeight();
     gridSprite->X = (640 - gridSprite->Width) / 2;//640 - 320 - ((gridWidth / 2) * squareWidth);
     gridSprite->Y = (480 - gridSprite->Height) / 2;//480 - 240 - ((gridHeight / 2) * squareHeight);
     backgroundSprite->Append(gridSprite);

     // Initialize player score sprite.
     scoreSprite[0] = new GfxSprite();
     scoreSprite[0]->Width = 16 * 8;//640;
     scoreSprite[0]->Height = 16 * 8;
     scoreSprite[0]->X = 640 - scoreSprite[0]->Width - 16;
     scoreSprite[0]->Y = 50;
     scoreSprite[0]->ImageID = playerScore[0]->GetImageID();
     scoreSprite[0]->Frame.x1 = 0;
     scoreSprite[0]->Frame.y1 = 0;
     scoreSprite[0]->Frame.x2 = 16 * 8 - 1;
     scoreSprite[0]->Frame.y2 = 16 * 8 - 1;
     gridSprite->Append(scoreSprite[0]);

     gameOverSprite[0] = new GfxSprite();
     gameOverSprite[0]->Width = 16 * 8;//640;
     gameOverSprite[0]->Height = 16 * 8;
     gameOverSprite[0]->X = gridSprite->X + (gridSprite->Width / 2) - (16 * 4);
     gameOverSprite[0]->Y = 50;
     gameOverSprite[0]->ImageID = gameOverBox[0]->GetImageID();
     gameOverSprite[0]->Frame.x1 = 0;
     gameOverSprite[0]->Frame.y1 = 0;
     gameOverSprite[0]->Frame.x2 = 16 * 8 - 1;
     gameOverSprite[0]->Frame.y2 = 16 * 8 - 1;
     scoreSprite[0]->Append(gameOverSprite[0]);


//     screen->SetFirstSprite(gridSprite);



    TimtrisState = TimtrisSTATE_GAME;
     /*grid->SetGrid(4,12,5);
     grid->SetGrid(5,12,4);
     grid->SetGrid(6,12,3);
     grid->SetGrid(7,12,2);
     grid->SetGrid(8,12,1);
     grid->SetGrid(9,12,0);*/

     ErrOutWriteLine("Timtris.c gameStartUp 2");

}

void TimtrisUpdate()
{
     switch(TimtrisState)
     {
         case TimtrisSTATE_OPTIONS:
         {
             TimtrisOptionsUpdate();
             break;
         }
         case TimtrisSTATE_TITLE:
         {
             TimtrisTitleUpdate();
             break;
         }
         case TimtrisSTATE_GAME:
         {
             TimtrisGameUpdate();
             break;
         }
         case TimtrisSTATE_GAMEOVER:
         {
             TimtrisGameOverUpdate();
             break;
         }
     }
}

static void TimtrisTitleUpdate()
{
    if (control1->Button(0))
    {
        optionStartUp();
    }
}

static void TimtrisOptionsUpdate()
{
    if (lastCursorMove < SysClockGetTime() && control1->KeyDown())
    {
        lastCursorMove = SysClockGetTime() + 500;
        if (control1->Down())
        {
            optionsCursor ++;
            if (optionsCursor > 3)
               optionsCursor = 2;
            return;
        }
        if (control1->Up())
        {
            optionsCursor --;
            if (optionsCursor < 0)
               optionsCursor = 0;
            return;
        }
        if (control1->Button(0))
        {
            gameStartUp();
        }
        switch(optionsCursor)
        {
            case 0:
            {
                // Players
                optionsLayer->SetBox(3, 6, 3, 3,
                                     0, 0, 0, 0, 0, 0, 0, 0);
                optionsLayer->SetBox(7, 6, 3, 3,
                                     0, 0, 0, 0, 0, 0, 0, 0);
                optionsLayer->SetBox(11, 6, 3, 3,
                                     0, 0, 0, 0, 0, 0, 0, 0);
                optionsLayer->SetBox(15, 6, 3, 3,
                                     0, 0, 0, 0, 0, 0, 0, 0);
                if (control1->Left())
                {
                    if (playerCount > 1)
                        playerCount --;
                }
                if (control1->Right())
                {
                    if (playerCount < 4)
                        playerCount ++;
                }
                if (playerCount == 1)
                {
                    optionsLayer->SetBox(3, 6, 3, 3,
                                     96 + 9, 96 + 5, 96 + 4,
                                          96 + 0, 96 + 0,
                                          96 + 2, 96 + 5, 96 + 7);
                }
                if (playerCount == 2)
                {
                    optionsLayer->SetBox(7, 6, 3, 3,
                                     96 + 9, 96 + 5, 96 + 4,
                                          96 + 0, 96 + 0,
                                          96 + 2, 96 + 5, 96 + 7);
                }
                if (playerCount == 3)
                {
                    optionsLayer->SetBox(11, 6, 3, 3,
                                     96 + 9, 96 + 5, 96 + 4,
                                          96 + 0, 96 + 0,
                                          96 + 2, 96 + 5, 96 + 7);
                }
                if (playerCount == 4)
                {
                    optionsLayer->SetBox(15, 6, 3, 3,
                                     96 + 9, 96 + 5, 96 + 4,
                                          96 + 0, 96 + 0,
                                          96 + 2, 96 + 5, 96 + 7);
                }
                break;
            }
            case 1:
            {
                 // DiFFICULTY
                 break;
            }
            case 2:
            {   // GRID SIZE
                if (control1->Left())
                {
                    if (optionsGridSize == 20)
                        optionsGridSize = 10;
                }
                if (control1->Right())
                {
                    if (optionsGridSize == 10)
                        optionsGridSize = 20;
                }
                optionsLayer->SetBox(2, 12, 11, 3,
                                     0, 0, 0, 0, 0, 0, 0, 0);
                optionsLayer->SetBox(12, 12, 5, 3,
                                     0, 0, 0, 0, 0, 0, 0, 0);
                 if (optionsGridSize == 10)
                 {
                     optionsLayer->SetBox(2, 12, 11, 3,
                                          96 + 9, 96 + 5, 96 + 4,
                                          96 + 0, 96 + 0,
                                          96 + 2, 96 + 5, 96 + 7);
                 }
                 if (optionsGridSize == 20)
                 {
                     optionsLayer->SetBox(12, 12, 5, 3,
                                          96 + 9, 96 + 5, 96 + 4,
                                          96 + 0, 96 + 0,
                                          96 + 2, 96 + 5, 96 + 7);
                 }
            }
            case 3:
            {
            }
        }
    }
}

static void TimtrisGameOverUpdate()
{
}

static void TimtrisGameUpdate()
{
     ErrOutWriteLine("Timtris.c TimtrisGameUpdate() 1");
     //bgMusic->SetToSource(soundTimtris);
     //bgMusic->Play();
     if (!grid->Update())
         return;
     for (int i = 0; i < playerCount; i ++)
     {
         players[i]->Update();
     }
     ErrOutWriteLine("Timtris.c TimtrisGameUpdate() 2");
}

/*
     gridX = 640-320-((gridWidth / 2) * squareWidth);
     gridY = 480 - 240 - ((gridHeight / 2) * squareHeight);
     numColors = 9;
     numPlayers = 1;

     initTetrad(&(tetrads[0]), (charptr) tetradI, 1, 4);
     initTetrad(&(tetrads[1]), (charptr) tetradT, 3, 2);
     initTetrad(&(tetrads[2]), (charptr) tetradO, 2, 2);
     initTetrad(&(tetrads[3]), (charptr) tetradL, 2, 3);
     initTetrad(&(tetrads[4]), (charptr) tetradJ, 2, 3);
     initTetrad(&(tetrads[5]), (charptr) tetradS, 3, 2);
     initTetrad(&(tetrads[6]), (charptr) tetradZ, 3, 2);

     int c = 1;

     GfxSprite * lastOne = nullptr;
     for(int y = 0; y < gridHeight; y ++)
     {
         for (int x = 0; x < gridWidth; x ++)
         {
             gridSprites[y][x] = new GfxSprite();
             if (!tateMode)
             {
                 gridSprites[y][x]->X = gridX + (squareWidth * x);
                 gridSprites[y][x]->Y = gridY + (squareHeight * y);
                 gridSprites[y][x]->Width = squareWidth;
                 gridSprites[y][x]->Height = squareHeight;
             }
             else
             {   //48, 80  , so 640 - 80 =560
                 gridSprites[y][x]->X = ((640 - (gridHeight * 32)) /2)
                                        + (32 * (gridHeight - y)) - 32;
                 gridSprites[y][x]->Y = ((480 - (gridWidth * 32)) / 2)
                                      + (32 * (x)) - 32;
                 gridSprites[y][x]->Width = 32;//squareWidth;
                 gridSprites[y][x]->Height = 32;//squareHeight;
             }
             gridSprites[y][x]->Frame.x1 = 0;
             gridSprites[y][x]->Frame.y1 = 0;
             gridSprites[y][x]->Frame.x2 = 7;
             gridSprites[y][x]->Frame.y2 = 7;
             gridSprites[y][x]->ImageID = colors[c ++];
             if (c >= numColors)
                c = 1;
             if (lastOne != nullptr)
             {
                 lastOne->Next = gridSprites[y][x];
                 gridSprites[y][x]->Prev = lastOne;
                 gridSprites[y][x]->Next = nullptr;
             }
             lastOne = gridSprites[y][x];
         }
     }
     for (int y = 0; y < gridHeight; y ++)
     {
         for (int x = 0; x < gridWidth; x ++)
         {
             grid[y][x] = 0;
         }
     }
     bg->Next = gridSprites[0][0];
//     GfxSetFirstSprite(gridSprites[0][0]);

     for (int i = 0; i < numPlayers; i ++)
     {
         initPlayerInfo(&(players[i]), i);
     }

}

/*
// About rotation...
//    i and j always are horizontally and vertically scanning the grid.
//    But different ones may read the tetrads to "rotate" them.
// dir          j - direction j reads tetrad i - direction i reads tetrad
// normal = 0   j - down i - right
// 90 = 1       j - right i - down
// 180 = 2     j - up  i - left
// 270 = 3    j - left i - up
bool collideAtSpot(int x, int y, int tetradIndex, int rotateDir)
{

     bool reverseJI;
     int jStart, jInc, jEnd, iStart,iInc, iEnd, jy, ix;
          if (rotateDir == 0)
     {
         reverseJI = false;
         jStart = 0;
         jInc = 1;
         jEnd = tetrads[tetradIndex].height;
         iStart = 0;
         iInc = 1;
         iEnd = tetrads[tetradIndex].width;
     }
     else if (rotateDir == 1)
     {
         reverseJI = true;
         jStart = 0;
         jInc = 1;
         jEnd = tetrads[tetradIndex].width;
         iStart = tetrads[tetradIndex].height - 1;//0;
         iInc = -1;//1;
         iEnd = -1;//tetrads[tetradIndex].height;
     }
     else if (rotateDir == 2)
     {
         reverseJI = false;
         jStart = tetrads[tetradIndex].height - 1;
         jInc = -1;
         jEnd = -1;
         iStart = tetrads[tetradIndex].width - 1;
         iInc = -1;
         iEnd = -1;
     }
     else if (rotateDir == 3)
     {
         reverseJI = true;
         jStart = tetrads[tetradIndex].width - 1;
         jInc = -1;
         jEnd = -1;
         iStart = 0;//tetrads[tetradIndex].height - 1;
         iInc = 1;//-1;
         iEnd = tetrads[tetradIndex].height;//-1;
     }
     else
     {
         ErrOutWriteLine(0, "Bad rotate dir!");
     }
     for (int j = jStart; j != jEnd; j += jInc)
     {
         for (int i = iStart; i != iEnd; i += iInc)
         {   // The block is over another block.
             if (
                 (!reverseJI &&(tetrads[tetradIndex].blocks[j][i] == 'X'))
                 ||
                 (reverseJI &&(tetrads[tetradIndex].blocks[i][j] == 'X'))
                )
             {
                 jy = y + ((jEnd - j) * jInc);
                 ix = x + ((iEnd - i) *iInc);
                 if (jy >= 0)
                 {
                     if ((jy < 0) || (ix < 0) ||
                         (jy >= gridHeight) || (ix >= gridWidth)
                         || (grid[jy][ix] != 0))
                     {
                         return true;
                     }
                 }
             }
         }
     }
     return false;
}

void removeTetrad(int x, int y, int tetradIndex, int rotateDir)
{
     bool reverseJI;
     int jStart, jInc, jEnd, iStart,iInc, iEnd, jy, ix;
     if (rotateDir == 0)
     {
         reverseJI = false;
         jStart = 0;
         jInc = 1;
         jEnd = tetrads[tetradIndex].height;
         iStart = 0;
         iInc = 1;
         iEnd = tetrads[tetradIndex].width;
     }
     else if (rotateDir == 1)
     {
         reverseJI = true;
         jStart = 0;
         jInc = 1;
         jEnd = tetrads[tetradIndex].width;
         iStart = tetrads[tetradIndex].height - 1;//0;
         iInc = -1;//1;
         iEnd = -1;//tetrads[tetradIndex].height;
     }
     else if (rotateDir == 2)
     {
         reverseJI = false;
         jStart = tetrads[tetradIndex].height - 1;
         jInc = -1;
         jEnd = -1;
         iStart = tetrads[tetradIndex].width - 1;
         iInc = -1;
         iEnd = -1;
     }
     else if (rotateDir == 3)
     {
         reverseJI = true;
         jStart = tetrads[tetradIndex].width - 1;
         jInc = -1;
         jEnd = -1;
         iStart = 0;//tetrads[tetradIndex].height - 1;
         iInc = 1;//-1;
         iEnd = tetrads[tetradIndex].height;//-1;
     }
     else
     {
         ErrOutWriteLine(0, "Bad rotate dir!");
     }
     for (int j = jStart; j != jEnd; j += jInc)
     {
         for (int i = iStart; i != iEnd; i += iInc)
         {   // The block is over another block.
             if (
                 (!reverseJI &&(tetrads[tetradIndex].blocks[j][i] == 'X'))
                 ||
                 (reverseJI &&(tetrads[tetradIndex].blocks[i][j] == 'X'))
                )
             {
                 jy = y + ((jEnd - j) * jInc);
                 ix = x + ((iEnd - i) *iInc);
                 if ((jy >= 0) && (ix >= 0) &&
                     (jy < gridHeight) && (ix < gridWidth))
                 {
                     grid[jy][ix] = 0;
                 }
             }
         }
     }
}

void placeTetrad(int x, int y, int tetradIndex, int rotateDir, int color)
{
 bool reverseJI;
     int jStart, jInc, jEnd, iStart,iInc, iEnd, jy, ix;
     if (rotateDir == 0)
     {
         reverseJI = false;
         jStart = 0;
         jInc = 1;
         jEnd = tetrads[tetradIndex].height;
         iStart = 0;
         iInc = 1;
         iEnd = tetrads[tetradIndex].width;
     }
     else if (rotateDir == 1)
     {
         reverseJI = true;
         jStart = 0;
         jInc = 1;
         jEnd = tetrads[tetradIndex].width;
         iStart = tetrads[tetradIndex].height - 1;//0;
         iInc = -1;//1;
         iEnd = -1;//tetrads[tetradIndex].height;
     }
     else if (rotateDir == 2)
     {
         reverseJI = false;
         jStart = tetrads[tetradIndex].height - 1;
         jInc = -1;
         jEnd = -1;
         iStart = tetrads[tetradIndex].width - 1;
         iInc = -1;
         iEnd = -1;
     }
     else if (rotateDir == 3)
     {
         reverseJI = true;
         jStart = tetrads[tetradIndex].width - 1;
         jInc = -1;
         jEnd = -1;
         iStart = 0;//tetrads[tetradIndex].height - 1;
         iInc = 1;//-1;
         iEnd = tetrads[tetradIndex].height;//-1;
     }
     else
     {
         ErrOutWriteLine(0, "Bad rotate dir!");
     }
     for (int j = jStart; j != jEnd; j += jInc)
     {
         for (int i = iStart; i != iEnd; i += iInc)
         {   // The block is over another block.
             if (
                 (!reverseJI &&(tetrads[tetradIndex].blocks[j][i] == 'X'))
                 ||
                 (reverseJI &&(tetrads[tetradIndex].blocks[i][j] == 'X'))
                )
             {
                 jy = y + ((jEnd - j) * jInc);
                 ix = x + ((iEnd - i) *iInc);
                 if ((jy >= 0) && (ix >= 0) &&
                     (jy < gridHeight) && (ix < gridWidth))
                 {
                     grid[jy][ix] = color;
                 }
             }
         }
     }
}

void rotatePlayer(playerInfo * player)
{
    int oldRotate = player->rotate;
    int oldX = player->fallX;
    int i, maxMoves;
    int gx, gy;
    player->rotate ++;
    if (player->rotate > 3)
       player->rotate = 0;
    if (collideAtSpot(player->fallX, player->fallY,
                      player->tetrad, player->rotate))
    {
        // Ok, I half assed this.  As a result, if a stack was jutting
        // out, and the line laying down was over it perfectly, it would
        // be possible to "swing" around and THROUGH the stack to get back
        // to a good spot.  The way to solve this will involve creating
        // additional, unseen fake tet-rads (such as an I that is three blocks)
        // temporarily changing the tetrad type and then testing to see
        // what would happening.  Otherwise the rotation collasion detecting
        // isn't great when rotating.
        if (player->tetrad != 2)// the O doesn't rotate
        {
            if (player->tetrad == 0)
            {
                maxMoves = 4;
            }
            else if (player->tetrad == 1)
            {
                maxMoves = 2;
            }
            else if (player->tetrad == 3)
            {
                maxMoves = 3;
            }
            else if (player->tetrad == 4)
            {
                maxMoves = 3;
            }
            else if (player->tetrad == 5)
            {
                maxMoves = 2;
            }
                        else if (player->tetrad == 6)
            {
                maxMoves = 2;
            }
            i = 0;
            while(i < maxMoves &&
                  collideAtSpot(player->fallX, player->fallY,
                                player->tetrad, player->rotate))
            {
               i ++;
               player->fallX --;
            }
            if (i == maxMoves)
            {
                player->fallX = oldX;
                player->rotate = oldRotate;
            }
        }
        else
        {
                player->rotate = oldRotate;
        }
    }

}

void dropNextPlayerPiece(playerInfo * player)
{
     player->tetrad ++;
     player->rotate = 0;
     player->color ++;
            if (player->color >= numColors)
             {
                player->color = 1;
             }
             if (player->tetrad >= numTetrads)
             {
                player->tetrad = 0;
             }
             player->fallX = gridWidth / 2;
             player->fallX -=(tetrads[player->tetrad].width / 2);
             player->fallY = (-1 * tetrads[player->tetrad].height);
             if (collideAtSpot(player->fallX, player->fallY,
                               player->tetrad, player->rotate))
             {
                player->timeToDie = SysClockGetTime() + 1500;
             }
}


static ClockTimeInt matchFlashTime;
static int matchRow = -1;
static int matchColor;
static ClockTimeInt flashTime = 1000;

bool checkMatch()
{
    bool match;
    for (int j = 0; j < gridHeight; j ++)
    {
        match = true;
        int i;
        for (i = 0; i < gridWidth; i ++)
        {
            if (grid[j][i] == 0)
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            matchRow = j;
            matchFlashTime = SysClockGetTime() + flashTime;
            matchColor = 0;
            for (int f = 0; f < gridWidth; f ++)
            {
                grid[matchRow][f] = 0;
            }
            return true;
        }
    }
    return false;
}

void refreshGrid()
{
     //Refresh grid.
     for (int y = 0; y < gridHeight; y ++)
     {
         for (int x = 0; x < gridWidth; x ++)
         {
             gridSprites[y][x]->ImageID = colors[(int)(grid[y][x])];
         }
     }
}
*/

/*
void TimtrisUpdate()
{
     /*
     if (gameOverKey)
     {
        if (LpInputGetControl(0)->Button(0))
        {
            gameOverKey = false;
            gameStart = false;
            screenWaitTime = SysClockGetTime() + 1000;
             titleScreenShow();
        }
        return;
     }
     if (!gameStart)
     {
        if ((SysClockGetTime() >= screenWaitTime) && LpInputGetControl(0)->Button(0))
        {
            GameStartUp();
            gameStart = true;
            soundGameover->Stop();
        }
        return;
     }

     // Call update for each player piece.
     // Check to see if any rows match.
     return;
/*
     if (matchRow >= 0)
     {
         if (SysClockGetTime() < matchFlashTime)
         {
             for (int f = 0; f < gridWidth; f ++)
             {
                 //gridSprites[matchRow][f]->ImageID = colors[(int)matchColor];
                 grid[matchRow][f] = matchColor;
                 refreshGrid();
                 matchColor ++;
                 if (matchColor >= numColors)
                     matchColor = 1;
             }
         }
         else
         {
             for (int j = matchRow; j > 0; j --)
             {
                 for (int i = 0; i < gridWidth; i ++)
                 {
                     grid[j][i] = grid[j - 1][i];
                 }
             }
             matchRow = -1;
             refreshGrid();
             checkMatch();
         }
         return;
     }

     bool getNextPiece;
     int oldX, oldY, oldRotate;
     for (int i = 0; i < numPlayers; i ++)
     {
         if (players[i].timeToDie >= 0
             && players[i].timeToDie > SysClockGetTime())
         {
             // GAME OVER
             gameStart = false;
             gameOverScreen();
             soundGameover->Play(false);
         }

         getNextPiece = false;
         oldX = players[i].fallX;
         oldY = players[i].fallY;
         oldRotate = players[i].rotate;

         removeTetrad(oldX, oldY,
                      players[i].tetrad, players[i].rotate);

         if (SysClockGetTime() > players[i].moveTime)
         {
             players[i].fallX += (int)players[i].control->GetX();
             if (players[i].control->Button(0))
             {
                 //players[i].rotate ++;
                 //if (players[i].rotate > 3)
                 //   players[i].rotate = 0;
                 rotatePlayer(&(players[i]));
                 soundRotate->Play(false);

             }
             if (collideAtSpot(players[i].fallX, players[i].fallY,
                              players[i].tetrad, players[i].rotate))
             {
                 //Some of the rotations may work if you move to the left a bit.
                 if (collideAtSpot(players[i].fallX, players[i].fallY,
                              players[i].tetrad, players[i].rotate))
                 // BUMP!  They must be hitting one on the side.
                 soundHit->Play(false);


                 players[i].fallX = oldX;
                 players[i].rotate = oldRotate;
                 ErrOutWriteLine("bump!");
             }
             if (players[i].control->Down())
             {
                 // Cause them to fall immedietly.
                 players[i].fallTime = SysClockGetTime() - 1;
             }
             players[i].moveTime = SysClockGetTime() + players[i].moveSpeed;
         }
         if (SysClockGetTime() > players[i].fallTime)
         {
             players[i].fallY ++;
             players[i].fallTime = SysClockGetTime() + players[i].fallSpeed;
             if (collideAtSpot(players[i].fallX, players[i].fallY,
                              players[i].tetrad, players[i].rotate))
             {
                 // Can't go any further.  This piece is done!
                 soundBottom->Play(false);

                 players[i].fallY = oldY;
                 players[i].fallX = oldX;
                 getNextPiece = true;
             }
         }

         placeTetrad(players[i].fallX, players[i].fallY,
                     players[i].tetrad, players[i].rotate, players[i].color);

         //if (checkMatch())
        // {
         //   dropNextPlayerPiece(&players[i]);
         //}
         if (getNextPiece)
         {
             if (checkMatch())
             {
                soundLine->Play(false);
             }
             dropNextPlayerPiece(&players[i]);
         }
     }

     refreshGrid();

}*/

#endif
