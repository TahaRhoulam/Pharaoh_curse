// scene.cpp - "The Pharaoh's Curse" - DRONE CAMERA
// Story: An explorer steals a sacred treasure -> the ancient Pharaoh awakens
// -> a Mummy Warrior rises from the sand -> both chase the explorer inside
// the pyramid.
//
// Camera style and phase-based structure inspired by a friend's "Hot Pizza
// Delivery" project, fully re-themed and re-written for the pyramid setting.

#include "prefix.h"
#include "constants.h"
#include "utility.h"
#include "light.h"
#include "camera.h"
#include "texture.h"
#include "scene.h"
#include "snd.h"
#include "skybox.h"
#include "fog.h"
#include "viewfrustum.h"
#include "terrain.h"
#include "DesertPalms.h"
#include "cube.h"
#include "sphere.h"
#include "composite2.h"
#include "composite1.h"
#include "TombGuardianStatue.h"
#include "Pharaoh.h"
#include "PyramidStructure.h"

static const float SC_PI = 3.14159265358979f;

extern ProgramMode       gProgramMode;
extern cameraClass       gCamera;
extern object3d         *gSky;
extern fog              *gFog;
extern viewfrustum      *gViewFrustum;
extern float             gCurrentFrameTime;
extern vector<object3d*> gShapeVector;

static composite2*         gExplorer  = nullptr;   // the thief
static composite1*         gMummy     = nullptr;   // mummy warrior
static Pharaoh*            gPharaoh   = nullptr;   // awakened ruler
static TombGuardianStatue* gGuardian  = nullptr;   // curse-state statue
static sphere*             gIdolGlow  = nullptr;   // free-standing treasure on its plinth

static terrain* theGround = nullptr;
static texture* groundTex = nullptr;
static cube*    plinth    = nullptr;          // stone pedestal that originally held the idol
static cube*    idol      = nullptr;          // the sacred golden treasure itself
static std::vector<cube*> sandDunesDeco;       // small decorative sand mounds
static DesertPalms*  palms     = nullptr;
static light*   ambient   = nullptr;
static light*   sunLight  = nullptr;
static light*   fillLight = nullptr;
static snd*     ambienceSound = nullptr;       // looping dark pyramid night ambience

static bool treasureStolen   = false;
static bool mummyRisen       = false;
static bool pharaohAwake     = false;

static const float PLINTH_X    =  12.0f;
static const float PLINTH_Z    =  44.0f;
static const float SARCO_X     =  18.0f;
static const float SARCO_Z     =  40.0f;
static const float MUMMY_BURY_X = -6.0f;
static const float MUMMY_BURY_Z = 30.0f;
static const float PYRAMID_X   =   0.0f;
static const float PYRAMID_Z   = -22.0f;

static inline float lerpf(float a,float b,float t){
    if(t<0)t=0; if(t>1)t=1; return a+(b-a)*t;
}

