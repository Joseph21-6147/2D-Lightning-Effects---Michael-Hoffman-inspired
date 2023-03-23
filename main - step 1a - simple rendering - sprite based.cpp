// "How to Generate Shockingly good 2D Lightning Effects" - courtesy of Michael Hoffman (thanks!)
// Url:      https://gamedevelopment.tutsplus.com/tutorials/how-to-generate-shockingly-good-2d-lightning-effects--gamedev-2681
//
// olc::PixelGameEngine - Courtesy of Javidx9 (thanks again!)
// Url: https://github.com/OneLoneCoder/olcPixelGameEngine
//
// Implementation on PGE (= PixelGameEngine): Joseph21, march 23, 2023
//
// This is the implementation upto and including: Step 1a - simple sprite based rendering
//

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define LINE_SEGMENT_FILE "Line Segment Example.png"    // file contains the base image for the lightning bolt

class LightningEffects : public olc::PixelGameEngine {

public:
    LightningEffects() {
        sAppName = "2D lightning effects";
    }

private:
    // put your class variables here
    olc::Sprite *pSegmentSpr = nullptr, *pCapSpr = nullptr, *pMidSpr = nullptr;

    // for the rotation animation
    float fCumulatedTime = 0.0f;

public:
    bool OnUserCreate() override {

        // cut the input image into a cap part and a mid part
        pSegmentSpr = new olc::Sprite( LINE_SEGMENT_FILE );
        pCapSpr = pSegmentSpr->Duplicate( olc::vi2d(   0, 0 ), olc::vi2d( 64, 128 ));
        pMidSpr = pSegmentSpr->Duplicate( olc::vi2d( 127, 0 ), olc::vi2d(  1, 128 ));

        return true;
    }

    bool OnUserUpdate( float fElapsedTime ) override {

        // your update code per frame here
        fCumulatedTime += fElapsedTime;
        int nMilliSecs = int( float( fCumulatedTime - int( fCumulatedTime )) * 1000.0f);


        // start rendering to screen
        Clear( olc::BLACK );

        // display regular image
        DrawSprite( 10, 10, pSegmentSpr );

        // display image using cap and mid parts of the sprite
        if (nMilliSecs >= pCapSpr->width) {
            DrawSprite( 10             , 200, pCapSpr, 1, olc::Sprite::NONE  );
            DrawSprite( 10 + nMilliSecs, 200, pCapSpr, 1, olc::Sprite::HORIZ );
            for (int i = 10 + pCapSpr->width; i < 10 + nMilliSecs; i++) {
                DrawSprite( i, 200, pMidSpr, 1, olc::Sprite::NONE );
            }
        }

        return true;
    }

    bool OnUserDestroy() override {

        // your clean up code here
        delete pSegmentSpr;
        delete pCapSpr;
        delete pMidSpr;

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

