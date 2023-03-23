// "How to Generate Shockingly good 2D Lightning Effects" - courtesy of Michael Hoffman (thanks!)
// Url: https://gamedevelopment.tutsplus.com/tutorials/how-to-generate-shockingly-good-2d-lightning-effects--gamedev-2681
//
// olc::PixelGameEngine - Courtesy of Javidx9 (thanks again!)
// Url: https://github.com/OneLoneCoder/olcPixelGameEngine
//
// Implementation on PGE (= PixelGameEngine): Joseph21, march 23, 2023
//
// This is the implementation upto and including: Step 2 - Jagged Lines
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
    olc::Decal                          *pCapDec = nullptr, *pMidDec = nullptr;

    std::vector<Line> vBolt;
    olc::vf2d src, dst;

protected:


    // creates and retursn a std::vector of Lines with specific jaggedness and displacement
    std::vector<Line> CreateBolt( olc::vf2d source, olc::vf2d dest, float thickness = 1.0f ) {
        std::vector<Line> results;
        olc::vf2d tangent = dest - source;
        olc::vf2d normal = olc::vf2d( tangent.y, -tangent.x ).norm();
        float length = tangent.mag();

        std::vector<float> positions;
        positions.push_back( 0.0f );

        for (int i = 0; i < length / 4; i++) {
            positions.push_back( (rand() % 10000) / 10000.0f );
        }

        std::sort( positions.begin(), positions.end() );

        float fSway = 80.0f;
        float fJaggedness = 1.0f / fSway;

        olc::vf2d prevPoint = source;
        float fPrevDisplacement = 0.0f;
        for (int i = 1; i < (int)positions.size(); i++) {
            float pos = positions[i];

            // used to prevent sharp angles by ensuring very close positions also have small perpendicular variation.
            float scale = (length * fJaggedness) * (pos - positions[i - 1]);

            // defines an envelope. Points near the middle of the bolt can be further from the central line.
            float envelope = pos > 0.95f ? 20 * (1 - pos) : 1;

            float displacement = float( rand() % int(fSway * 2 + 1)) - fSway;
            displacement -= (displacement - fPrevDisplacement) * (1 - scale);
            displacement *= envelope;

            olc::vf2d point = source + pos * tangent + displacement * normal;

            results.push_back( Line( prevPoint, point, thickness ));
            prevPoint = point;
            fPrevDisplacement = displacement;
        }
        results.push_back( Line( prevPoint, dest, thickness ));

        return results;
    }

public:

    bool OnUserCreate() override {

        // cut the input image into a cap part and a mid part, and create decals out of these parts
        pSegmentSpr = new olc::Sprite( LINE_SEGMENT_FILE );
        pCapSpr = pSegmentSpr->Duplicate( olc::vi2d(   0, 0 ), olc::vi2d( 64, 128 ));
        pMidSpr = pSegmentSpr->Duplicate( olc::vi2d( 127, 0 ), olc::vi2d(  1, 128 ));
        pCapDec = new olc::Decal( pCapSpr );
        pMidDec = new olc::Decal( pMidSpr );
        // choose initial coordinates for lightning bolt
        src = { ScreenWidth() * 0.25f, ScreenHeight() * 0.25f };
        dst = { ScreenWidth() * 0.75f, ScreenHeight() * 0.75f };

        vBolt = CreateBolt( src, dst );

        return true;
    }

    bool OnUserUpdate( float fElapsedTime ) override {

        // your update code per frame here

        if (GetMouse( 0 ).bPressed) { src = GetMousePos(); vBolt.clear(); vBolt = CreateBolt( src, dst ); }
        if (GetMouse( 1 ).bPressed) { dst = GetMousePos(); vBolt.clear(); vBolt = CreateBolt( src, dst ); }

        // display to screen
        Clear( olc::BLANK );
        SetDecalMode( olc::DecalMode::ADDITIVE );

        for (auto segment : vBolt) {
            segment.Draw( this, pCapDec, pMidDec, olc::WHITE );
        }

        return true;
    }

    bool OnUserDestroy() override {

        // your clean up code here
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