void constructScene()
{
    ambient   = new light(GL_LIGHT_MODEL_AMBIENT);
    sunLight  = new light(GL_LIGHT0);
    fillLight = new light(GL_LIGHT1);

    ambienceSound = new snd("pyramid_ambience.ogg");   // dark pyramid night ambience loop

    theGround = new terrain("ground.bmp");
    groundTex = new texture("sand_desert.jpg");      // clean, uniform desert sand texture
    theGround->setTexture(groundTex);
    theGround->setColour(0.92f, 0.84f, 0.62f);       // natural golden sand tint
    theGround->setPosition(0,-2.5f,0);
    theGround->setDeformation(2,1,2);
    theGround->setScale(2);

    gSky = new skybox("sky22-");

    // ── small decorative sand dunes scattered around the plaza, kept clear
    //    of the explorer's running corridor (X: -2..14) so nothing blocks him ─
    for(int i=0;i<10;i++){
        cube* dune = new cube();
        dune->setColour(0.88f, 0.78f, 0.54f);
        dune->setDeformation(randomFloat(1.5f,3.0f), randomFloat(0.3f,0.7f), randomFloat(1.5f,3.0f));
        float dx, dz;
        do {
            dx = randomFloat(-24,24);
            dz = randomFloat(-20,46);
        } while (dx > -3.0f && dx < 15.0f);     // avoid the explorer's path corridor
        dune->setPosition(dx, -0.4f, dz);
        sandDunesDeco.push_back(dune);
    }

    palms = new DesertPalms(12);
    palms->setPosition(-22.0f,0.0f,10.0f);

    // ── Pyramids far from the action, plus the BIG destination pyramid ───
    PyramidStructure* smallA = new PyramidStructure(0.80f,0.74f,0.55f,1.2f);
    smallA->setPosition(-20.0f,0.0f,16.0f);
    PyramidStructure* smallB = new PyramidStructure(0.78f,0.70f,0.50f,1.6f);
    smallB->setPosition(22.0f,0.0f,4.0f);
    PyramidStructure* smallC = new PyramidStructure(0.76f,0.68f,0.48f,1.0f);
    smallC->setPosition(-18.0f,0.0f,-10.0f);

    PyramidStructure* greatPyramid = new PyramidStructure(0.86f,0.78f,0.56f,3.2f);
    greatPyramid->setPosition(PYRAMID_X,0.0f,PYRAMID_Z);

    // Sarcophagus housing for the Pharaoh (re-uses cube primitives directly here,
    // since it's a simple prop rather than a full composite character)
    cube* sarcophagus = new cube();
    sarcophagus->setColour(0.70f,0.58f,0.16f);
    sarcophagus->setDeformation(0.9f,0.5f,2.0f);
    sarcophagus->setPosition(SARCO_X,0.25f,SARCO_Z);

    // Guardian statue stands watch near the treasure plinth
    gGuardian = new TombGuardianStatue();
    gGuardian->setPosition(PLINTH_X-4.0f,0.0f,PLINTH_Z-2.0f);
    gGuardian->setDormant();

    // Treasure plinth + the sacred golden idol itself
    plinth = new cube();
    plinth->setColour(0.55f,0.50f,0.40f);
    plinth->setDeformation(0.7f,0.5f,0.7f);
    plinth->setPosition(PLINTH_X,0.25f,PLINTH_Z);

    idol = new cube();
    idol->setColour(0.95f,0.80f,0.10f);
    idol->setEmissiveColour(0.30f,0.22f,0.0f,1.0f);
    idol->setDeformation(0.30f,0.45f,0.30f);
    idol->setPosition(PLINTH_X,0.95f,PLINTH_Z);

    gIdolGlow = new sphere(14);
    gIdolGlow->setColour(1.0f,0.9f,0.2f,0.18f);
    gIdolGlow->setDeformation(0.65f,0.65f,0.65f);
    gIdolGlow->setPosition(PLINTH_X,0.95f,PLINTH_Z);

    gExplorer = new composite2();
    gExplorer->setPosition(8.0f,0.0f,58.0f);
    gExplorer->setRotation('y',180.0f);

    gMummy = new composite1();
    gMummy->setPosition(MUMMY_BURY_X,-1.6f,MUMMY_BURY_Z);
    gMummy->hide();

    gPharaoh = new Pharaoh();
    gPharaoh->setPosition(SARCO_X,0.0f,SARCO_Z);
    gPharaoh->hide();
}

void resetScene()
{
    // Drone start: high above the treasure plaza
    gCamera.setPosition(4.0f,32.0f,70.0f);
    gCamera.setTarget(8.0f,0.0f,50.0f);

    ambient->setColour(0.42f,0.40f,0.34f,1.0f);
    sunLight->turnOn();
    sunLight->setPosition(14.0f,30.0f,30.0f);
    sunLight->setColour(1.0f,0.92f,0.70f,1.0f);
    sunLight->setSpecularColour(0.7f,0.65f,0.5f,1.0f);
    sunLight->makePositional();
    sunLight->setLinearAttenuation(0.007f);
    fillLight->turnOn();
    fillLight->setPosition(-22.0f,18.0f,-10.0f);
    fillLight->setColour(0.35f,0.32f,0.45f,1.0f);
    fillLight->makePositional();
    fillLight->setLinearAttenuation(0.014f);

    ambienceSound->setVolume(0.55f);
    ambienceSound->startSound();          // begin the looping dark pyramid ambience

    treasureStolen=false; mummyRisen=false; pharaohAwake=false;
}

