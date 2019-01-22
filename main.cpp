#include <iostream>
#include <fstream>
#include <algorithm>
#include "SBDL.h"

using namespace std;

// Constants
const int window_width = 1100;
const int window_height = 461;
const int FPS = 30;
const int delay = 1000 / FPS;

// Public Variables
struct MovingThing {
    int size;
    Texture *images;
    int CurrentIndex;
};

// Struct for Fliping
struct FlippingThing {
    int size;
    Texture *images = nullptr;
    int currentIndex;
};


// Function for adding Fliping Coin
void initializeFlippingCoin(FlippingThing &flipping) {
    flipping.images = new Texture[6];
    flipping.size = 6;
    flipping.currentIndex = 0;
    flipping.images[0] = SBDL::loadTexture("assets/pic/coin/1.png");
    flipping.images[1] = SBDL::loadTexture("assets/pic/coin/2.png");
    flipping.images[2] = SBDL::loadTexture("assets/pic/coin/3.png");
    flipping.images[3] = SBDL::loadTexture("assets/pic/coin/4.png");
    flipping.images[4] = SBDL::loadTexture("assets/pic/coin/5.png");
    flipping.images[5] = SBDL::loadTexture("assets/pic/coin/6.png");
}


// Enums for Menu Options
enum PlayerIcon {
    Barry, Alien
};
enum Background {
    Sector, Lab, Volcano
};
enum GameStatus {
    Menu, Pause, Lose, Game , Setting
};

