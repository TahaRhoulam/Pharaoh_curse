// PyramidStructure.h
// A great stone pyramid building block, used to construct the necropolis
// skyline and the final pyramid the explorer is chased into.
// (Renamed/re-themed from CityBuilding.)
//
// The base tier is built from two side pillars plus a lintel above a real
// open gap (rather than one solid cube with a painted-on door panel), so
// the doorway is an actual opening characters can walk through.

#ifndef _PYRAMIDSTRUCTURE_H
#define _PYRAMIDSTRUCTURE_H

#include "object3d.h"
#include "cube.h"

class PyramidStructure : public object3d {
public:
    // colR/G/B = stone colour,  h = relative height scale
    PyramidStructure(float colR, float colG, float colB, float h);
    virtual void hide(void);
    virtual void show(void);

private:
    cube* baseLeft;     // base tier, left of the doorway gap
    cube* baseRight;     // base tier, right of the doorway gap
    cube* baseBack;       // base tier, solid block behind/away from the doorway side
    cube* lintel;          // stone slab above the doorway opening
    cube* midBlock;
    cube* topBlock;
    cube* capstone;
    cube* doorway;         // dark recessed opening (no longer blocks movement)
    cube* doorFrameL; cube* doorFrameR;   // carved door-frame trim either side
    cube* glyphA1; cube* glyphA2;
    cube* glyphB1; cube* glyphB2;
};
#endif