void animateForNextFrame(float time, long frame)
{
    // Manually loop the dark pyramid ambience track (engine sounds don't
    // natively loop), restarting it every time its ~19.6s duration elapses.
    static float lastAmbienceRestart = 0.0f;
    if (time - lastAmbienceRestart >= 19.6f) {
        ambienceSound->restartSound();
        lastAmbienceRestart = time;
    }

    // ── DAY -> SUNSET -> NIGHT progression over the whole 78s animation ──
    float d = (time - 20.0f) / 36.0f;
    if(d < 0.0f) d = 0.0f;
    if(d > 1.0f) d = 1.0f;

    float sunR = lerpf(1.00f, 0.30f, d);
    float sunG = lerpf(0.92f, 0.18f, d);
    float sunB = lerpf(0.70f, 0.35f, d);
    float sunset = 1.0f - fabsf(d - 0.5f) * 2.0f;
    if(sunset < 0.0f) sunset = 0.0f;
    sunR += sunset * 0.30f;
    sunG += sunset * 0.10f;
    sunLight->setColour(sunR, sunG, sunB, 1.0f);

    ambient->setColour(lerpf(0.42f,0.08f,d),
                       lerpf(0.40f,0.08f,d),
                       lerpf(0.34f,0.18f,d), 1.0f);

    fillLight->setColour(lerpf(0.35f,0.05f,d),
                         lerpf(0.32f,0.06f,d),
                         lerpf(0.45f,0.16f,d), 1.0f);

    if(gSky) gSky->setColour(lerpf(1.0f,0.18f,d),
                             lerpf(1.0f,0.20f,d),
                             lerpf(1.0f,0.36f,d), 1.0f);

    // ── Phase 1 (0-6s): Drone overview - explorer approaches the treasure ─
    if(time < 6.0f){
        float t=time/6.0f;
        float exX=lerpf(8.0f,PLINTH_X+2.5f,t);
        float exZ=lerpf(58.0f,PLINTH_Z+1.0f,t);
        gExplorer->setPosition(exX,0.0f,exZ);
        gExplorer->setRotation('y',200.0f);
        gExplorer->animateRun(time);
        gExplorer->animateArmL(time, 0.0f);
        gExplorer->animateArmR(time, 0.0f);
        gCamera.setPosition(lerpf(4.0f,8.0f,t), lerpf(32.0f,26.0f,t), lerpf(70.0f,60.0f,t));
        gCamera.setTarget(8.0f,0.0f,PLINTH_Z+4.0f);
    }

    // ── Phase 2 (6-9s): Explorer studies the glowing idol ─────────────────
    if(time>=6.0f && time<9.0f){
        float t=(time-6.0f)/3.0f;
        gExplorer->setPosition(PLINTH_X+2.2f,0.0f,PLINTH_Z+0.6f);
        gExplorer->setRotation('y',225.0f);
        gExplorer->stand();
        gExplorer->animateLookBack(0.0f);
        gCamera.setPosition(lerpf(8.0f,PLINTH_X+1.0f,t), lerpf(26.0f,9.0f,t), lerpf(60.0f,PLINTH_Z+9.0f,t));
        gCamera.setTarget(PLINTH_X, 1.0f, PLINTH_Z);
    }

    // ── Phase 3 (9-12s): The theft - explorer grabs the idol ─────────────
    if(time>=9.0f && time<12.0f){
        float t=(time-9.0f)/3.0f;
        gExplorer->setPosition(PLINTH_X+2.2f,0.0f,PLINTH_Z+0.6f);
        gExplorer->setRotation('y',225.0f);
        gExplorer->grabTreasurePose();
        if(t>=0.55f && !treasureStolen){
            idol->hide();
            gIdolGlow->hide();
            gExplorer->showTreasure();
            treasureStolen=true;
            gGuardian->setStirring();             // statue senses the theft
        }
        // Drone swoops in close for the dramatic theft moment
        gCamera.setPosition(PLINTH_X+1.0f, lerpf(9.0f,5.0f,t), PLINTH_Z+lerpf(9.0f,5.0f,t));
        gCamera.setTarget(PLINTH_X, 1.0f, PLINTH_Z);
    }

    // ── Phase 4 (12-16s): The Pharaoh awakens in his sarcophagus ─────────
    if(time>=12.0f && time<16.0f){
        float t=(time-12.0f)/4.0f;
        gPharaoh->show();
        gPharaoh->setPosition(SARCO_X,0.0f,SARCO_Z);
        gPharaoh->setRotation('y',250.0f);
        gPharaoh->awakenAnim(time);
        if(t>0.3f) gGuardian->setAwakened();       // curse fully awakened
        pharaohAwake = true;
        // Drone rushes to the sarcophagus, low and dramatic
        gCamera.setPosition(SARCO_X+7.0f, lerpf(14.0f,5.0f,t), SARCO_Z+lerpf(10.0f,5.0f,t));
        gCamera.setTarget(SARCO_X,1.0f,SARCO_Z);
    }

    // ── Phase 5 (16-19s): Pharaoh rises and points accusingly ────────────
    if(time>=16.0f && time<19.0f){
        float t=(time-16.0f)/3.0f;
        gPharaoh->setPosition(SARCO_X,0.0f,SARCO_Z);
        gPharaoh->setRotation('y',250.0f);
        gPharaoh->pointAccuseAnim(time);
        gCamera.setPosition(SARCO_X+5.0f, lerpf(5.0f,9.0f,t), SARCO_Z+lerpf(5.0f,11.0f,t));
        gCamera.setTarget(SARCO_X,1.2f,SARCO_Z);
    }

    // ── Phase 6 (19-23s): The Mummy Warrior rises from the sand ──────────
    if(time>=19.0f && time<23.0f){
        float t=(time-19.0f)/4.0f;
        gMummy->show();
        gMummy->setPosition(MUMMY_BURY_X,0.0f,MUMMY_BURY_Z);
        gMummy->setRotation('y',300.0f);
        gMummy->animateRise(t);
        gMummy->animateArmL(time, t*40.0f);
        gMummy->animateArmR(time, t*40.0f);
        gMummy->setEyeGlow(0.4f + t*0.8f);
        if(t>=0.99f) mummyRisen=true;
        // Drone circles the rising mummy from a low dramatic angle
        float ang = lerpf(0.0f, 95.0f, t) * SC_PI/180.0f;
        gCamera.setPosition(MUMMY_BURY_X+sinf(ang)*9.0f, lerpf(3.0f,7.0f,t), MUMMY_BURY_Z+cosf(ang)*9.0f);
        gCamera.setTarget(MUMMY_BURY_X,1.0f,MUMMY_BURY_Z);
    }

    // ── Phase 7 (23-26s): Both stand and turn toward the fleeing thief ───
    if(time>=23.0f && time<26.0f){
        float t=(time-23.0f)/3.0f;
        gMummy->setPosition(MUMMY_BURY_X,0.0f,MUMMY_BURY_Z);
        gMummy->setRotation('y',lerpf(300.0f,210.0f,t));
        gMummy->animateWalk(time);
        gMummy->animateArmL(time,40.0f);
        gMummy->animateArmR(time,40.0f);

        gPharaoh->setPosition(SARCO_X,0.0f,SARCO_Z);
        gPharaoh->setRotation('y',lerpf(250.0f,210.0f,t));
        gPharaoh->pursueAnim(time);

        gExplorer->setPosition(PLINTH_X+2.2f,0.0f,PLINTH_Z+0.6f);
        gExplorer->setRotation('y',210.0f);
        gExplorer->animateLookBack(lerpf(0.0f,140.0f,t));
        gExplorer->stand();

        // Drone pulls wide, framing thief + both pursuers together
        gCamera.setPosition(PLINTH_X+4.0f, lerpf(8.0f,16.0f,t), PLINTH_Z+lerpf(8.0f,20.0f,t));
        gCamera.setTarget((PLINTH_X+MUMMY_BURY_X)*0.5f, 1.0f, (PLINTH_Z+MUMMY_BURY_Z)*0.5f);
    }

    // ── Phase 8 (26-36s): THE CHASE - explorer flees toward the pyramid ──
    if(time>=26.0f && time<36.0f){
        float elapsed=time-26.0f;
        float t=elapsed/10.0f;

        // Explorer curves onto the doorway's centreline (X=0) early, while
        // still well outside the pyramid's footprint (Z reaches -16.4 only
        // at t=1.0), so he never clips the pyramid's corner or walls.
        float exX=lerpf(PLINTH_X+2.0f, PYRAMID_X, t);
        float exZ=lerpf(PLINTH_Z, PYRAMID_Z+12.0f, t);
        gExplorer->setPosition(exX,0.0f,exZ);
        gExplorer->setRotation('y',195.0f);
        gExplorer->animateRun(time);
        gExplorer->animateArmL(time,1.0f);
        gExplorer->animateArmR(time,1.0f);
        gExplorer->animateLookBack(130.0f);

        float mumX=lerpf(MUMMY_BURY_X, PYRAMID_X-4.0f, t*0.92f);
        float mumZ=lerpf(MUMMY_BURY_Z, PYRAMID_Z+14.0f, t*0.92f);
        gMummy->setPosition(mumX,0.0f,mumZ);
        gMummy->setRotation('y',198.0f);
        gMummy->animateWalk(time*1.4f);
        gMummy->animateArmL(time,55.0f);
        gMummy->animateArmR(time,55.0f);

        float phX=lerpf(SARCO_X, PYRAMID_X+6.0f, t*0.85f);
        float phZ=lerpf(SARCO_Z, PYRAMID_Z+16.0f, t*0.85f);
        gPharaoh->setPosition(phX,0.0f,phZ);
        gPharaoh->setRotation('y',198.0f);
        gPharaoh->pursueAnim(time);

        // Classic drone chase shot: directly above-behind, angled forward
        gCamera.setPosition(exX, 22.0f, exZ+18.0f);
        gCamera.setTarget  (exX, 0.0f, exZ-12.0f);
    }

    // ── Phase 9 (36-42s): Closing in - the gap narrows near the entrance ─
    if(time>=36.0f && time<42.0f){
        float elapsed=time-36.0f;
        float t=elapsed/6.0f;

        // Stay centred on the doorway's X=0 centreline the whole way in -
        // this is the clear, obstacle-free corridor straight through the gap.
        float exX=PYRAMID_X;
        float exZ=lerpf(PYRAMID_Z+12.0f, PYRAMID_Z+5.6f, t);
        gExplorer->setPosition(exX,0.0f,exZ);
        gExplorer->setRotation('y',180.0f);
        gExplorer->animateRun(time);
        gExplorer->animateArmL(time,1.3f);
        gExplorer->animateArmR(time,1.3f);
        gExplorer->animateLookBack(150.0f);

        float mumX=lerpf(PYRAMID_X-4.0f, PYRAMID_X-0.6f, t);
        float mumZ=lerpf(PYRAMID_Z+14.0f, PYRAMID_Z+7.0f, t);
        gMummy->setPosition(mumX,0.0f,mumZ);
        gMummy->setRotation('y',186.0f);
        gMummy->animateWalk(time*1.5f);
        gMummy->animateArmL(time,65.0f);
        gMummy->animateArmR(time,65.0f);

        float phX=lerpf(PYRAMID_X+6.0f, PYRAMID_X+0.6f, t);
        float phZ=lerpf(PYRAMID_Z+16.0f, PYRAMID_Z+8.0f, t);
        gPharaoh->setPosition(phX,0.0f,phZ);
        gPharaoh->setRotation('y',186.0f);
        gPharaoh->pursueAnim(time);

        // Drone descends low and tight as the chase narrows
        float droneH=lerpf(22.0f,10.0f,t);
        gCamera.setPosition(exX+6.0f, droneH, exZ+10.0f);
        gCamera.setTarget  (exX, 1.0f, exZ-2.0f);
    }

    // ── Phase 10 (42-47s): Explorer vanishes into the pyramid doorway ────
    if(time>=42.0f && time<47.0f){
        float t=(time-42.0f)/5.0f;

        // Walk straight through the open doorway gap (X=0 centreline),
        // from just outside the front face (Z=-16.4) into the dark interior.
        float exZ=lerpf(PYRAMID_Z+5.6f, PYRAMID_Z+2.0f, t);
        gExplorer->setPosition(PYRAMID_X,0.0f,exZ);
        gExplorer->setRotation('y',180.0f);
        gExplorer->animateRun(time);
        gExplorer->animateArmL(time,1.4f);
        gExplorer->animateArmR(time,1.4f);
        if(t>0.75f) gExplorer->hide();             // swallowed by the darkness

        float mumZ=lerpf(PYRAMID_Z+7.0f, PYRAMID_Z+3.0f, t);
        gMummy->setPosition(PYRAMID_X-0.5f,0.0f,mumZ);
        gMummy->setRotation('y',180.0f);
        gMummy->animateWalk(time*1.5f);

        float phZ=lerpf(PYRAMID_Z+8.0f, PYRAMID_Z+3.6f, t);
        gPharaoh->setPosition(PYRAMID_X+0.5f,0.0f,phZ);
        gPharaoh->setRotation('y',180.0f);
        gPharaoh->pursueAnim(time);

        // Drone pushes in toward the dark doorway, then both pursuers follow
        gCamera.setPosition(PYRAMID_X+5.0f, lerpf(10.0f,4.0f,t), PYRAMID_Z+lerpf(10.0f,6.0f,t));
        gCamera.setTarget  (PYRAMID_X, 1.5f, PYRAMID_Z+1.0f);
        if(t>0.85f){ gMummy->hide(); gPharaoh->hide(); }
    }

    // ── Phase 11 (47-54s): Inside the pyramid - silence, then the curse's
    //    glow flares deep within as the chase continues unseen ───────────
    if(time>=47.0f && time<54.0f){
        float t=(time-47.0f)/7.0f;
        gGuardian->setAwakened();
        // Drone retreats, climbing high above the now-sealed pyramid
        float droneH=lerpf(4.0f,40.0f,t);
        float droneZ=lerpf(PYRAMID_Z+6.0f,PYRAMID_Z+50.0f,t);
        gCamera.setPosition(lerpf(PYRAMID_X+5.0f,0.0f,t),droneH,droneZ);
        gCamera.setTarget  (PYRAMID_X,0.0f,PYRAMID_Z);
    }

    // ── Phase 12 (54-66s): The curse's wrath - distant rumble glimpsed
    //    through a final wide drone shot of the whole necropolis ─────────
    if(time>=54.0f && time<66.0f){
        float t=(time-54.0f)/12.0f;
        float orbit = lerpf(0.0f, 140.0f, t) * SC_PI/180.0f;
        float r = 46.0f;
        gCamera.setPosition(PYRAMID_X+sinf(orbit)*r, 42.0f, PYRAMID_Z+30.0f+cosf(orbit)*r);
        gCamera.setTarget  (PYRAMID_X,4.0f,PYRAMID_Z);
        gGuardian->setAwakened();
    }

    // ── Phase 13 (66-78s): Slow fade - drone climbs to a final epic
    //    overview of the cursed pyramid under the night sky ───────────────
    if(time>=66.0f && time<78.0f){
        float t=(time-66.0f)/12.0f;
        float droneH=lerpf(42.0f,70.0f,t);
        float droneZ=lerpf(PYRAMID_Z+30.0f,PYRAMID_Z+90.0f,t);
        gCamera.setPosition(0.0f,droneH,droneZ);
        gCamera.setTarget  (PYRAMID_X,0.0f,PYRAMID_Z);
    }

    if(time>=78.0f) gProgramMode=kpmFinished;
}
