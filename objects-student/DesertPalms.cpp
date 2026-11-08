// DesertPalms.cpp
// Palm-tree billboards scattered around the oasis edge of the desert scene.
// (Renamed/re-themed from RoadsideTrees.)

#include "prefix.h"
#include "DesertPalms.h"
#include "utility.h"

DesertPalms::DesertPalms(int numPalms)
{
    palmCount   = numPalms;
    palmTexture = new texture("tree.tga", kContinuousTone, kMipmaps);
    palms.resize(palmCount);

    for (int i = 0; i < palmCount; i++)
    {
        palms[i] = new billboard(palmTexture, kCylindricalBillboarding);

        float x      = randomFloat(-26, 34);
        float z      = randomFloat(-12, 46);
        float height = randomFloat(1.4f, 3.6f);
        float width  = height * randomFloat(1.0f, 1.6f);

        palms[i]->setDeformation(width, height, width);
        palms[i]->setPosition(x, height - 1, z + 1);
    }
}

void DesertPalms::hide()
{
    for (int i = 0; i < palmCount; i++)
        palms[i]->hide();
}

void DesertPalms::show()
{
    for (int i = 0; i < palmCount; i++)
        palms[i]->show();
}
