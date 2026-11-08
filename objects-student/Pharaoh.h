// Pharaoh.h
// The ancient Pharaoh, awakened by the theft of his sacred treasure.
// Rises from his sarcophagus, furious, and joins the Mummy Warrior in
// pursuing the explorer through the pyramid.
// (Renamed/re-themed from Customer.)

#ifndef _PHARAOH_H
#define _PHARAOH_H

#include "object3d.h"
#include "sphere.h"
#include "cube.h"
#include "cylinder.h"

class Pharaoh : public object3d {
public:
    Pharaoh();
    virtual void hide(void);
    virtual void show(void);

    void awakenAnim(float time);      // stirs to life, arms tremble
    void pointAccuseAnim(float time); // points furiously at the thief
    void pursueAnim(float time);      // strides forward in pursuit

private:
    cube*     torso;
    sphere*   head;
    cube*     crown;       // double crown of Egypt
    cylinder* neck;
    cylinder* armLeft;
    cylinder* armRight;
    cylinder* legLeft;
    cylinder* legRight;
    cube*     collar;      // golden ceremonial collar
};
#endif