int main() {
    srand(time(0));

    // Adding Pattern
    int randPattern = rand() % 10 + rand() % 10 + 1;
    std::string pattern = "assets/coinPatterns/coinPattern" + to_string(randPattern) + ".txt";
    std::ifstream fin(pattern);
    int linesCounter = 0;
    int eachLineLength = 0;
    std::string temp;
    while (getline(fin, temp)) {
        eachLineLength = std::max(eachLineLength, (int) (temp.size()));
        linesCounter++;
    }

    // Create the Program Screen
    SBDL::InitEngine("JoyPack JoyRider", window_width, window_height);

    // Create a 2D array for Coins Pattern
    FlippingThing **coinsAr = new FlippingThing *[linesCounter];
    for (int i = 0; i < linesCounter; i++)
        coinsAr[i] = new FlippingThing[eachLineLength];
    int **grid = new int *[linesCounter];
    for (int i = 0; i < linesCounter; i++)
        grid[i] = new int[eachLineLength];


    // Create a 2D bool array for Hiding Coins
    bool **showCoin = new bool *[linesCounter];
    for (int i = 0; i < linesCounter; i++)
        showCoin[i] = new bool[eachLineLength];
    for (int i = 0; i < linesCounter; i++) {
        for (int j = 0; j < eachLineLength; j++) {
            showCoin[i][j] = true;
        }
    }

    // For File Pattern
    fin.clear();
    fin.seekg(0, std::ios::beg);
    int lineCounterTemp = 0;
    while (getline(fin, temp)) {
        if (!temp.empty()) {
            for (int i = 0; i < temp.size(); i++) {
                grid[lineCounterTemp][i] = (temp[i] - '0');
                if (grid[lineCounterTemp][i] == 1) {
                    initializeFlippingCoin(coinsAr[lineCounterTemp][i]);
                }
            }
        }
        lineCounterTemp++;
    }


    // Music
    Music *BgMusic = SBDL::loadMusic("assets/sound/music.mp3");

    // Sounds
    Sound *CoinSound = SBDL::loadSound("assets/sound/coin.wav");
    Sound *ZapperSound = SBDL::loadSound("assets/sound/zapper.wav");
    Sound *LazerSound = SBDL::loadSound("assets/sound/laser_fire_lp.wav");
    Sound *LazerWarning = SBDL::loadSound("assets/sound/laser_warning.wav");
    Sound *MissleWaringSound = SBDL::loadSound("assets/sound/missile_warning.wav") ;
    Sound *MissleFireSound = SBDL::loadSound("assets/sound/missile_launch.wav") ;
    Sound *SoundOnEffect = SBDL::loadSound("assets/sound/soundonEffect.wav") ;
    Sound *ChangeBtnEffect = SBDL::loadSound("assets/sound/changebarry.wav") ;


    // Fonts
    Font *CoinScoreFont = SBDL::loadFont("assets/font/digitalnumberbttf.otf", 20);
    Font *DistanceFont = SBDL::loadFont("assets/font/SourceHanSansSC-Normal.otf", 20);

    // Textures
    Texture Coin = SBDL::loadTexture("assets/pic/coin/1.png");
    Texture Pallete = SBDL::loadTexture("assets/pic/menu/free.png") ;

    Texture PauseBg = SBDL::loadTexture("assets/pic/menu/pauseb.png") ; 
    Texture MenuBg = SBDL::loadTexture("assets/pic/menu/Background.png") ;
    Texture BackBtn = SBDL::loadTexture("assets/pic/menu/back.png") ;
    Texture ChangeBtn = SBDL::loadTexture("assets/pic/menu/change_keys.png") ;
    Texture PlayBtn = SBDL::loadTexture("assets/pic/menu/play.png") ;
    Texture QuitBtn = SBDL::loadTexture("assets/pic/menu/quit.png") ;
    Texture ResumeBtn = SBDL::loadTexture("assets/pic/menu/resume.png") ;
    Texture RetryBtn = SBDL::loadTexture("assets/pic/menu/retry.png") ;
    Texture SettingBtn = SBDL::loadTexture("assets/pic/menu/setting.png") ;
    Texture BuyResumeBtn = SBDL::loadTexture("assets/pic/menu/resume_500.png") ;
    Texture SoundBtn = SBDL::loadTexture("assets/pic/menu/sound.png") ;
    Texture SoundOffBtn = SBDL::loadTexture("assets/pic/menu/sound_off.png") ;




    Texture DistanceGameOver_Text;
    Texture BestGameOver_Text;
    Texture CoinGameOver_Text ;
    Texture NotEnoughCoin_Text  = SBDL::createFontTexture(DistanceFont , "Not Enough Coins" , 255 ,255 ,255 )  ;


    // Sector Txtures
    Texture SectorStart = SBDL::loadTexture("assets/pic/back/Sector_Start.png");
    Texture Sector2 = SBDL::loadTexture("assets/pic/back/Sector1.png");
    Texture Sector1 = SBDL::loadTexture("assets/pic/back/Sector2.png");

    // Lab Textures
    Texture LabStart = SBDL::loadTexture("assets/pic/back/Lab_Start.png");
    Texture Lab2 = SBDL::loadTexture("assets/pic/back/Lab1.png");
    Texture Lab1 = SBDL::loadTexture("assets/pic/back/Lab2.png");

    // Volcano Textures
    Texture VolcanoStart = SBDL::loadTexture("assets/pic/back/Volcano_Start.png");
    Texture Volcano2 = SBDL::loadTexture("assets/pic/back/Volcano1.png");
    Texture Volcano1 = SBDL::loadTexture("assets/pic/back/Volcano2.png");

    Texture WareHouseStart  = SBDL::loadTexture("assets/pic/back/warehouse_Start.png") ;
    Texture WareHouse1 = SBDL::loadTexture("assets/pic/back/warehouse.png") ;

    // Text Textures
    Texture CoinScore_Text;
    Texture Distance_Text;

    // Gravity Token Texture
    Texture GravityToken = SBDL::loadTexture("assets/pic/menu/gravity_token.png") ;

    //Barry Texture
    MovingThing MovingBarry;
    MovingBarry.images = new Texture[8];
    MovingBarry.size = 6;
    MovingBarry.CurrentIndex = 0;
    MovingBarry.images[0] = SBDL::loadTexture("assets/pic/barry/barry.png");
    MovingBarry.images[1] = SBDL::loadTexture("assets/pic/barry/barry.png");
    MovingBarry.images[2] = SBDL::loadTexture("assets/pic/barry/barry.png");
    MovingBarry.images[3] = SBDL::loadTexture("assets/pic/barry/barry2.png");
    MovingBarry.images[4] = SBDL::loadTexture("assets/pic/barry/barry2.png");
    MovingBarry.images[5] = SBDL::loadTexture("assets/pic/barry/barry2.png");
    MovingBarry.images[6] = SBDL::loadTexture("assets/pic/barry/barry3.png");
    MovingBarry.images[7] = SBDL::loadTexture("assets/pic/barry/barry4.png");

    // Alien Txture
    MovingThing MovingAlien;
    MovingAlien.images = new Texture[8];
    MovingAlien.size = 6;
    MovingAlien.CurrentIndex = 0;
    MovingAlien.images[0] = SBDL::loadTexture("assets/pic/barry/alien.png");
    MovingAlien.images[1] = SBDL::loadTexture("assets/pic/barry/alien.png");
    MovingAlien.images[2] = SBDL::loadTexture("assets/pic/barry/alien.png");
    MovingAlien.images[3] = SBDL::loadTexture("assets/pic/barry/alien2.png");
    MovingAlien.images[4] = SBDL::loadTexture("assets/pic/barry/alien2.png");
    MovingAlien.images[5] = SBDL::loadTexture("assets/pic/barry/alien2.png");
    MovingAlien.images[6] = SBDL::loadTexture("assets/pic/barry/alienup.png");
    MovingAlien.images[7] = SBDL::loadTexture("assets/pic/barry/aliendown.png");


    MovingThing GravitySuit ;
    GravitySuit.images = new Texture[14] ;
    GravitySuit.CurrentIndex = 0 ;
    GravitySuit.size = 12 ;
    GravitySuit.images[0] = SBDL::loadTexture("assets/pic/barry/ggdown.png") ;
    GravitySuit.images[1] = SBDL::loadTexture("assets/pic/barry/gg1.png") ;
    GravitySuit.images[2] = SBDL::loadTexture("assets/pic/barry/gg1.png") ;
    GravitySuit.images[3] = SBDL::loadTexture("assets/pic/barry/gg1.png") ;
    GravitySuit.images[4] = SBDL::loadTexture("assets/pic/barry/gg2.png") ;
    GravitySuit.images[5] = SBDL::loadTexture("assets/pic/barry/gg2.png") ;
    GravitySuit.images[6] = SBDL::loadTexture("assets/pic/barry/gg2.png") ;
    GravitySuit.images[7] = SBDL::loadTexture("assets/pic/barry/ggup.png") ;
    GravitySuit.images[8] =   SBDL::loadTexture("assets/pic/barry/gg3.png") ;
    GravitySuit.images[9] =   SBDL::loadTexture("assets/pic/barry/gg3.png") ;
    GravitySuit.images[10] =   SBDL::loadTexture("assets/pic/barry/gg3.png") ;
    GravitySuit.images[11] =   SBDL::loadTexture("assets/pic/barry/gg4.png") ;
    GravitySuit.images[12] = SBDL::loadTexture("assets/pic/barry/gg4.png") ;
    GravitySuit.images[13] = SBDL::loadTexture("assets/pic/barry/gg4.png") ;


    // Scientists Textures
    MovingThing *MovingScientist = new MovingThing[4];
    MovingScientist[0].images = new Texture[12];
    MovingScientist[0].size = 6;
    MovingScientist[0].CurrentIndex = 0;
    MovingScientist[0].images[0] = SBDL::loadTexture("assets/pic/scientist/walk.png");
    MovingScientist[0].images[1] = SBDL::loadTexture("assets/pic/scientist/walk.png");
    MovingScientist[0].images[2] = SBDL::loadTexture("assets/pic/scientist/walk.png");
    MovingScientist[0].images[3] = SBDL::loadTexture("assets/pic/scientist/walk2.png");
    MovingScientist[0].images[4] = SBDL::loadTexture("assets/pic/scientist/walk2.png");
    MovingScientist[0].images[5] = SBDL::loadTexture("assets/pic/scientist/walk2.png");
    MovingScientist[0].images[6] = SBDL::loadTexture("assets/pic/scientist/run.png");
    MovingScientist[0].images[7] = SBDL::loadTexture("assets/pic/scientist/run.png");
    MovingScientist[0].images[8] = SBDL::loadTexture("assets/pic/scientist/run.png");
    MovingScientist[0].images[9] = SBDL::loadTexture("assets/pic/scientist/run2.png");
    MovingScientist[0].images[10] = SBDL::loadTexture("assets/pic/scientist/run2.png");
    MovingScientist[0].images[11] = SBDL::loadTexture("assets/pic/scientist/run2.png");
    MovingScientist[1].images = new Texture[12];
    MovingScientist[1].size = 6;
    MovingScientist[1].CurrentIndex = 0;
    MovingScientist[1].images[0] = SBDL::loadTexture("assets/pic/scientist/walk_2.png");
    MovingScientist[1].images[1] = SBDL::loadTexture("assets/pic/scientist/walk_2.png");
    MovingScientist[1].images[2] = SBDL::loadTexture("assets/pic/scientist/walk_2.png");
    MovingScientist[1].images[3] = SBDL::loadTexture("assets/pic/scientist/walk2_2.png");
    MovingScientist[1].images[4] = SBDL::loadTexture("assets/pic/scientist/walk2_2.png");
    MovingScientist[1].images[5] = SBDL::loadTexture("assets/pic/scientist/walk2_2.png");
    MovingScientist[1].images[6] = SBDL::loadTexture("assets/pic/scientist/run_2.png");
    MovingScientist[1].images[7] = SBDL::loadTexture("assets/pic/scientist/run_2.png");
    MovingScientist[1].images[8] = SBDL::loadTexture("assets/pic/scientist/run_2.png");
    MovingScientist[1].images[9] = SBDL::loadTexture("assets/pic/scientist/run2_2.png");
    MovingScientist[1].images[10] = SBDL::loadTexture("assets/pic/scientist/run2_2.png");
    MovingScientist[1].images[11] = SBDL::loadTexture("assets/pic/scientist/run2_2.png");
    MovingScientist[2].images = new Texture[12];
    MovingScientist[2].size = 6;
    MovingScientist[2].CurrentIndex = 0;
    MovingScientist[2].images[0] = SBDL::loadTexture("assets/pic/scientist/walk3.png");
    MovingScientist[2].images[1] = SBDL::loadTexture("assets/pic/scientist/walk3.png");
    MovingScientist[2].images[2] = SBDL::loadTexture("assets/pic/scientist/walk3.png");
    MovingScientist[2].images[3] = SBDL::loadTexture("assets/pic/scientist/walk4.png");
    MovingScientist[2].images[4] = SBDL::loadTexture("assets/pic/scientist/walk4.png");
    MovingScientist[2].images[5] = SBDL::loadTexture("assets/pic/scientist/walk4.png");
    MovingScientist[2].images[6] = SBDL::loadTexture("assets/pic/scientist/run.png");
    MovingScientist[2].images[7] = SBDL::loadTexture("assets/pic/scientist/run.png");
    MovingScientist[2].images[8] = SBDL::loadTexture("assets/pic/scientist/run.png");
    MovingScientist[2].images[9] = SBDL::loadTexture("assets/pic/scientist/run2.png");
    MovingScientist[2].images[10] = SBDL::loadTexture("assets/pic/scientist/run2.png");
    MovingScientist[2].images[11] = SBDL::loadTexture("assets/pic/scientist/run2.png");
    MovingScientist[3].images = new Texture[12];
    MovingScientist[3].size = 6;
    MovingScientist[3].CurrentIndex = 0;
    MovingScientist[3].images[0] = SBDL::loadTexture("assets/pic/scientist/walk3_2.png");
    MovingScientist[3].images[1] = SBDL::loadTexture("assets/pic/scientist/walk3_2.png");
    MovingScientist[3].images[2] = SBDL::loadTexture("assets/pic/scientist/walk3_2.png");
    MovingScientist[3].images[3] = SBDL::loadTexture("assets/pic/scientist/walk4_2.png");
    MovingScientist[3].images[4] = SBDL::loadTexture("assets/pic/scientist/walk4_2.png");
    MovingScientist[3].images[5] = SBDL::loadTexture("assets/pic/scientist/walk4_2.png");
    MovingScientist[3].images[6] = SBDL::loadTexture("assets/pic/scientist/run_2.png");
    MovingScientist[3].images[7] = SBDL::loadTexture("assets/pic/scientist/run_2.png");
    MovingScientist[3].images[8] = SBDL::loadTexture("assets/pic/scientist/run_2.png");
    MovingScientist[3].images[9] = SBDL::loadTexture("assets/pic/scientist/run2_2.png");
    MovingScientist[3].images[10] = SBDL::loadTexture("assets/pic/scientist/run2_2.png");
    MovingScientist[3].images[11] = SBDL::loadTexture("assets/pic/scientist/run2_2.png");



    // Horizntal Zapper
    MovingThing ZapperH[2];
    ZapperH[0].size = 8;
    ZapperH[0].CurrentIndex = 0;
    ZapperH[0].images = new Texture[8];
    ZapperH[0].images[0] = SBDL::loadTexture("assets/pic/zappers/h1.png");
    ZapperH[0].images[1] = SBDL::loadTexture("assets/pic/zappers/h1.png");
    ZapperH[0].images[2] = SBDL::loadTexture("assets/pic/zappers/h2.png");
    ZapperH[0].images[3] = SBDL::loadTexture("assets/pic/zappers/h2.png");
    ZapperH[0].images[4] = SBDL::loadTexture("assets/pic/zappers/h3.png");
    ZapperH[0].images[5] = SBDL::loadTexture("assets/pic/zappers/h3.png");
    ZapperH[0].images[6] = SBDL::loadTexture("assets/pic/zappers/h4.png");
    ZapperH[0].images[7] = SBDL::loadTexture("assets/pic/zappers/h4.png");
    ZapperH[1].size = 8;
    ZapperH[1].CurrentIndex = 0;
    ZapperH[1].images = new Texture[8];
    ZapperH[1].images[0] = SBDL::loadTexture("assets/pic/zappers/h1.png");
    ZapperH[1].images[1] = SBDL::loadTexture("assets/pic/zappers/h1.png");
    ZapperH[1].images[2] = SBDL::loadTexture("assets/pic/zappers/h2.png");
    ZapperH[1].images[3] = SBDL::loadTexture("assets/pic/zappers/h2.png");
    ZapperH[1].images[4] = SBDL::loadTexture("assets/pic/zappers/h3.png");
    ZapperH[1].images[5] = SBDL::loadTexture("assets/pic/zappers/h3.png");
    ZapperH[1].images[6] = SBDL::loadTexture("assets/pic/zappers/h4.png");
    ZapperH[1].images[7] = SBDL::loadTexture("assets/pic/zappers/h4.png");


    // Vertical Zapper
    MovingThing ZapperV[2];
    ZapperV[0].size = 8;
    ZapperV[0].CurrentIndex = 0;
    ZapperV[0].images = new Texture[8];
    ZapperV[0].images[0] = SBDL::loadTexture("assets/pic/zappers/v1.png");
    ZapperV[0].images[1] = SBDL::loadTexture("assets/pic/zappers/v1.png");
    ZapperV[0].images[2] = SBDL::loadTexture("assets/pic/zappers/v2.png");
    ZapperV[0].images[3] = SBDL::loadTexture("assets/pic/zappers/v2.png");
    ZapperV[0].images[4] = SBDL::loadTexture("assets/pic/zappers/v3.png");
    ZapperV[0].images[5] = SBDL::loadTexture("assets/pic/zappers/v3.png");
    ZapperV[0].images[6] = SBDL::loadTexture("assets/pic/zappers/v4.png");
    ZapperV[0].images[7] = SBDL::loadTexture("assets/pic/zappers/v4.png");
    ZapperV[1].size = 8;
    ZapperV[1].CurrentIndex = 0;
    ZapperV[1].images = new Texture[8];
    ZapperV[1].images[0] = SBDL::loadTexture("assets/pic/zappers/v1.png");
    ZapperV[1].images[1] = SBDL::loadTexture("assets/pic/zappers/v1.png");
    ZapperV[1].images[2] = SBDL::loadTexture("assets/pic/zappers/v2.png");
    ZapperV[1].images[3] = SBDL::loadTexture("assets/pic/zappers/v2.png");
    ZapperV[1].images[4] = SBDL::loadTexture("assets/pic/zappers/v3.png");
    ZapperV[1].images[5] = SBDL::loadTexture("assets/pic/zappers/v3.png");
    ZapperV[1].images[6] = SBDL::loadTexture("assets/pic/zappers/v4.png");
    ZapperV[1].images[7] = SBDL::loadTexture("assets/pic/zappers/v4.png");


    MovingThing Missle;
    Missle.size = 6;
    Missle.CurrentIndex = 0;
    Missle.images = new Texture[8];
    Missle.images[0] = SBDL::loadTexture("assets/pic/missle/missle_1.png");
    Missle.images[1] = SBDL::loadTexture("assets/pic/missle/missle_2.png");
    Missle.images[2] = SBDL::loadTexture("assets/pic/missle/missle_3.png");
    Missle.images[3] = SBDL::loadTexture("assets/pic/missle/missle_4.png");
    Missle.images[4] = SBDL::loadTexture("assets/pic/missle/missle_5.png");
    Missle.images[5] = SBDL::loadTexture("assets/pic/missle/missle_6.png");
    Missle.images[6] = SBDL::loadTexture("assets/pic/missle/2m.png");
    Missle.images[7] = SBDL::loadTexture("assets/pic/missle/1m.png");

    MovingThing Smoke;
    Smoke.size = 6;
    Smoke.CurrentIndex = 0;
    Smoke.images = new Texture[6];
    Smoke.images[0] = SBDL::loadTexture("assets/pic/smoke/smoke1.png");
    Smoke.images[1] = SBDL::loadTexture("assets/pic/smoke/smoke2.png");
    Smoke.images[2] = SBDL::loadTexture("assets/pic/smoke/smoke3.png");
    Smoke.images[3] = SBDL::loadTexture("assets/pic/smoke/smoke4.png");
    Smoke.images[4] = SBDL::loadTexture("assets/pic/smoke/smoke5.png");
    Smoke.images[5] = SBDL::loadTexture("assets/pic/smoke/smoke6.png");


    MovingThing Lazer ;
    Lazer.size = 4 ;
    Lazer.CurrentIndex = 0 ;
    Lazer.images = new Texture[6] ;
    Lazer.images[0] = SBDL::loadTexture("assets/pic/lazer/laser_noneactive.png") ;
    Lazer.images[1] = SBDL::loadTexture("assets/pic/lazer/laser_active1.png") ;
    Lazer.images[2] = SBDL::loadTexture("assets/pic/lazer/laser_active1.png") ;
    Lazer.images[3] = SBDL::loadTexture("assets/pic/lazer/laser_active2.png") ;
    Lazer.images[4] = SBDL::loadTexture("assets/pic/lazer/laser_active2.png") ;


    MovingThing MovingSpeedToken ;
    MovingSpeedToken.CurrentIndex = 0  ;
    MovingSpeedToken.images = new Texture[8] ;
    MovingSpeedToken.size = 8 ;
    MovingSpeedToken.images[0] = SBDL::loadTexture("assets/pic/speedToken/speedtoken.png") ;
    MovingSpeedToken.images[1] = SBDL::loadTexture("assets/pic/speedToken/speedtoken.png") ;
    MovingSpeedToken.images[2] = SBDL::loadTexture("assets/pic/speedToken/speedtoken2.png") ;
    MovingSpeedToken.images[3] = SBDL::loadTexture("assets/pic/speedToken/speedtoken2.png") ;
    MovingSpeedToken.images[4] = SBDL::loadTexture("assets/pic/speedToken/speedtoken3.png") ;
    MovingSpeedToken.images[5] = SBDL::loadTexture("assets/pic/speedToken/speedtoken3.png") ;
    MovingSpeedToken.images[6] = SBDL::loadTexture("assets/pic/speedToken/speedtoken4.png") ;
    MovingSpeedToken.images[7] = SBDL::loadTexture("assets/pic/speedToken/speedtoken4.png") ;





    PlayerIcon PlayerStatus = Barry;
    GameStatus GameStat = Menu;

    // Rects
    SDL_Rect SectorStartRect = {0, 0, SectorStart.width+30, SectorStart.height};
    SDL_Rect Sector1Rect = {2048, 0, Sector1.width + 30, Sector1.height};
    SDL_Rect Sector2Rect = {4096, 0, Sector2.width + 30, Sector2.height};

    SDL_Rect LabStartRect = {3*2048, 0, SectorStart.width+30, SectorStart.height};
    SDL_Rect Lab1Rect = {4*2048, 0, Sector1.width + 30, Sector1.height};
    SDL_Rect Lab2Rect = {5*2048, 0, Sector2.width + 30, Sector2.height};

    SDL_Rect VolcanoStartRect = {6*2048, 0, SectorStart.width+30, SectorStart.height};
    SDL_Rect Volcano1Rect = {7*2048, 0, Sector1.width + 30, Sector1.height};
    SDL_Rect Volcano2Rect = {8*2048, 0, Sector2.width + 30, Sector2.height};

    SDL_Rect WareHouseStartRect = {9*2048, 0, SectorStart.width+30, SectorStart.height};
    SDL_Rect WareHouse1Rect = {10*2048, 0, Sector1.width + 30, Sector1.height};
    


    SDL_Rect BarryRect = {180, 360, 60, 72};

    SDL_Rect ScientistRect[4];
    ScientistRect[0] = {rand() % 1100 + 1100, 380, 30, 50};
    ScientistRect[1] = {rand() % 1100 + 1100, 380, 50, 50};
    ScientistRect[2] = {rand() % 1100 + 1100, 380, 30, 50};
    ScientistRect[3] = {rand() % 1100 + 1100, 380, 30, 50};

    SDL_Rect ZapperHRect = {1100 + (rand() % 2) * 1100, 10 + rand() % 300, 150, 70};
    SDL_Rect ZapperH2Rect = {-500, 10 + rand() % 300, 150, 70};

    SDL_Rect ZapperVRect = {-500, 10 + rand() % 200, 70, 150};
    SDL_Rect ZapperV2Rect = {-500, 10 + rand() % 200, 70, 150};


    SDL_Rect MissleRect = {3000, 200, Missle.images[0].width, Missle.images[0].height};

    SDL_Rect LazerRect = {-800 , 10 + rand() % 300 , 350 , 30} ;

    SDL_Rect GravityTokenRect = { (rand() % 5 + 1)*1100 + 1100   , 10 + rand() % 300 , 100  , 100};

    SDL_Rect SpeedTokenRect = {-500, 0 , 60, 60 } ;

    SDL_Rect SettingBtnRect = {880 , 80 , 148 , 50} ;

    SDL_Rect PlayBtnRect = {880 , 10 , 148 , 50} ;
    
    SDL_Rect QuitBtnRect = {880 , 400 , 148 , 50} ;

    SDL_Rect BackBtnRect = {880 , 400 , 148 , 50} ;

    SDL_Rect SoundBtnRect = {880 , 250 , 148 , 50} ;

    SDL_Rect ChangeBtnRect = {880 , 320 , 148 , 50} ;

    SDL_Rect ResumeBtnRect = {880 , 10 , 148 , 50} ;

    SDL_Rect RetryBtnRect = {880 , 130 , 148 , 50} ;

    SDL_Rect BuyResumeBtnRect = {900 , 10 , 100, 100} ; 


    // Create a 2D array Rect for each Coin
    int RandomXD = rand() % 2400;
    int RandomYD = rand() % 300 + 10;
    SDL_Rect **coinsArRect = new SDL_Rect *[linesCounter];
    for (int i = 0; i < linesCounter; i++)
        coinsArRect[i] = new SDL_Rect[eachLineLength];

    for (int i = 0; i < linesCounter; i++) {
        for (int j = 0; j < eachLineLength; j++) {
            if (grid[i][j] == 1) {
                coinsArRect[i][j] = {
                        30 * j + 60 + RandomXD - coinsAr[i][j].images[coinsAr[i][j].currentIndex].width / 2 + 1300,
                        30 * i + RandomYD, 30, 30};
            } else
                coinsArRect[i][j] = {-5000, -5000, 0, 0};
        }
    }


    // Variables
    int Speed = 0;
    int BgSpeed = 10;
    int CoinScore = 0;
    int Distance = 0;
    int counter = 0;
    bool fear[4] = {false, false, false, false};
    bool CoinNeed = true;
    bool RandomNeed = false;
    bool BringAOpject = true;
    bool Bring[7] = {false, false, false, false, false, false , false};
    bool MusicStatus = true;
    bool SoundStatus = true;
    bool isMusicPlay = true;
    bool GravitySuitBool = false;
    bool SpeedToken = false;
    bool isHitten = false ;
    bool GravityPlace  = 0;
    int TokenCounter = 0 ;
    int Yspeed = 5 ;
    int Best=0 ;


    ifstream myfile ;
    myfile.open("Best.txt") ;
    myfile >> Best ;
    myfile.close() ;


    while (SBDL::isRunning()) {
        int startTime = SBDL::getTime();
        SBDL::updateEvents();
        SBDL::clearRenderScreen();


        if (MusicStatus && isMusicPlay) {
            SBDL::playMusic(BgMusic, 1);
            isMusicPlay = false;
        }
        if (!MusicStatus && !isMusicPlay) {
            SBDL::stopMusic();
        }


        if (GameStat == Game) {



            counter++;


            if (SpeedToken ) {
                BgSpeed =  30;
                TokenCounter++ ;
                Distance+=5 ;
                if (TokenCounter >= 200) {
                    SpeedToken = false ;
                    TokenCounter = 0 ;
                    BgSpeed = 10 ;
                }
            }

            if (Distance > 2000 && Distance < 3000)
                BgSpeed = 15 ;
            if (Distance > 3000)
                BgSpeed = 20 ;


            // Music 




                SBDL::showTexture(SectorStart, SectorStartRect);
                SBDL::showTexture(Sector2, Sector2Rect);
                SBDL::showTexture(Sector1, Sector1Rect);


                SBDL::showTexture(Lab2, Lab2Rect);
                SBDL::showTexture(Lab1, Lab1Rect);
                SBDL::showTexture(LabStart, LabStartRect);

                SBDL::showTexture(Volcano2, Volcano2Rect);
                SBDL::showTexture(Volcano1, Volcano1Rect);
                SBDL::showTexture(VolcanoStart, VolcanoStartRect);

                SBDL::showTexture(WareHouseStart , WareHouseStartRect ) ;
                SBDL::showTexture(WareHouse1 , WareHouse1Rect) ;



            // Scientists Textures
            SBDL::showTexture(MovingScientist[0].images[MovingScientist[0].CurrentIndex], ScientistRect[0]);
            SBDL::showTexture(MovingScientist[1].images[MovingScientist[1].CurrentIndex], ScientistRect[1]);
            SBDL::showTexture(MovingScientist[2].images[MovingScientist[2].CurrentIndex], ScientistRect[2]);
            SBDL::showTexture(MovingScientist[3].images[MovingScientist[3].CurrentIndex], ScientistRect[3]);
            // Create Font Texture for Texts
            CoinScore_Text = SBDL::createFontTexture(CoinScoreFont, to_string(CoinScore), 255, 255, 255);
            Distance_Text = SBDL::createFontTexture(DistanceFont, to_string(Distance) + " M", 255, 255, 255);


            // Show Texts Textures
            SBDL::showTexture(CoinScore_Text, 0, 30);
            SBDL::showTexture(Distance_Text, 0, 0);
            SBDL::showTexture(Coin, {40, 30, 20, 20});

            // Show Coins Textures
            for (int i = 0; i < linesCounter; i++) {
                for (int j = 0; j < eachLineLength; j++) {
                    if (grid[i][j] == 1) {
                        if (showCoin[i][j]) {
                            SBDL::showTexture(coinsAr[i][j].images[coinsAr[i][j].currentIndex], coinsArRect[i][j]);
                        }
                        if (coinsArRect[i][j].x <= -1000) {
                            RandomNeed = true;
                            showCoin[i][j] = true;
                        }

                        if (counter > 2)
                            coinsAr[i][j].currentIndex = (coinsAr[i][j].currentIndex + 1) % coinsAr[i][j].size;
                    }
                }
            }
            if (counter > 2)
                counter = 0;


            SBDL::showTexture(GravityToken ,GravityTokenRect) ;


            // Zappers Textures
            SBDL::showTexture(ZapperH[0].images[ZapperH[0].CurrentIndex], ZapperHRect);
            ZapperH[0].CurrentIndex++;
            if (ZapperH[0].CurrentIndex >= ZapperH[0].size)
                ZapperH[0].CurrentIndex = 0;

            SBDL::showTexture(ZapperH[1].images[ZapperH[1].CurrentIndex], ZapperH2Rect);
            ZapperH[1].CurrentIndex++;
            if (ZapperH[1].CurrentIndex >= ZapperH[1].size)
                ZapperH[1].CurrentIndex = 0;

            SBDL::showTexture(ZapperV[0].images[ZapperV[0].CurrentIndex], ZapperVRect);
            ZapperV[0].CurrentIndex++;
            if (ZapperV[0].CurrentIndex >= ZapperV[0].size)
                ZapperV[0].CurrentIndex = 0;

            SBDL::showTexture(ZapperV[1].images[ZapperV[1].CurrentIndex], ZapperV2Rect);
            ZapperV[1].CurrentIndex++;
            if (ZapperV[1].CurrentIndex >= ZapperV[1].size)
                ZapperV[1].CurrentIndex = 0;

            // Missle Textures
            SBDL::showTexture(Missle.images[Missle.CurrentIndex], MissleRect);
            SBDL::showTexture(Smoke.images[Smoke.CurrentIndex], MissleRect.x + 130, MissleRect.y);
            Missle.CurrentIndex++;
            Smoke.CurrentIndex++;
            if (Missle.CurrentIndex >= Missle.size)
                Missle.CurrentIndex = 0;
            if (Smoke.CurrentIndex >= Smoke.size)
                Smoke.CurrentIndex = 0;

            if (MissleRect.x <= 2400 && MissleRect.x > 1500) {
                SBDL::showTexture(Missle.images[6], {1040, MissleRect.y, 50, 50});
                SBDL::playSound(MissleWaringSound , 1 ) ;
            }
            if (MissleRect.x <= 1500 && MissleRect.x >= 1100) {
                SBDL::showTexture(Missle.images[7], {1040, MissleRect.y, 50, 50});
                SBDL::playSound(MissleFireSound , 1 ) ;
            }

            SBDL::showTexture(Lazer.images[Lazer.CurrentIndex] , LazerRect) ;
            if (Lazer.CurrentIndex !=0)
                Lazer.CurrentIndex++ ;
            if (Lazer.CurrentIndex >= 4)
                Lazer.CurrentIndex =  1 ;

            if (LazerRect.x <= 0)
                Lazer.CurrentIndex = 1 ;

            SBDL::showTexture(MovingSpeedToken.images[MovingSpeedToken.CurrentIndex] , SpeedTokenRect) ;
            MovingSpeedToken.CurrentIndex++ ;
            if (MovingSpeedToken.CurrentIndex >= MovingSpeedToken.size)
                MovingSpeedToken.CurrentIndex = 0 ;


            // Player Texture
            if (!GravitySuitBool) {
                if (PlayerStatus == Barry)
                    SBDL::showTexture(MovingBarry.images[MovingBarry.CurrentIndex], BarryRect);
                if (PlayerStatus == Alien)
                    SBDL::showTexture(MovingAlien.images[MovingAlien.CurrentIndex], BarryRect);
            }
            else {
                SBDL::showTexture(GravitySuit.images[GravitySuit.CurrentIndex] , BarryRect) ;
            }





            // Coin Random 
            if (RandomNeed) {
                int RandomX = rand() % 200 + 1100;
                int RandomY = rand() % 200 + 10;
                for (int i = 0; i < linesCounter; i++) {
                    for (int j = 0; j < eachLineLength; j++) {
                        if (grid[i][j] == 1) {
                            coinsArRect[i][j] = {
                                    30 * j + 60 + RandomX - coinsAr[i][j].images[coinsAr[i][j].currentIndex].width / 2 +
                                    1300,
                                    30 * i + RandomY, 30, 30};
                            showCoin[i][j] = true;
                        } else
                            coinsArRect[i][j] = {-5000, -5000, 0, 0};
                    }
                }
                RandomNeed = false;
            }


            // Intersecetion with Coins
            for (int i = 0; i < linesCounter; i++) {
                for (int j = 0; j < eachLineLength; j++) {
                    if (SBDL::hasIntersectionRect(BarryRect, coinsArRect[i][j])) {
                        if (showCoin[i][j]) {
                            CoinScore+=1;
                            if (SoundStatus)
                                SBDL::playSound(CoinSound, 1);
                        }
                        showCoin[i][j] = false;
                    }
                }
            }

            if (!GravitySuitBool) {
                GravityTokenRect.x -= BgSpeed ;
            }
            if (GravitySuitBool) {
                GravityTokenRect.x = (rand() % 5 + 1) * 1100 + 1100 ;
                GravityTokenRect.y = rand() % 300 + 10 ;

            }

            if (GravityTokenRect.x <= -500) {
                GravityTokenRect.x = (rand() % 5 + 1) * 1100 + 1100 ;
                GravityTokenRect.y = rand() % 300 + 10 ;
            }


            if (!SpeedToken) {
                SpeedTokenRect.x += BgSpeed - 8  ;
                SpeedTokenRect.y +=Yspeed ;
                if (SpeedTokenRect.y >260)
                    Yspeed = -5 ;
                if (SpeedTokenRect.y <=100 )
                    Yspeed = 5 ;
            }
            if (SpeedToken) {
                SpeedTokenRect.x = ((rand() % 10 + 5) * 1100 + 1100)* (-1) ;
            }
            if (SpeedTokenRect.x <= -500) {
                SpeedTokenRect.x = ((rand() % 5 + 1) * 1100 + 1100 ) *(-1) ;
                SpeedTokenRect.y = 0;
            }



            if (LazerRect.x >10 && LazerRect.x < 700 )
                SBDL::playSound(LazerWarning , 1) ;


            // Intersection with Zappers
            if (!SpeedToken) {
                if ((SBDL::hasIntersectionRect(BarryRect, ZapperHRect) ||
                     SBDL::hasIntersectionRect(BarryRect, ZapperH2Rect) ||
                     SBDL::hasIntersectionRect(BarryRect, ZapperVRect) ||
                     SBDL::hasIntersectionRect(BarryRect, ZapperV2Rect) ||
                     SBDL::hasIntersectionRect(BarryRect, MissleRect)) && isHitten == false) {
                    isHitten = true;
                    if (SoundStatus)
                        SBDL::playSound(LazerSound , 1) ;
                    if (!GravitySuitBool)
                        GameStat = Lose;
                    if (GravitySuitBool) {
                        GravitySuitBool = false;
                    }
                }
                if(SBDL::hasIntersectionRect(BarryRect  , LazerRect) && Lazer.CurrentIndex!=0 && isHitten == false) {
                    isHitten = true;
                    if (SoundStatus)
                        SBDL::playSound(LazerSound , 1) ;
                    if (!GravitySuitBool)
                        GameStat = Lose;
                    if (GravitySuitBool) {
                        GravitySuitBool = false;
                    }
                }
            }


            if (SBDL::hasIntersectionRect(BarryRect, GravityTokenRect))
                GravitySuitBool = true ;
            if (SBDL::hasIntersectionRect(BarryRect , SpeedTokenRect))
                SpeedToken =true ;

            if (!GravitySuitBool) {
                // Player Keyboard Handling
                if (SBDL::keyHeld(SDL_SCANCODE_SPACE) && BarryRect.y >= 0) {
                    Speed-=1;
                    MovingBarry.CurrentIndex = 6;
                    MovingAlien.CurrentIndex = 6;
                    for (int i = 0; i < 4; i++) {
                        if (abs(BarryRect.x - ScientistRect[i].x) <= 300)
                            fear[i] = true;
                    }
                }
                if (!SBDL::keyHeld(SDL_SCANCODE_SPACE) && BarryRect.y < 360) {
                    Speed+=1;
                    MovingBarry.CurrentIndex = 7;
                    MovingAlien.CurrentIndex = 7;
                }

                // Barry Moving Up and Down with Acceleration
                BarryRect.y += Speed;
                if (BarryRect.y >= 360) {
                    BarryRect.y = 360;
                    Speed = 0;
                    MovingBarry.CurrentIndex++;
                    MovingAlien.CurrentIndex++;
                    if (MovingBarry.CurrentIndex >= 5 || MovingAlien.CurrentIndex >= 5) {
                        MovingBarry.CurrentIndex = 0;
                        MovingAlien.CurrentIndex = 0;
                    }
                }
                if (BarryRect.y <= 0) {
                    BarryRect.y = 0;
                    Speed = 0;
                }
            }
            else {
                if (SBDL::keyPressed(SDL_SCANCODE_SPACE) ) {
                    GravityPlace = !GravityPlace ;
                    if (GravityPlace == 0)
                        GravitySuit.CurrentIndex = 0 ;
                    if (GravityPlace == 1)
                        GravitySuit.CurrentIndex = 7 ;
                }

                // Barry Moving Up and Down with Acceleration
                if (GravityPlace == 0) {
                    BarryRect.y += 15;
                }
                if (GravityPlace == 1) {
                    BarryRect.y -=15 ;
                }
                if (BarryRect.y >= 360) {
                    BarryRect.y = 360;
                    GravitySuit.CurrentIndex++;
                    if (GravitySuit.CurrentIndex >= 6 ) {
                        GravitySuit.CurrentIndex = 1;
                    }
                }
                if (BarryRect.y <= 0) {
                    BarryRect.y = 0;
                    GravitySuit.CurrentIndex++ ;
                    if (GravitySuit.CurrentIndex >=13)
                        GravitySuit.CurrentIndex = 8 ;
                }
            }


            // Pause Keyboard Handling
            if (SBDL::keyPressed(SDL_SCANCODE_P))
                GameStat = Pause;





            // Moving Scientists
            for (int i = 0; i < 2; i++) {
                if (fear[i]) {
                    ScientistRect[i].w = 50;
                    MovingScientist[i].CurrentIndex++;
                    if (MovingScientist[i].CurrentIndex >= 12 || MovingScientist[i].CurrentIndex < 6)
                        MovingScientist[i].CurrentIndex = 6;
                    ScientistRect[i].x -= 2;
                } else {
                    MovingScientist[i].CurrentIndex++;
                    if (MovingScientist[i].CurrentIndex >= 6)
                        MovingScientist[i].CurrentIndex = 0;
                    ScientistRect[i].x -= 5;
                }
            }

            // Moving Scientists 2 
            for (int i = 2; i < 4; i++) {
                if (fear[i]) {
                    ScientistRect[i].w = 50;
                    MovingScientist[i].CurrentIndex++;
                    if (MovingScientist[i].CurrentIndex >= 12 || MovingScientist[i].CurrentIndex < 6)
                        MovingScientist[i].CurrentIndex = 6;
                    ScientistRect[i].x -= 2;
                } else {
                    MovingScientist[i].CurrentIndex++;
                    if (MovingScientist[i].CurrentIndex >= 6)
                        MovingScientist[i].CurrentIndex = 0;
                    ScientistRect[i].x -= 11;
                }
            }




            // Random Scientists 
            if (ScientistRect[0].x <= -100) {
                ScientistRect[0].x = 1100 + rand() % 550 + rand() % 550;
                fear[0] = false;
                ScientistRect[0].w = 30;
            }
            if (ScientistRect[1].x <= -100) {
                ScientistRect[1].x = 2000 + rand() % 550 + rand() % 550;
                fear[1] = false;
            }
            if (ScientistRect[2].x <= -100) {
                ScientistRect[2].x = 2200 + rand() % 550 + rand() % 550;
                fear[2] = false;
                ScientistRect[2].w = 30;
            }
            if (ScientistRect[3].x <= -100) {
                ScientistRect[3].x = 2200 + rand() % 550 + rand() % 550;
                fear[3] = false;
                ScientistRect[3].w = 30;

            }

            // Moving Opjects And Back
            Distance++;


            if (BringAOpject) {
                int RandomBring = rand() % 7;
                if (RandomBring == 0) {
                    Bring[0] = true;
                    BringAOpject = false;
                }
                if (RandomBring == 1) {
                    Bring[1] = true;
                    BringAOpject = false;

                }
                if (RandomBring == 2) {
                    Bring[2] = true;
                    BringAOpject = false;

                }
                if (RandomBring == 3) {
                    Bring[0] = true;
                    BringAOpject = false;

                }
                if (RandomBring == 4) {
                    Bring[4] = true;
                    BringAOpject = false;

                }
                if (RandomBring == 5) {
                    Bring[0] = true;
                    BringAOpject = false;

                }
                if (RandomBring == 6) {
                    Bring[6] = true ;
                    BringAOpject = true ;
                }
            }

            if (Bring[0]) {
                for (int i = 0; i < linesCounter; i++) {
                    for (int j = 0; j < eachLineLength; j++) {
                        if (grid[i][j] == 1) {
                            coinsArRect[i][j].x -= BgSpeed;
                            if (coinsArRect[i][j].x <= 200)
                                BringAOpject = true;
                        }
                    }
                }
            }
            if (Bring[1]) {
                MissleRect.x -= 3 * BgSpeed;
                if (MissleRect.x <= 500)
                    BringAOpject = true;

            }
            if (Bring[2]) {
                ZapperHRect.x -= BgSpeed;
                if (ZapperHRect.x <= 500)
                    BringAOpject = true;
            }
            if (Bring[3]) {
                ZapperH2Rect.x -= BgSpeed;
                if (ZapperH2Rect.x <= 500)
                    BringAOpject = true;
            }
            if (Bring[4]) {
                ZapperVRect.x -= BgSpeed;
                if (ZapperVRect.x <= 500)
                    BringAOpject = true;
            }
            if (Bring[5]) {
                ZapperV2Rect.x -= BgSpeed;
                if (ZapperV2Rect.x <= 500)
                    BringAOpject = true;
            }
            if (Bring[6]) {
                LazerRect.x -= BgSpeed ;
                if (LazerRect.x <=100)
                    BringAOpject = true ;
            }

            if (abs(LazerRect.x - ZapperHRect.x) <= 300 || abs(LazerRect.x - ZapperH2Rect.x) <= 300 || abs(LazerRect.x - ZapperVRect.x) <= 300 || abs(LazerRect.x - ZapperV2Rect.x) <= 300) {
                LazerRect.x = (rand() % 5 ) * 1100 + 1100 ;
                Lazer.CurrentIndex = 0 ;
            }


            if (MissleRect.x <= -200) {
                MissleRect.x = 2048 + rand() % 2048 + (rand() % 10 + 10) * 1100 + 1100;
                MissleRect.y = rand() % 300 + 10;
                isHitten = false ;
            }

            if (ZapperHRect.x < -200) {
                ZapperHRect.x = 1100 + (rand() % 2) * 1100  ;
                ZapperHRect.y = rand() % 300 + 10;
                isHitten = false ;

            }

            if (ZapperH2Rect.x <= -200 || abs(ZapperHRect.x - ZapperH2Rect.x) <= 300) {
                ZapperH2Rect.x = rand() % 2048 + (rand() % 3) * 1100  ;
                ZapperH2Rect.y = rand() % 300 + 10;
                isHitten = false ;

            }


            if (ZapperVRect.x <= -200 || abs(ZapperVRect.x - ZapperHRect.x) <= 400 ||
                abs(ZapperVRect.x - ZapperH2Rect.x) <= 400) {
                ZapperVRect.x = 1100 + (rand() % 3) * 1100 ;
                ZapperVRect.y = rand() % 300 + 10;
                isHitten = false ;

            }

            if (ZapperV2Rect.x <= -200 || abs(ZapperVRect.x - ZapperV2Rect.x) <= 300 ||
                abs(ZapperV2Rect.x - ZapperHRect.x) <= 400 || abs(ZapperV2Rect.x - ZapperH2Rect.x) <= 400) {
                ZapperV2Rect.x = rand() % 2048 + (rand() % 4) * 1100 + 1100;
                ZapperV2Rect.y = rand() % 300 + 10;
                isHitten = false ;

            }

            if (LazerRect.x <= -500) {
                LazerRect.x  = rand() % 2048 + (rand() % 4) * 1100 + 1100 ;
                LazerRect.y= rand() % 300 + 10;
                Lazer.CurrentIndex = 0 ;
                isHitten = false ;

            }


            if (MissleRect.x <= -200) {
                Bring[1] = false;
            }
            if (ZapperHRect.x <= -200) {
                Bring[2] = false;
            }
            if (ZapperH2Rect.x <= -200) {
                Bring[3] = false;
            }
            if (ZapperVRect.x <= -200) {
                Bring[4] = false;
            }
            if (ZapperV2Rect.x <= -200) {
                Bring[5] = false;
            }
            if (LazerRect.x <=-600) {
                Bring[6] = false ;
            }



            // Moving Background
            SectorStartRect.x -= BgSpeed;
            Sector1Rect.x -=BgSpeed ;
            Sector2Rect.x -=BgSpeed ;
            LabStartRect.x -= BgSpeed;
            Lab1Rect.x -=BgSpeed ;
            Lab2Rect.x -=BgSpeed ;
            VolcanoStartRect.x -= BgSpeed;
            Volcano1Rect.x -=BgSpeed ;
            Volcano2Rect.x -=BgSpeed ;
            WareHouseStartRect.x -= BgSpeed;
            WareHouse1Rect.x -=BgSpeed ;

            if (SectorStartRect.x <= -2048) {
                SectorStartRect.x = 10*2048 ;
            }
            if (Sector1Rect.x <= -2048) {
                Sector1Rect.x = 10*2048;
            }
            if (Sector2Rect.x <= -2048) {
                Sector2Rect.x = 10*2048;
            }

            if (LabStartRect.x <= -2048) {
                LabStartRect.x = 10*2048 ;
            }
            if (Lab1Rect.x <= -2048) {
                Lab1Rect.x = 10*2048;
            }
            if (Lab2Rect.x <= -2048) {
                Lab2Rect.x = 10*2048;
            }



            if (VolcanoStartRect.x <= -2048) {
                VolcanoStartRect.x = 10*2048 ;
            }
            if (Volcano1Rect.x <= -2048) {
                Volcano1Rect.x = 10*2048;
            }
            if (Volcano2Rect.x <= -2048) {
                Volcano2Rect.x = 10*2048;
            }



            if (WareHouseStartRect.x <= -2048) {
                WareHouseStartRect.x = 10*2048 ;
            }
            if (WareHouse1Rect.x <= -2048) {
                WareHouse1Rect.x = 10*2048;
            }


        }

        if (GameStat == Menu) {
            SBDL::showTexture(Pallete , 800 , 0) ;
            SBDL::showTexture(MenuBg , 0 , 0) ;
            SBDL::showTexture(PlayBtn , PlayBtnRect) ;
            SBDL::showTexture(SettingBtn , SettingBtnRect) ;
            SBDL::showTexture(QuitBtn , QuitBtnRect) ;

            if (SBDL::Mouse.clicked()) {
                if (SBDL::mouseInRect(SettingBtnRect)) {
                    GameStat = Setting ;
                }
                if (SBDL::mouseInRect(PlayBtnRect)) {
                    GameStat = Game ;
                }
                if (SBDL::mouseInRect(QuitBtnRect)) {
                    return  0 ;
                }

            }



            if (PlayerStatus == Barry)
                SBDL::showTexture(MovingBarry.images[6], {180,360,60, 72});
            if (PlayerStatus == Alien)
                SBDL::showTexture(MovingAlien.images[6], {180,360,60,72});

        }

        if (GameStat == Lose) {


            if (Distance > Best)
            {
                Best = Distance;
                ofstream myfile;
                myfile.open ("Best.txt");
                myfile << Best << "\n";
                myfile.close();
            }



            SBDL::showTexture(Pallete , {0 , 0 ,1100,461}) ;
            SBDL::showTexture(MenuBg , 0 , 0) ;

            BestGameOver_Text = SBDL::createFontTexture(DistanceFont,"Best:  " + to_string(Best), 255, 255, 255);
            DistanceGameOver_Text = SBDL::createFontTexture(DistanceFont , "Distance: " + to_string(Distance) , 255 ,255 ,255) ;
            CoinGameOver_Text = SBDL::createFontTexture(DistanceFont , "Coins: " + to_string(CoinScore) , 255 ,255 ,255) ;
            SBDL::showTexture(CoinGameOver_Text ,900 , 250  ) ;
            SBDL::showTexture(DistanceGameOver_Text ,900 , 300  ) ;
            SBDL::showTexture(BestGameOver_Text ,900 , 350  ) ;

            SBDL::showTexture(RetryBtn , RetryBtnRect ) ;
            SBDL::showTexture(QuitBtn , QuitBtnRect) ;
            SBDL::showTexture(BuyResumeBtn , BuyResumeBtnRect) ;
            if (SBDL::Mouse.clicked()) {

                if (SBDL::mouseInRect(BuyResumeBtnRect)) {
                    if (CoinScore >=500 ) {
                        CoinScore = CoinScore - 500 ;
                        isHitten = true ;
                        GameStat = Game ;
                    }
                    else {
                        SBDL::showTexture(NotEnoughCoin_Text , 880 , 200) ;
                    }
                }

                if (SBDL::mouseInRect(QuitBtnRect)) {
                    GameStat = Menu ;
                    SectorStartRect = {0, 0, SectorStart.width+30, SectorStart.height};
                    Sector1Rect = {2048, 0, Sector1.width + 30, Sector1.height};
                    Sector2Rect = {4096, 0, Sector2.width + 30, Sector2.height};
                    LabStartRect = {3*2048, 0, SectorStart.width+30, SectorStart.height};
                    Lab1Rect = {4*2048, 0, Sector1.width + 30, Sector1.height};
                    Lab2Rect = {5*2048, 0, Sector2.width + 30, Sector2.height};
                    VolcanoStartRect = {6*2048, 0, SectorStart.width+30, SectorStart.height};
                    Volcano1Rect = {7*2048, 0, Sector1.width + 30, Sector1.height};
                    Volcano2Rect = {8*2048, 0, Sector2.width + 30, Sector2.height};
                    WareHouseStartRect = {9*2048, 0, SectorStart.width+30, SectorStart.height};
                    WareHouse1Rect = {10*2048, 0, Sector1.width + 30, Sector1.height};
                    BarryRect = {180, 360, 60, 72};
                    ScientistRect[0] = {rand() % 1100 + 1100, 380, 30, 50};
                    ScientistRect[1] = {rand() % 1100 + 1100, 380, 50, 50};
                    ScientistRect[2] = {rand() % 1100 + 1100, 380, 30, 50};
                    ScientistRect[3] = {rand() % 1100 + 1100, 380, 30, 50};
                    ZapperHRect = {-500, 10 + rand() % 300, 150, 70};
                    ZapperVRect = {-500, 10 + rand() % 200, 70, 150};


                    MissleRect = {3000, 200, Missle.images[0].width, Missle.images[0].height};
                    LazerRect = {-800 , 10 + rand() % 300 , 350 , 30} ;
                    GravityTokenRect = { (rand() % 5 + 1)*1100 + 1100   , 10 + rand() % 300 , 100  , 100};
                    SpeedTokenRect = {-500, 0 , 60, 60 } ;


                    // Create a 2D array Rect for each Coin
                    RandomXD = rand() % 2400;
                    RandomYD = rand() % 300 + 10;
                    for (int i = 0; i < linesCounter; i++) {
                        for (int j = 0; j < eachLineLength; j++) {
                            if (grid[i][j] == 1) {
                                coinsArRect[i][j] = {
                                        30 * j + 60 + RandomXD - coinsAr[i][j].images[coinsAr[i][j].currentIndex].width / 2 + 1300,
                                        30 * i + RandomYD, 30, 30};
                            } else
                                coinsArRect[i][j] = {-5000, -5000, 0, 0};
                        }
                    }


                    // Variables
                    Speed = 0;
                    BgSpeed = 10;
                    CoinScore = 0;
                    Distance = 0;
                    counter = 0;
                    for (int i=0 ; i<4 ; i++)
                        fear[i] = false;
                    CoinNeed = true;
                    RandomNeed = false;
                    BringAOpject = true;
                    for (int i=0 ; i< 7 ; i++)
                        Bring[i] = false ;
                    GravitySuitBool = false;
                    SpeedToken = false;
                    isHitten = false ;
                    GravityPlace  = 0;
                    TokenCounter = 0 ;
                    Yspeed = 5 ;

                }

                if (SBDL::mouseInRect(RetryBtnRect)) {
                    SectorStartRect = {0, 0, SectorStart.width+30, SectorStart.height};
                    Sector1Rect = {2048, 0, Sector1.width + 30, Sector1.height};
                    Sector2Rect = {4096, 0, Sector2.width + 30, Sector2.height};
                    LabStartRect = {3*2048, 0, SectorStart.width+30, SectorStart.height};
                    Lab1Rect = {4*2048, 0, Sector1.width + 30, Sector1.height};
                    Lab2Rect = {5*2048, 0, Sector2.width + 30, Sector2.height};
                    VolcanoStartRect = {6*2048, 0, SectorStart.width+30, SectorStart.height};
                    Volcano1Rect = {7*2048, 0, Sector1.width + 30, Sector1.height};
                    Volcano2Rect = {8*2048, 0, Sector2.width + 30, Sector2.height};
                    WareHouseStartRect = {9*2048, 0, SectorStart.width+30, SectorStart.height};
                    WareHouse1Rect = {10*2048, 0, Sector1.width + 30, Sector1.height};
                    BarryRect = {180, 360, 60, 72};
                    ScientistRect[0] = {rand() % 1100 + 1100, 380, 30, 50};
                    ScientistRect[1] = {rand() % 1100 + 1100, 380, 50, 50};
                    ScientistRect[2] = {rand() % 1100 + 1100, 380, 30, 50};
                    ScientistRect[3] = {rand() % 1100 + 1100, 380, 30, 50};
                    ZapperHRect = {-500, 10 + rand() % 300, 150, 70};
                    ZapperVRect = {-500, 10 + rand() % 200, 70, 150};


                    MissleRect = {3000, 200, Missle.images[0].width, Missle.images[0].height};
                    LazerRect = {-800 , 10 + rand() % 300 , 350 , 30} ;
                    GravityTokenRect = { (rand() % 5 + 1)*1100 + 1100   , 10 + rand() % 300 , 100  , 100};
                    SpeedTokenRect = {-500, 0 , 60, 60 } ;


                    // Create a 2D array Rect for each Coin
                    RandomXD = rand() % 2400;
                    RandomYD = rand() % 300 + 10;
                    for (int i = 0; i < linesCounter; i++) {
                        for (int j = 0; j < eachLineLength; j++) {
                            if (grid[i][j] == 1) {
                                coinsArRect[i][j] = {
                                        30 * j + 60 + RandomXD - coinsAr[i][j].images[coinsAr[i][j].currentIndex].width / 2 + 1300,
                                        30 * i + RandomYD, 30, 30};
                            } else
                                coinsArRect[i][j] = {-5000, -5000, 0, 0};
                        }
                    }


                    // Variables
                    Speed = 0;
                    BgSpeed = 10;
                    CoinScore = 0;
                    Distance = 0;
                    counter = 0;
                    for (int i=0 ; i<4 ; i++)
                        fear[i] = false;
                    CoinNeed = true;
                    RandomNeed = false;
                    BringAOpject = true;
                    for (int i=0 ; i< 7 ; i++)
                        Bring[i] = false ;
                    GravitySuitBool = false;
                    SpeedToken = false;
                    isHitten = false ;
                    GravityPlace  = 0;
                    TokenCounter = 0 ;
                    Yspeed = 5 ;
                    GameStat = Game ;
                }
            }


        }
        if (GameStat == Pause) {
            SBDL::showTexture(Pallete , {0 , 0 ,1100,461}) ;
            SBDL::showTexture(PauseBg , {100, 60, 600 , 300 }) ;
            SBDL::showTexture(ResumeBtn , ResumeBtnRect ) ;
            SBDL::showTexture(QuitBtn , QuitBtnRect) ;
            if (SBDL::Mouse.clicked()) {
                if (SBDL::mouseInRect(ResumeBtnRect))
                    GameStat = Game ;
                if (SBDL::mouseInRect(QuitBtnRect)) {

                    SectorStartRect = {0, 0, SectorStart.width+30, SectorStart.height};
                    Sector1Rect = {2048, 0, Sector1.width + 30, Sector1.height};
                    Sector2Rect = {4096, 0, Sector2.width + 30, Sector2.height};
                    LabStartRect = {3*2048, 0, SectorStart.width+30, SectorStart.height};
                    Lab1Rect = {4*2048, 0, Sector1.width + 30, Sector1.height};
                    Lab2Rect = {5*2048, 0, Sector2.width + 30, Sector2.height};
                    VolcanoStartRect = {6*2048, 0, SectorStart.width+30, SectorStart.height};
                    Volcano1Rect = {7*2048, 0, Sector1.width + 30, Sector1.height};
                    Volcano2Rect = {8*2048, 0, Sector2.width + 30, Sector2.height};
                    WareHouseStartRect = {9*2048, 0, SectorStart.width+30, SectorStart.height};
                    WareHouse1Rect = {10*2048, 0, Sector1.width + 30, Sector1.height};
                    BarryRect = {180, 360, 60, 72};
                    ScientistRect[0] = {rand() % 1100 + 1100, 380, 30, 50};
                    ScientistRect[1] = {rand() % 1100 + 1100, 380, 50, 50};
                    ScientistRect[2] = {rand() % 1100 + 1100, 380, 30, 50};
                    ScientistRect[3] = {rand() % 1100 + 1100, 380, 30, 50};
                    ZapperHRect = {-500, 10 + rand() % 300, 150, 70};
                    ZapperVRect = {-500, 10 + rand() % 200, 70, 150};


                    MissleRect = {3000, 200, Missle.images[0].width, Missle.images[0].height};
                    LazerRect = {-800 , 10 + rand() % 300 , 350 , 30} ;
                    GravityTokenRect = { (rand() % 5 + 1)*1100 + 1100   , 10 + rand() % 300 , 100  , 100};
                    SpeedTokenRect = {-500, 0 , 60, 60 } ;


                    // Create a 2D array Rect for each Coin
                    RandomXD = rand() % 2400;
                    RandomYD = rand() % 300 + 10;
                    for (int i = 0; i < linesCounter; i++) {
                        for (int j = 0; j < eachLineLength; j++) {
                            if (grid[i][j] == 1) {
                                coinsArRect[i][j] = {
                                        30 * j + 60 + RandomXD - coinsAr[i][j].images[coinsAr[i][j].currentIndex].width / 2 + 1300,
                                        30 * i + RandomYD, 30, 30};
                            } else
                                coinsArRect[i][j] = {-5000, -5000, 0, 0};
                        }
                    }


                    // Variables
                    Speed = 0;
                    BgSpeed = 10;
                    CoinScore = 0;
                    Distance = 0;
                    counter = 0;
                    for (int i=0 ; i<4 ; i++)
                        fear[i] = false;
                    CoinNeed = true;
                    RandomNeed = false;
                    BringAOpject = true;
                    for (int i=0 ; i< 7 ; i++)
                        Bring[i] = false ;
                    GravitySuitBool = false;
                    SpeedToken = false;
                    isHitten = false ;
                    GravityPlace  = 0;
                    TokenCounter = 0 ;
                    Yspeed = 5 ;

                    GameStat = Menu ;


                }


            }


            if (SBDL::keyPressed(SDL_SCANCODE_SPACE))
                GameStat = Game;
        }
        if( GameStat == Setting) {
            SBDL::showTexture(Pallete , 800 , 0) ;

            SBDL::showTexture(MenuBg , 0 , 0) ;

            if (SoundStatus && MusicStatus)
                SBDL::showTexture(SoundOffBtn , SoundBtnRect) ;
            if (!SoundStatus && !MusicStatus)
                SBDL::showTexture(SoundBtn , SoundBtnRect) ;
            SBDL::showTexture(BackBtn , BackBtnRect ) ;
            SBDL::showTexture(ChangeBtn , ChangeBtnRect) ;

            if (PlayerStatus == Barry)
                SBDL::showTexture(MovingBarry.images[6], {900 , 30 , 100 ,120});
            if (PlayerStatus == Alien)
                SBDL::showTexture(MovingAlien.images[6], {900 , 30 ,100 ,120});

            if (SBDL::Mouse.clicked()) {
                if (SBDL::mouseInRect(SoundBtnRect)) {
                    MusicStatus = !MusicStatus ;
                    SoundStatus = !SoundStatus ;
                    isMusicPlay = true ;
                    if (SoundStatus && MusicStatus)
                        SBDL::playSound(SoundOnEffect , 1) ;
                    if (!SoundStatus && !MusicStatus)
                        SBDL::stopMusic();
                }

                if (SBDL::mouseInRect(ChangeBtnRect)) {
                    SBDL::playSound(ChangeBtnEffect , 1) ;
                    if (PlayerStatus == Barry)
                        PlayerStatus = Alien ;
                    else if (PlayerStatus == Alien)
                        PlayerStatus = Barry ;
                }


                if (SBDL::mouseInRect(BackBtnRect)) {
                    GameStat = Menu ;
                }
            }


            }


        SBDL::updateRenderScreen();
        int elapsedTime = SBDL::getTime() - startTime;
        if (elapsedTime < delay)
            SBDL::delay(delay - elapsedTime);
    }


    return 0;
}
