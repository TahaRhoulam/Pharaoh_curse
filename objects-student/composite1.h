// composite1.h
// The Pharaoh's Curse - Mummy Warrior
//
// Hierarchy (13 children attached to root or body):
//   composite1 (root)
//     body      (cube torso, textured with bandage wrap)
//       head    (sphere, textured)
//         eyeL  (sphere, glowing orange - emissive)
//         eyeR  (sphere, glowing orange - emissive)
//       neck    (cylinder)
//       armL    (sweep - bandage_profile along mummy_arm_path)  [SWEEP]
//       armR    (sweep - bandage_profile along mummy_arm_path)  [SWEEP]
//       legL    (cylinder)
//       legR    (cylinder)
//       chest   (extrusion - cartouche hieroglyph panel)        [EXTRUSION]
//       cape    (cube, TRANSPARENT alpha=0.35)                  [TRANSPARENT]
//     crown     (cube, golden headdress)
//     headdressL(cube, side panel)
//     headdressR(cube, side panel)
//
// Independently animatable children:
//   animateArmL(time)  - raises/swings left arm
//   animateArmR(time)  - raises/swings right arm independently
//   animateHeadTurn(angle) - rotates head
//   animateRise(t)     - mummy emerges from sand
//   animateWalk(time)  - walk cycle for legs

#ifndef _COMPOSITE1_H
#define _COMPOSITE1_H

#include "object3d.h"
#include "cube.h"
#include "sphere.h"
#include "cylinder.h"
#include "sweep.h"
#include "extrusion.h"
#include "texture.h"

class composite1 : public object3d {
public:
    composite1();
    virtual void hide(void);
    virtual void show(void);

    // Animation methods (all child animation done only through these)
    void animateRise(float t);                 // t 0..1: emerge from sand
    void animateWalk(float time);              // leg swing + body sway
    void animateArmL(float time, float reach); // left arm – independently animatable child #1
    void animateArmR(float time, float reach); // right arm – independently animatable child #2
    void animateHeadTurn(float angle);         // head faces target
    void setEyeGlow(float intensity);         // glow more brightly when angry

private:
    cube*      body;           // torso (textured – bandage wrap)
    cylinder*  neck;
    sphere*    head;           // textured
    sphere*    eyeL;           // orange emissive glow
    sphere*    eyeR;           // orange emissive glow
    sweep*     armL;           // SWEEP bandage-wrapped arm
    sweep*     armR;           // SWEEP bandage-wrapped arm
    cylinder*  legL;
    cylinder*  legR;
    extrusion* chest;          // EXTRUSION hieroglyph cartouche
    cube*      cape;           // TRANSPARENT linen cape
    cube*      crown;          // golden nemes headdress top
    cube*      headdressL;     // nemes left panel
    cube*      headdressR;     // nemes right panel

    texture*   bandageTex;     // wrapped linen texture on body
    texture*   headTex;        // head bandage texture
};

#endif // _COMPOSITE1_H
