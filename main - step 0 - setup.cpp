// "How to Generate Shockingly good 2D Lightning Effects" - courtesy of Michael Hoffman (thanks!)
// Url:      https://gamedevelopment.tutsplus.com/tutorials/how-to-generate-shockingly-good-2d-lightning-effects--gamedev-2681
//
// olc::PixelGameEngine - Courtesy of Javidx9 (thanks again!)
// Url: https://github.com/OneLoneCoder/olcPixelGameEngine
//
// Implementation on PGE (= PixelGameEngine): Joseph21, march 23, 2023
//
// This is just the setup of the PGE skeleton code - needed for useful comparison
//

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class LightningEffects : public olc::PixelGameEngine {

public:
    LightningEffects() {
        sAppName = "2D lightning effects";
    }

private:
    // put your class variables here

public:
    bool OnUserCreate() override {

        // initialize your assets here

        return true;
    }

    bool OnUserUpdate( float fElapsedTime ) override {

        // your update code per frame here

        return true;
    }

    bool OnUserDestroy() override {

        // your clean up code here

        return true;
    }
};

// keep the screen dimensions constant and vary the resolution by adapting the pixel size
#define SCREEN_X   1200
#define SCREEN_Y    800
#define PIXEL_X       1
#define PIXEL_Y       1

int main()
{
	LightningEffects demo;
	if (demo.Construct( SCREEN_X / PIXEL_X, SCREEN_Y / PIXEL_Y, PIXEL_X, PIXEL_Y ))
		demo.Start();

	return 0;
}

