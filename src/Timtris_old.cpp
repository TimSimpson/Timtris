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
static gfx::Texture titleScreenImage;
static gfx::Texture optionScreenImage;
static gfx::Texture backgroundImage;
static gfx::Texture title;
static gfx::Texture gameOver;
static gfx::Texture gameBg;
static gfx::Texture scoreText;
static gfx::Texture gridTiles;

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
     // GfxPixel colorKey[2];
     // colorKey[0].blue = 128;
     // colorKey[0].green = 0;
     // colorKey[0].red = 255;
     // colorKey[0].alpha = 0;
     // colorKey[1].blue = 0;//61;
     // colorKey[1].green = 0;//39;
     // colorKey[1].red = 0;//37;
     // colorKey[1].alpha = 50;//100;

     // screen = GfxGetScreen(0);
     // if ((title = screen->GetImageID("TimtrisTitle.bmp", colorKey, 1)) == -1){//pixel, 128, 128)) == -1){
     //     ErrOutWriteLineP(0, "Error getting testbm.bmp");
     //     return;// false;
     // }
     // if ((gameOver = screen->GetImageID("TimtrisGameOver.bmp", colorKey, 1)) == -1){//pixel, 128, 128)) == -1){
     //     ErrOutWriteLineP(0, "Error getting testbm.bmp");
     //     return;// false;
     // }
     // if ((gameBg = screen->GetImageID("Timtris.bmp", colorKey, 1)) == -1){//pixel, 128, 128)) == -1){
     //     ErrOutWriteLineP(0, "Error getting testbm.bmp");
     //     return; //false;
     // }
     // if ((gridTiles = screen->GetMapImageID("TimtrisTiles.bmp", colorKey, 2, 32, 32)) == -1){//pixel, 128, 128)) == -1){
     //     ErrOutWriteLineP(0, "Error getting testbm.bmp");
     //     return; //false;
     // }
     // if ((backgroundImage = screen->GetImageID("tetBg.bmp", colorKey, 0)) == -1){
     //     ErrOutWriteLineP(0, "Error getting SlugBearAndTugWatchVegtableStealer.bmp");
     //     return; //false;
     // }
     // if ((titleScreenImage = screen->GetImageID("TimtrisTitle.bmp", colorKey, 0)) == -1){
     //     ErrOutWriteLineP(0, "Error getting TimtrisTitle.bmp");
     //     return; //false;
     // }
     // if ((optionScreenImage = screen->GetImageID("OptionScreen.bmp", colorKey, 0)) == -1){
     //     ErrOutWriteLineP(0, "Error getting SlugBearAndTugWatchVegtableStealer.bmp");
     //     return; //false;
     // }

     // if ((scoreText = screen->GetMapImageID("scoreText.bmp", colorKey, 1, 16, 16)) == -1)
     // {
     //     ErrOutWriteLineP(0, "Error getting scoreText.bmp");
     //     return;
     // }

     control1 = LpInputGetControl(0);

     //  // Set background image.
     // backgroundSprite = new GfxSprite();
     // backgroundSprite->X = 0;
     // backgroundSprite->Y = 0;
     // backgroundSprite->Width = 640;
     // backgroundSprite->Height = 480;
     // backgroundSprite->ImageID = backgroundImage;
     // backgroundSprite->Frame.x1 = 0;
     // backgroundSprite->Frame.y1 = 0;
     // backgroun dSprite->Frame.x2 = 640;//1022;//639;
     // backgroundSprite->Frame.y2 = 480;//767;//439;
     // screen->SetFirstGfxObject(backgroundSprite);

     // Set options layer
     // optionsLayer = new GfxLayer(screen, scoreText, 16, 16,
     //                             640, 480);
     // optionsLayer->Clear(0, 0, 40, 30, 0);
     // optionsLayer->SetBox(0, 0, 40, 30, 96 + 9, 96 + 5, 96 + 4,
     //                                    96 + 0, 96 + 0,
     //                                    96 + 2, 96 + 5, 96 + 7);

     // optionsLayer->SetBox(15, 2, 9, 3, 96 + 9, 96 + 5, 96 + 4,
     //                                    96 + 0, 96 + 0,
     //                                    96 + 2, 96 + 5, 96 + 7);
     // optionsLayer->Write(16, 3, "OPTIONS", 7);
     // optionsLayer->Write(3, 5, "Players");
     // optionsLayer->Write(3, 7, " 1   2   3   4");
     // optionsLayer->Write(3, 9, "Difficulty", 10);
     //                          //123456789012345678901
     // optionsLayer->Write(3, 11, " Easy   Normal   Hard", 21);
     // optionsLayer->Write(3, 13, "Play Field", 10);
     // optionsLayer->Write(3, 15, " Standard Fat", 13);

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
     // backgroundSprite->ImageID = optionsLayer->GetImageId();
     // TimtrisState = TimtrisSTATE_OPTIONS;
     // optionsGridSize = 10;
     // optionsCursor = 0;
     // lastCursorMove = SysClockGetTime() + 500;
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



    TimtrisState = TimtrisSTATE_GAME;

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

