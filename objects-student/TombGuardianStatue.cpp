// TombGuardianStatue.cpp
// Jackal-headed Anubis guardian statue holding a staff topped with a
// glowing curse orb. A real GL point light sits inside the orb so it
// actually illuminates nearby geometry, matching its current state colour.

#include "prefix.h"
#include "TombGuardianStatue.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;

TombGuardianStatue::TombGuardianStatue()
{
    setName("TombGuardianStatue");

    // Stone pedestal
    pedestal = new cylinder();
    pedestal->setColour(0.42f, 0.38f, 0.30f);
    pedestal->setDeformation(0.55f, 0.45f, 0.55f);
    pedestal->setPosition(0.0f, 0.22f, 0.0f);
    pedestal->setParent(this);

    // Statue body (dark basalt)
    body = new cube();
    body->setColour(0.16f, 0.15f, 0.17f);
    body->setDeformation(0.42f, 1.30f, 0.30f);
    body->setPosition(0.0f, 1.55f, 0.0f);
    body->setParent(this);

    // Jackal head
    head = new cube();
    head->setColour(0.18f, 0.17f, 0.19f);
    head->setDeformation(0.26f, 0.34f, 0.42f);
    head->setPosition(0.0f, 1.45f, 0.0f);
    head->setParent(body);

    earL = new cube();
    earL->setColour(0.16f, 0.15f, 0.17f);
    earL->setDeformation(0.07f, 0.30f, 0.10f);
    earL->setPosition(-0.18f, 0.32f, -0.10f);
    earL->setRotation('z', -12.0f);
    earL->setParent(head);

    earR = new cube();
    earR->setColour(0.16f, 0.15f, 0.17f);
    earR->setDeformation(0.07f, 0.30f, 0.10f);
    earR->setPosition(0.18f, 0.32f, -0.10f);
    earR->setRotation('z', 12.0f);
    earR->setParent(head);

    // EXTRUSION staff shaft (requirement: extrusion used in supporting object)
    staff = new extrusion("staff_cross.txt", "staff_path.txt");
    staff->setColour(0.55f, 0.45f, 0.18f);
    staff->setDeformation(0.10f, 0.10f, 0.10f);
    staff->setPosition(0.50f, 0.40f, 0.0f);
    staff->setParent(body);

    // Glowing curse orb at the top of the staff
    curseOrb = new sphere(16);
    curseOrb->setDeformation(0.20f, 0.20f, 0.20f);
    curseOrb->setPosition(0.50f, 2.85f, 0.0f);
    curseOrb->setParent(this);

    // Real point light matching the orb's colour
    orbLight = new light(GL_LIGHT4);
    orbLight->setPosition(0.50f, 2.85f, 0.0f);
    orbLight->makePositional();
    orbLight->setLinearAttenuation(0.10f);
    orbLight->setParent(this);

    setDormant();
}

void TombGuardianStatue::setDormant()
{
    curseOrb->setColour(0.10f, 0.20f, 0.55f);
    curseOrb->setEmissiveColour(0.05f, 0.10f, 0.35f, 1.0f);
    orbLight->turnOn();
    orbLight->setColour(0.10f, 0.20f, 0.60f, 1.0f);
}

void TombGuardianStatue::setStirring()
{
    curseOrb->setColour(0.90f, 0.55f, 0.05f);
    curseOrb->setEmissiveColour(0.80f, 0.45f, 0.0f, 1.0f);
    orbLight->turnOn();
    orbLight->setColour(0.95f, 0.55f, 0.0f, 1.0f);
}

void TombGuardianStatue::setAwakened()
{
    curseOrb->setColour(1.00f, 0.08f, 0.05f);
    curseOrb->setEmissiveColour(0.95f, 0.05f, 0.0f, 1.0f);
    orbLight->turnOn();
    orbLight->setColour(1.0f, 0.0f, 0.0f, 1.0f);
}

void TombGuardianStatue::hide()
{
    pedestal->hide(); body->hide(); head->hide(); earL->hide(); earR->hide();
    staff->hide(); curseOrb->hide();
}

void TombGuardianStatue::show()
{
    pedestal->show(); body->show(); head->show(); earL->show(); earR->show();
    staff->show(); curseOrb->show();
}
