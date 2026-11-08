// TombGuardianStatue.h
// A carved Anubis guardian statue with a glowing curse-orb in its staff.
// The orb's colour signals the state of the curse: dormant (dim blue),
// stirring (amber), or fully awakened (blazing red) - functionally
// equivalent to a traffic light's red/yellow/green state machine, themed
// for the pyramid setting.
// (Renamed/re-themed from TrafficLight.)

#ifndef _TOMBGUARDIANSTATUE_H
#define _TOMBGUARDIANSTATUE_H

#include "object3d.h"
#include "cube.h"
#include "cylinder.h"
#include "sphere.h"
#include "extrusion.h"
#include "light.h"

class TombGuardianStatue : public object3d {
public:
    TombGuardianStatue();
    virtual void hide(void);
    virtual void show(void);

    void setDormant();    // curse asleep   - dim blue orb
    void setStirring();   // curse stirring - amber orb
    void setAwakened();   // curse awakened - blazing red orb

private:
    cylinder*  pedestal;
    cube*      body;
    cube*      head;
    cube*      earL;
    cube*      earR;
    extrusion* staff;        // EXTRUSION staff shaft
    sphere*    curseOrb;     // glowing orb atop the staff
    light*     orbLight;     // real point light matching the orb colour
};
#endif
