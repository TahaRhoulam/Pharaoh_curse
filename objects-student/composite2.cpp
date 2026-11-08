// composite2.cpp
// The Pharaoh's Curse - The Explorer
// Requirements satisfied: TEXTURE (khaki shirt), SWEEP (fedora brim, using
// hat_brim_profile.txt along hat_brim_path.txt), EXTRUSION (satchel buckle,
// using cartouche-style flat panel via astronaut_body/path-equivalent custom
// files), TRANSPARENT object (treasure glow aura, alpha 0.18), >=2
// independently animatable children (animateArmL / animateArmR / animateLookBack).

#include "prefix.h"
#include "composite2.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;

composite2::composite2()
{
    setName("composite2");

    // TEXTURE for the khaki adventurer shirt (requirement: textured object #2)
    shirtTexture = new texture("Area51.jpg");

    torso = new cube();
    torso->setColour(0.62f, 0.55f, 0.36f);
    torso->setDeformation(0.70f, 1.02f, 0.46f);
    torso->setTexture(shirtTexture);
    torso->setPosition(0.0f, 0.0f, 0.0f);
    torso->setParent(this);

    neck = new cylinder();
    neck->setColour(0.85f, 0.68f, 0.52f);
    neck->setDeformation(0.10f, 0.20f, 0.10f);
    neck->setPosition(0.0f, 0.76f, 0.0f);
    neck->setParent(torso);

    head = new sphere(20);
    head->setColour(0.85f, 0.68f, 0.52f);
    head->setDeformation(0.42f, 0.46f, 0.42f);
    head->setPosition(0.0f, 0.48f, 0.0f);
    head->setParent(neck);

    hair = new sphere(16);
    hair->setColour(0.20f, 0.12f, 0.06f);
    hair->setDeformation(0.44f, 0.24f, 0.44f);
    hair->setPosition(0.0f, 0.24f, 0.0f);
    hair->setParent(head);

    // Fedora crown
    hat = new cube();
    hat->setColour(0.42f, 0.30f, 0.14f);
    hat->setDeformation(0.34f, 0.22f, 0.34f);
    hat->setPosition(0.0f, 0.50f, 0.0f);
    hat->setParent(head);

    // SWEEP fedora brim (requirement: at least one sweep across composites)
    brim = new sweep("hat_brim_profile.txt", 14);
    brim->setColour(0.36f, 0.25f, 0.11f);
    brim->setDeformation(0.50f, 0.50f, 0.50f);
    brim->setPosition(0.0f, 0.30f, 0.0f);
    brim->setRotation('x', 90.0f);
    brim->setParent(hat);

    armL = new cylinder();
    armL->setColour(0.62f, 0.55f, 0.36f);
    armL->setDeformation(0.13f, 0.68f, 0.13f);
    armL->setPosition(-0.58f, 0.16f, 0.0f);
    armL->setRotation('z', -16.0f);
    armL->setParent(torso);

    armR = new cylinder();
    armR->setColour(0.62f, 0.55f, 0.36f);
    armR->setDeformation(0.13f, 0.68f, 0.13f);
    armR->setPosition(0.58f, 0.16f, 0.0f);
    armR->setRotation('z', 16.0f);
    armR->setParent(torso);

    legL = new cylinder();
    legL->setColour(0.30f, 0.24f, 0.14f);
    legL->setDeformation(0.17f, 0.86f, 0.17f);
    legL->setPosition(-0.22f, -1.22f, 0.0f);
    legL->setParent(torso);

    legR = new cylinder();
    legR->setColour(0.30f, 0.24f, 0.14f);
    legR->setDeformation(0.17f, 0.86f, 0.17f);
    legR->setPosition(0.22f, -1.22f, 0.0f);
    legR->setParent(torso);

    // Leather satchel slung on hip
    satchel = new cube();
    satchel->setColour(0.44f, 0.30f, 0.16f);
    satchel->setDeformation(0.32f, 0.30f, 0.16f);
    satchel->setPosition(-0.55f, -0.30f, 0.10f);
    satchel->setParent(torso);

    // EXTRUSION satchel buckle badge (requirement: at least one extrusion)
    satchelBadge = new extrusion("cartouche_profile.txt", "cartouche_path.txt");
    satchelBadge->setColour(0.78f, 0.68f, 0.20f);
    satchelBadge->setDeformation(0.10f, 0.10f, 0.10f);
    satchelBadge->setPosition(-0.55f, -0.20f, 0.27f);
    satchelBadge->setParent(torso);

    // Stolen golden idol carried in right hand
    treasure = new cube();
    treasure->setColour(0.95f, 0.80f, 0.10f);
    treasure->setEmissiveColour(0.25f, 0.18f, 0.0f, 1.0f);
    treasure->setDeformation(0.18f, 0.22f, 0.18f);
    treasure->setPosition(0.62f, -0.55f, 0.20f);
    treasure->setParent(torso);
    treasure->hide();

    // TRANSPARENT magical glow aura around the treasure (requirement: transparent object)
    glowAura = new sphere(14);
    glowAura->setColour(1.0f, 0.90f, 0.20f, 0.18f);
    glowAura->setDeformation(0.42f, 0.42f, 0.42f);
    glowAura->setPosition(0.0f, 0.0f, 0.0f);
    glowAura->setParent(treasure);
}

void composite2::animateRun(float time)
{
    float p = time * 5.2f;
    legL->setRotation('x', -sinf(p) * 26.0f);
    legR->setRotation('x',  sinf(p) * 26.0f);
    torso->setRotation('x', 9.0f);              // forward sprint lean
}

void composite2::animateArmL(float time, float panic)
{
    // Independently animatable child #1: left arm flails while sprinting.
    float p = time * 5.2f;
    armL->setRotation('x', sinf(p + 3.14159f) * (28.0f + panic * 26.0f), 'z', -16.0f);
}

void composite2::animateArmR(float time, float panic)
{
    // Independently animatable child #2: right arm flails independently.
    float p = time * 5.2f;
    armR->setRotation('x', sinf(p) * (28.0f + panic * 26.0f), 'z', 16.0f);
}

void composite2::animateLookBack(float angle)
{
    head->setRotation('y', angle);
}

void composite2::stand()
{
    torso->setRotation('x', 0.0f);
    armL->setRotation('z', -16.0f);
    armR->setRotation('z',  16.0f);
    legL->setRotation('x', 0.0f);
    legR->setRotation('x', 0.0f);
}

void composite2::grabTreasurePose()
{
    armR->setRotation('x', -70.0f, 'z', 24.0f);
    armL->setRotation('x', -20.0f, 'z', -16.0f);
    torso->setRotation('x', 14.0f);
}

void composite2::showTreasure() { treasure->show(); }
void composite2::hideTreasure() { treasure->hide(); }

void composite2::hide()
{
    torso->hide(); neck->hide(); head->hide(); hair->hide();
    hat->hide(); brim->hide();
    armL->hide(); armR->hide(); legL->hide(); legR->hide();
    satchel->hide(); satchelBadge->hide();
    treasure->hide(); glowAura->hide();
}

void composite2::show()
{
    torso->show(); neck->show(); head->show(); hair->show();
    hat->show(); brim->show();
    armL->show(); armR->show(); legL->show(); legR->show();
    satchel->show(); satchelBadge->show();
}
