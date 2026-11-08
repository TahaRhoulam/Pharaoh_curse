// DesertPalms.h
// Billboard-based palm trees scattered around the desert/pyramid scene.
// (Renamed/re-themed from RoadsideTrees.)

#ifndef _DESERTPALMS_H
#define _DESERTPALMS_H

#include <vector>
using namespace std;

#include "object3d.h"
#include "billboard.h"
#include "texture.h"

class DesertPalms : public object3d
{
public:
    int                 palmCount;
    vector<billboard*>  palms;
    texture*            palmTexture;

                        DesertPalms(int palmCount);
    virtual void        hide(void);
    virtual void        show(void);
};

#endif // _DESERTPALMS_H
