// Pharaoh.cpp - the ancient ruler who is awakened by the theft.

#include "prefix.h"
#include "Pharaoh.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;

Pharaoh::Pharaoh()
{
    setName("Pharaoh");

    // Royal robes – deep blue and gold
    torso = new cube();
    torso->setColour(0.16f, 0.22f, 0.55f);
    torso->setDeformation(0.74f, 1.08f, 0.50f);
    torso->setPosition(0.0f, 0.0f, 0.0f);
    torso->setParent(this);

    // Golden ceremonial collar
    collar = new cube();
    collar->setColour(0.85f, 0.68f, 0.10f);
    collar->setDeformation(0.78f, 0.16f, 0.52f);
    collar->setPosition(0.0f, 0.86f, 0.0f);
    collar->setParent(torso);

    neck = new cylinder();
    neck->setColour(0.78f, 0.62f, 0.45f);
    neck->setDeformation(0.10f, 0.20f, 0.10f);
    neck->setPosition(0.0f, 0.80f, 0.0f);
    neck->setParent(torso);

    head = new sphere(22);
    head->setColour(0.78f, 0.62f, 0.45f);
    head->setDeformation(0.44f, 0.48f, 0.44f);
    head->setPosition(0.0f, 0.50f, 0.0f);
    head->setParent(neck);

    // Double crown of Egypt (Pschent)
    crown = new cube();
    crown->setColour(0.82f, 0.66f, 0.08f);
    crown->setDeformation(0.40f, 0.55f, 0.40f);
    crown->setPosition(0.0f, 0.55f, 0.0f);
    crown->setParent(head);

    armLeft = new cylinder();
    armLeft->setColour(0.16f, 0.22f, 0.55f);
    armLeft->setDeformation(0.13f, 0.70f, 0.13f);
    armLeft->setPosition(-0.62f, 0.18f, 0.0f);
    armLeft->setRotation('z', -16.0f);
    armLeft->setParent(torso);

    armRight = new cylinder();
    armRight->setColour(0.16f, 0.22f, 0.55f);
    armRight->setDeformation(0.13f, 0.70f, 0.13f);
    armRight->setPosition(0.62f, 0.18f, 0.0f);
    armRight->setRotation('z', 16.0f);
    armRight->setParent(torso);

    legLeft = new cylinder();
    legLeft->setColour(0.74f, 0.60f, 0.18f);
    legLeft->setDeformation(0.18f, 0.90f, 0.18f);
    legLeft->setPosition(-0.22f, -1.28f, 0.0f);
    legLeft->setParent(torso);

    legRight = new cylinder();
    legRight->setColour(0.74f, 0.60f, 0.18f);
    legRight->setDeformation(0.18f, 0.90f, 0.18f);
    legRight->setPosition(0.22f, -1.28f, 0.0f);
    legRight->setParent(torso);
}

void Pharaoh::awakenAnim(float time)
{
    // Trembling stir as the ancient body comes back to life
    float tremor = sinf(time * 14.0f) * 6.0f;
    armLeft->setRotation ('x', tremor, 'z', -16.0f);
    armRight->setRotation('x', -tremor, 'z', 16.0f);
    head->setRotation('z', sinf(time * 6.0f) * 4.0f);
}

void Pharaoh::pointAccuseAnim(float time)
{
    // Furious accusing point toward the thief, the other arm raised in rage
    armRight->setRotation('x', -80.0f, 'z', 4.0f);
    float shake = sinf(time * 9.0f) * 5.0f;
    armLeft->setRotation('x', -40.0f + shake, 'z', -30.0f);
}

void Pharaoh::pursueAnim(float time)
{
    float p = time * 3.0f;
    legLeft->setRotation ('x', -sinf(p) * 18.0f);
    legRight->setRotation('x',  sinf(p) * 18.0f);
    armLeft->setRotation ('x',  sinf(p) * 14.0f, 'z', -16.0f);
    armRight->setRotation('x', -sinf(p) * 14.0f, 'z',  16.0f);
}

void Pharaoh::hide()
{
    torso->hide(); collar->hide(); neck->hide(); head->hide(); crown->hide();
    armLeft->hide(); armRight->hide(); legLeft->hide(); legRight->hide();
}

void Pharaoh::show()
{
    torso->show(); collar->show(); neck->show(); head->show(); crown->show();
    armLeft->show(); armRight->show(); legLeft->show(); legRight->show();
}