// static void TimtrisOptionsUpdate()
// {
//     if (lastCursorMove < SysClockGetTime() && control1->KeyDown())
//     {
//         lastCursorMove = SysClockGetTime() + 500;
//         if (control1->Down())
//         {
//             optionsCursor ++;
//             if (optionsCursor > 3)
//                optionsCursor = 2;
//             return;
//         }
//         if (control1->Up())
//         {
//             optionsCursor --;
//             if (optionsCursor < 0)
//                optionsCursor = 0;
//             return;
//         }
//         if (control1->Button(0))
//         {
//             gameStartUp();
//         }
//         switch(optionsCursor)
//         {
//             case 0:
//             {
//                 // Players
//                 optionsLayer->SetBox(3, 6, 3, 3,
//                                      0, 0, 0, 0, 0, 0, 0, 0);
//                 optionsLayer->SetBox(7, 6, 3, 3,
//                                      0, 0, 0, 0, 0, 0, 0, 0);
//                 optionsLayer->SetBox(11, 6, 3, 3,
//                                      0, 0, 0, 0, 0, 0, 0, 0);
//                 optionsLayer->SetBox(15, 6, 3, 3,
//                                      0, 0, 0, 0, 0, 0, 0, 0);
//                 if (control1->Left())
//                 {
//                     if (playerCount > 1)
//                         playerCount --;
//                 }
//                 if (control1->Right())
//                 {
//                     if (playerCount < 4)
//                         playerCount ++;
//                 }
//                 if (playerCount == 1)
//                 {
//                     optionsLayer->SetBox(3, 6, 3, 3,
//                                      96 + 9, 96 + 5, 96 + 4,
//                                           96 + 0, 96 + 0,
//                                           96 + 2, 96 + 5, 96 + 7);
//                 }
//                 if (playerCount == 2)
//                 {
//                     optionsLayer->SetBox(7, 6, 3, 3,
//                                      96 + 9, 96 + 5, 96 + 4,
//                                           96 + 0, 96 + 0,
//                                           96 + 2, 96 + 5, 96 + 7);
//                 }
//                 if (playerCount == 3)
//                 {
//                     optionsLayer->SetBox(11, 6, 3, 3,
//                                      96 + 9, 96 + 5, 96 + 4,
//                                           96 + 0, 96 + 0,
//                                           96 + 2, 96 + 5, 96 + 7);
//                 }
//                 if (playerCount == 4)
//                 {
//                     optionsLayer->SetBox(15, 6, 3, 3,
//                                      96 + 9, 96 + 5, 96 + 4,
//                                           96 + 0, 96 + 0,
//                                           96 + 2, 96 + 5, 96 + 7);
//                 }
//                 break;
//             }
//             case 1:
//             {
//                  // DiFFICULTY
//                  break;
//             }
//             case 2:
//             {   // GRID SIZE
//                 if (control1->Left())
//                 {
//                     if (optionsGridSize == 20)
//                         optionsGridSize = 10;
//                 }
//                 if (control1->Right())
//                 {
//                     if (optionsGridSize == 10)
//                         optionsGridSize = 20;
//                 }
//                 optionsLayer->SetBox(2, 12, 11, 3,
//                                      0, 0, 0, 0, 0, 0, 0, 0);
//                 optionsLayer->SetBox(12, 12, 5, 3,
//                                      0, 0, 0, 0, 0, 0, 0, 0);
//                  if (optionsGridSize == 10)
//                  {
//                      optionsLayer->SetBox(2, 12, 11, 3,
//                                           96 + 9, 96 + 5, 96 + 4,
//                                           96 + 0, 96 + 0,
//                                           96 + 2, 96 + 5, 96 + 7);
//                  }
//                  if (optionsGridSize == 20)
//                  {
//                      optionsLayer->SetBox(12, 12, 5, 3,
//                                           96 + 9, 96 + 5, 96 + 4,
//                                           96 + 0, 96 + 0,
//                                           96 + 2, 96 + 5, 96 + 7);
//                  }
//             }
//             case 3:
//             {
//             }
//         }
//     }
// }

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
