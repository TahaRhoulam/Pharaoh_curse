// composite1.cpp
// The Pharaoh's Curse - Mummy Warrior
// Requirements satisfied: TEXTURE (body + head bandage wrap), SWEEP (both arms,
// using bandage_profile.txt along mummy_arm_path.txt), EXTRUSION (chest cartouche
// hieroglyph panel, using cartouche_profile.txt / cartouche_path.txt), TRANSPARENT
// object (linen cape, alpha 0.35), >=2 independently animatable children
// (animateArmL / animateArmR / animateHeadTurn, all separate from animateWalk).

#include "prefix.h"
#include "composite1.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;

composite1::composite1()
{
    setName("composite1");

    // TEXTURE for the wrapped-linen body (requirement: textured object #1)
    bandageTex = new texture("vvv.bmp");
    headTex    = new texture("vvv.bmp");

    // Torso – ragged bandage-wrapped chest
    body = new cube();
    body->setColour(0.80f, 0.70f, 0.52f);
    body->setDeformation(0.62f, 1.05f, 0.46f);
    body->setTexture(bandageTex);
    body->setPosition(0.0f, 0.0f, 0.0f);
    body->setParent(this);

    neck = new cylinder();
    neck->setColour(0.78f, 0.66f, 0.48f);
    neck->setDeformation(0.11f, 0.20f, 0.11f);
    neck->setPosition(0.0f, 0.78f, 0.0f);
    neck->setParent(body);

    // Head – wrapped in bandage texture
    head = new sphere(20);
    head->setColour(0.82f, 0.72f, 0.55f);
    head->setDeformation(0.42f, 0.46f, 0.42f);
    head->setTexture(headTex);
    head->setPosition(0.0f, 0.46f, 0.0f);
    head->setParent(neck);

    // Glowing curse-orange eyes (emissive)
    eyeL = new sphere(10);
    eyeL->setColour(1.0f, 0.45f, 0.0f);
    eyeL->setEmissiveColour(0.9f, 0.35f, 0.0f, 1.0f);
    eyeL->setDeformation(0.07f, 0.07f, 0.07f);
    eyeL->setPosition(-0.16f, 0.06f, 0.38f);
    eyeL->setParent(head);

    eyeR = new sphere(10);
    eyeR->setColour(1.0f, 0.45f, 0.0f);
    eyeR->setEmissiveColour(0.9f, 0.35f, 0.0f, 1.0f);
    eyeR->setDeformation(0.07f, 0.07f, 0.07f);
    eyeR->setPosition(0.16f, 0.06f, 0.38f);
    eyeR->setParent(head);

    // Golden nemes headdress (crown + side panels) – Pharaoh / royal mummy motif
    crown = new cube();
    crown->setColour(0.85f, 0.68f, 0.10f);
    crown->setDeformation(0.46f, 0.20f, 0.46f);
    crown->setPosition(0.0f, 0.62f, 0.0f);
    crown->setParent(head);

    headdressL = new cube();
    headdressL->setColour(0.80f, 0.62f, 0.08f);
    headdressL->setDeformation(0.10f, 0.55f, 0.20f);
    headdressL->setPosition(-0.42f, -0.10f, 0.05f);
    headdressL->setParent(head);

    headdressR = new cube();
    headdressR->setColour(0.80f, 0.62f, 0.08f);
    headdressR->setDeformation(0.10f, 0.55f, 0.20f);
    headdressR->setPosition(0.42f, -0.10f, 0.05f);
    headdressR->setParent(head);

    // SWEEP arms – bandage-wrapped (requirement: at least one sweep across composites)
    armL = new sweep("bandage_profile.txt", 10);
    armL->setColour(0.78f, 0.68f, 0.50f);
    armL->setDeformation(0.40f, 0.40f, 0.40f);
    armL->setPosition(-0.62f, 0.40f, 0.0f);
    armL->setRotation('z', -10.0f);
    armL->setParent(body);

    armR = new sweep("bandage_profile.txt", 10);
    armR->setColour(0.78f, 0.68f, 0.50f);
    armR->setDeformation(0.40f, 0.40f, 0.40f);
    armR->setPosition(0.62f, 0.40f, 0.0f);
    armR->setRotation('z', 10.0f);
    armR->setParent(body);

    legL = new cylinder();
    legL->setColour(0.74f, 0.64f, 0.46f);
    legL->setDeformation(0.16f, 0.85f, 0.16f);
    legL->setPosition(-0.20f, -1.20f, 0.0f);
    legL->setParent(body);

    legR = new cylinder();
    legR->setColour(0.74f, 0.64f, 0.46f);
    legR->setDeformation(0.16f, 0.85f, 0.16f);
    legR->setPosition(0.20f, -1.20f, 0.0f);
    legR->setParent(body);

    // EXTRUSION chest cartouche (requirement: at least one extrusion across composites)
    chest = new extrusion("cartouche_profile.txt", "cartouche_path.txt");
    chest->setColour(0.85f, 0.70f, 0.12f);          // gold hieroglyph plate
    chest->setDeformation(0.34f, 0.34f, 0.34f);
    chest->setPosition(0.0f, 0.30f, 0.40f);
    chest->setParent(body);

    // TRANSPARENT linen cape (requirement: transparent object)
    cape = new cube();
    cape->setColour(0.70f, 0.58f, 0.40f, 0.35f);    // alpha 0.35 -> see-through
    cape->setDeformation(0.55f, 1.00f, 0.05f);
    cape->setPosition(0.0f, 0.10f, -0.45f);
    cape->setParent(body);
}

void composite1::animateRise(float t)
{
    // Independently animatable: the whole mummy rising out of the sand.
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    float x, y, z;
    getPosition(x, y, z);
    setPosition(x, -1.6f * (1.0f - t), z);
}

void composite1::animateWalk(float time)
{
    float p = time * 3.4f;
    legL->setRotation('x',  sinf(p) * 22.0f);
    legR->setRotation('x', -sinf(p) * 22.0f);
    body->setRotation('z', sinf(p) * 3.0f);          // gentle lurching sway
}

void composite1::animateArmL(float time, float reach)
{
    // Independently animatable child #1: left arm raises/swings toward target reach.
    float swing = sinf(time * 2.0f) * 8.0f;
    armL->setRotation('z', -10.0f - reach, 'x', swing);
}

void composite1::animateArmR(float time, float reach)
{
    // Independently animatable child #2: right arm raises/swings independently.
    float swing = sinf(time * 2.0f + 3.14159f) * 8.0f;
    armR->setRotation('z', 10.0f + reach, 'x', swing);
}

void composite1::animateHeadTurn(float angle)
{
    head->setRotation('y', angle);
}

void composite1::setEyeGlow(float intensity)
{
    eyeL->setEmissiveColour(0.9f * intensity, 0.35f * intensity, 0.0f, 1.0f);
    eyeR->setEmissiveColour(0.9f * intensity, 0.35f * intensity, 0.0f, 1.0f);
}

void composite1::hide()
{
    body->hide(); neck->hide(); head->hide(); eyeL->hide(); eyeR->hide();
    crown->hide(); headdressL->hide(); headdressR->hide();
    armL->hide(); armR->hide(); legL->hide(); legR->hide();
    chest->hide(); cape->hide();
}

void composite1::show()
{
    body->show(); neck->show(); head->show(); eyeL->show(); eyeR->show();
    crown->show(); headdressL->show(); headdressR->show();
    armL->show(); armR->show(); legL->show(); legR->show();
    chest->show(); cape->show();
}
