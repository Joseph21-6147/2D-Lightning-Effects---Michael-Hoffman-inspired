// "How to Generate Shockingly good 2D Lightning Effects" - courtesy of Michael Hoffman (thanks!)
// Url:      https://gamedevelopment.tutsplus.com/tutorials/how-to-generate-shockingly-good-2d-lightning-effects--gamedev-2681
//
// olc::PixelGameEngine - Courtesy of Javidx9 (thanks again!)
// Url: https://github.com/OneLoneCoder/olcPixelGameEngine
//
// Implementation on PGE (= PixelGameEngine): Joseph21, march 23, 2023
//
// This is the implementation upto and including: Step 1b - simple decal based rendering
//

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define LINE_SEGMENT_FILE "Line Segment Example.png"    // file contains the base image for the lightning bolt

#define PI 3.1415926535f

class Line {
public:
    olc::vf2d A, B;
    float fThickness;

    Line() {}
    Line( olc::vf2d a, olc::vf2d b, float thickness = 1.0f ) {
        A = a;
        B = b;
        fThickness = thickness;
    }

    void Draw( olc::PixelGameEngine *pPGE, olc::Decal *pCap, olc::Decal *pMid, olc::Pixel color ) {
        // determine tangent line and rotation from start and end points of line
        olc::vf2d tangent = B - A;
        float rotation = atan2( tangent.y, tangent.x );

        // set to thickness of the line in pixels (without the glow)
        float fImageThickness = 8;
        float fThicknessScale = fThickness / fImageThickness;
        olc::vf2d thicknessScale = olc::vf2d( fThicknessScale, fThicknessScale );
        // determine origin coords of cap and mid part of line
        olc::vf2d capOrigin = olc::vf2d( pCap->sprite->width, pCap->sprite->height / 2.0f );
        olc::vf2d midOrigin = olc::vf2d(                   0, pMid->sprite->height / 2.0f );
        // determine scale of midpart - needed to stretch it to required length
        olc::vf2d midScale  = olc::vf2d( tangent.mag(), fThicknessScale );
        // draw line: stretched mid part and bot cap part (one additionally rotated 180 degrees)
        pPGE->DrawRotatedDecal( A, pMid, rotation     , midOrigin, midScale, color );
        pPGE->DrawRotatedDecal( A, pCap, rotation     , capOrigin, thicknessScale, color );
        pPGE->DrawRotatedDecal( B, pCap, rotation + PI, capOrigin, thicknessScale, color );
    }
};

class LightningEffects : public olc::PixelGameEngine {

public:
    LightningEffects() {
        sAppName = "2D lightning effects";
    }

private:
    // put your class variables here
    olc::Sprite *pSegmentSpr = nullptr, *pCapSpr = nullptr, *pMidSpr = nullptr;
    olc::Decal  *pSegmentDec = nullptr, *pCapDec = nullptr, *pMidDec = nullptr;

    // for the rotation animation
    float fCumulatedTime = 0.0f;

public:
    bool OnUserCreate() override {

        // cut the input image into a cap part and a mid part, and create decals out of these parts
        pSegmentSpr = new olc::Sprite( LINE_SEGMENT_FILE );
        pCapSpr = pSegmentSpr->Duplicate( olc::vi2d(   0, 0 ), olc::vi2d( 64, 128 ));
        pMidSpr = pSegmentSpr->Duplicate( olc::vi2d( 127, 0 ), olc::vi2d(  1, 128 ));
        pSegmentDec = new olc::Decal( pSegmentSpr );
        pCapDec = new olc::Decal( pCapSpr );
        pMidDec = new olc::Decal( pMidSpr );

        return true;
    }

    bool OnUserUpdate( float fElapsedTime ) override {

        // your update code per frame here
        fCumulatedTime += fElapsedTime;
        int nMilliSecs = int( float( fCumulatedTime - int( fCumulatedTime )) * 1000.0f);

        // start rendering to screen
        Clear( olc::BLANK );
        SetDecalMode( olc::DecalMode::ADDITIVE );

        // display regular image
        DrawDecal( olc::vf2d( 10.0f, 10.0f ), pSegmentDec );

        // display image using cap and mid parts of the sprite
        if (nMilliSecs >= pCapSpr->width) {
            DrawRotatedDecal( olc::vf2d( 10.0f, 200.0f ), pCapDec, 0.0f, olc::vf2d( pCapSpr->width / 2.0f, pCapSpr->height / 2.0f ));
            DrawRotatedDecal( olc::vf2d( 10.0f + nMilliSecs, 200.0f ), pCapDec, PI, olc::vf2d( pCapSpr->width / 2.0f, pCapSpr->height / 2.0f ));
            for (int i = 10 + pCapSpr->width; i < 10 + nMilliSecs; i++) {
                DrawRotatedDecal( olc::vf2d( i, 200.0f ), pMidDec, 0.0f, olc::vf2d( pCapSpr->width / 2.0f, pCapSpr->height / 2.0f ));
            }
        }

        return true;
    }

    bool OnUserDestroy() override {

        // your clean up code here
        delete pSegmentDec;
        delete pCapDec;
        delete pMidDec;
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

