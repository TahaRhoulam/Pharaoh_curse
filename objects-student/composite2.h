// composite2.h
// The Pharaoh's Curse - The Explorer
//
// Hierarchy:
//   composite2 (root)
//     torso      (cube, TEXTURED khaki shirt)
//       neck     (cylinder)
//         head   (sphere)
//           hair (sphere)
//           hat   (cube crown)
//             brim (SWEEP - hat_brim_profile along hat_brim_path)  [SWEEP]
//       armL     (cylinder)
//       armR     (cylinder, holds treasure)
//       legL     (cylinder)
//       legR     (cylinder)
//       satchel  (cube, EXTRUSION flap badge)                      [EXTRUSION]
//       treasure (tetra-like cube, golden idol carried)
//         glowAura (sphere, TRANSPARENT alpha 0.18)                [TRANSPARENT]
//
// Independently animatable children:
//   animateArmL(time)  - left arm flails while running
//   animateArmR(time)  - right arm flails independently
//   animateLookBack(angle) - head turns to look behind in fear
//   animateRun(time)   - full leg run-cycle + lean

#ifndef _COMPOSITE2_H
#define _COMPOSITE2_H

#include "object3d.h"
#include "cube.h"
#include "sphere.h"
#include "cylinder.h"
#include "sweep.h"
#include "extrusion.h"
#include "texture.h"

class composite2 : public object3d {
public:
    composite2();
    virtual void hide(void);
    virtual void show(void);

    void animateRun(float time);                // run cycle (legs + lean)
    void animateArmL(float time, float panic);   // independently animatable child #1
    void animateArmR(float time, float panic);   // independently animatable child #2
    void animateLookBack(float angle);           // look-behind in fear
    void stand();                                // idle pose
    void grabTreasurePose();                     // reach-down steal pose
    void showTreasure();
    void hideTreasure();

private:
    cube*      torso;          // TEXTURED khaki shirt
    cylinder*  neck;
    sphere*    head;
    sphere*    hair;
    cube*      hat;
    sweep*     brim;           // SWEEP fedora brim
    cylinder*  armL;
    cylinder*  armR;
    cylinder*  legL;
    cylinder*  legR;
    extrusion* satchelBadge;   // EXTRUSION leather satchel buckle
    cube*      satchel;
    cube*      treasure;       // golden idol
    sphere*    glowAura;       // TRANSPARENT magical glow

    texture*   shirtTexture;
};

#endif // _COMPOSITE2_H
