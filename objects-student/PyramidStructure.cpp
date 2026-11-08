// PyramidStructure.cpp
// A stepped stone pyramid, built from four stacked, shrinking blocks
// plus a golden capstone and carved glyph panels.
//
// The base tier's south face (+Z) is built as two side pillars plus a
// lintel above a genuine open gap, instead of one solid block with a
// painted-on door panel. This means there is an actual walkable doorway
// opening rather than a flat wall blocking movement.

#include "prefix.h"
#include "PyramidStructure.h"
#include "utility.h"

extern vector<object3d*> gShapeVector;

PyramidStructure::PyramidStructure(float colR, float colG, float colB, float h)
{
    setName("PyramidStructure");

    float bH = 5.0f * h;     // overall pyramid height
    float baseH = bH * 0.4f; // half-height of the base tier
    float baseY = bH * 0.2f; // centre Y of the base tier

    // Doorway opening dimensions (world units, relative to pyramid centre)
    float doorHalfWidth = 1.3f;   // gap half-width on X
    float doorHalfDepth = 0.9f;   // how far the opening cuts in on Z
    float doorTopY      = 2.6f;   // doorway opening height (from ground, Y=0)

    // ── Base tier, split around a real doorway gap on the +Z (south) face ──
    // Left pillar: spans from the gap's left edge out to the base's left edge
    float leftPillarHalfW = (5.6f - doorHalfWidth) * 0.5f;
    baseLeft = new cube();
    baseLeft->setColour(colR, colG, colB);
    baseLeft->setDeformation(leftPillarHalfW, baseH, 5.6f);
    baseLeft->setPosition(-(doorHalfWidth + leftPillarHalfW), baseY, 0.0f);
    baseLeft->setParent(this);

    // Right pillar: mirror of the left pillar
    baseRight = new cube();
    baseRight->setColour(colR, colG, colB);
    baseRight->setDeformation(leftPillarHalfW, baseH, 5.6f);
    baseRight->setPosition((doorHalfWidth + leftPillarHalfW), baseY, 0.0f);
    baseRight->setParent(this);

    // Back block: fills in behind the doorway gap so the pyramid still
    // reads as a solid mass from every other angle, but stops short of the
    // +Z face within the doorway's X range, leaving a clear, walkable recess.
    float doorRecessDepth = doorHalfDepth * 2.0f;       // how deep the open recess cuts in from the face
    float backHalfDepth   = (11.2f - doorRecessDepth) * 0.5f;
    float backCenterZ     = 5.6f - doorRecessDepth - backHalfDepth;
    baseBack = new cube();
    baseBack->setColour(colR, colG, colB);
    baseBack->setDeformation(doorHalfWidth, baseH, backHalfDepth);
    baseBack->setPosition(0.0f, baseY, backCenterZ);
    baseBack->setParent(this);

    // Lintel: stone slab spanning the doorway gap, above the opening height,
    // up to the top of the base tier, so the base silhouette stays intact.
    float lintelHalfH = (baseH * 2.0f - doorTopY) * 0.5f;
    lintelHalfH = (lintelHalfH > 0.05f) ? lintelHalfH : 0.05f;
    float lintelY = doorTopY + lintelHalfH;
    lintel = new cube();
    lintel->setColour(colR * 0.95f, colG * 0.95f, colB * 0.95f);
    lintel->setDeformation(doorHalfWidth, lintelHalfH, 5.6f);
    lintel->setPosition(0.0f, lintelY, 0.0f);
    lintel->setParent(this);

    // Mid tier (narrower)
    midBlock = new cube();
    midBlock->setColour(colR * 0.92f, colG * 0.92f, colB * 0.92f);
    midBlock->setDeformation(3.9f, bH * 0.35f, 3.9f);
    midBlock->setPosition(0.0f, bH * 0.575f, 0.0f);
    midBlock->setParent(this);

    // Top tier (narrowest)
    topBlock = new cube();
    topBlock->setColour(colR * 0.84f, colG * 0.84f, colB * 0.84f);
    topBlock->setDeformation(2.2f, bH * 0.25f, 2.2f);
    topBlock->setPosition(0.0f, bH * 0.875f, 0.0f);
    topBlock->setParent(this);

    // Golden capstone
    capstone = new cube();
    capstone->setColour(0.85f, 0.70f, 0.12f);
    capstone->setEmissiveColour(0.12f, 0.09f, 0.0f, 1.0f);
    capstone->setDeformation(0.55f, bH * 0.10f, 0.55f);
    capstone->setPosition(0.0f, bH + 0.05f, 0.0f);
    capstone->setParent(this);

    // Dark recessed doorway interior - sits inside the open gap so the
    // opening reads as a dark passage rather than empty space.
    doorway = new cube();
    doorway->setColour(0.06f, 0.05f, 0.04f);
    doorway->setDeformation(doorHalfWidth * 0.92f, doorTopY * 0.5f, doorHalfDepth * 0.85f);
    doorway->setPosition(0.0f, doorTopY * 0.5f, 5.6f - doorHalfDepth * 0.85f);
    doorway->setParent(this);

    // Carved stone door-frame trim either side of the opening
    doorFrameL = new cube();
    doorFrameL->setColour(colR * 0.80f, colG * 0.80f, colB * 0.80f);
    doorFrameL->setDeformation(0.18f, doorTopY * 0.5f, doorHalfDepth);
    doorFrameL->setPosition(-doorHalfWidth - 0.18f, doorTopY * 0.5f, 5.6f - doorHalfDepth);
    doorFrameL->setParent(this);

    doorFrameR = new cube();
    doorFrameR->setColour(colR * 0.80f, colG * 0.80f, colB * 0.80f);
    doorFrameR->setDeformation(0.18f, doorTopY * 0.5f, doorHalfDepth);
    doorFrameR->setPosition(doorHalfWidth + 0.18f, doorTopY * 0.5f, 5.6f - doorHalfDepth);
    doorFrameR->setParent(this);

    // Carved hieroglyph panels (glowing faint gold) either side of the doorway
    glyphA1 = new cube();
    glyphA1->setColour(0.70f, 0.58f, 0.20f, 0.85f);
    glyphA1->setDeformation(0.45f, 0.70f, 0.06f);
    glyphA1->setPosition(-1.7f, bH * 0.30f, 5.62f);
    glyphA1->setParent(this);

    glyphA2 = new cube();
    glyphA2->setColour(0.70f, 0.58f, 0.20f, 0.85f);
    glyphA2->setDeformation(0.45f, 0.70f, 0.06f);
    glyphA2->setPosition(1.7f, bH * 0.30f, 5.62f);
    glyphA2->setParent(this);

    glyphB1 = new cube();
    glyphB1->setColour(0.70f, 0.58f, 0.20f, 0.85f);
    glyphB1->setDeformation(0.45f, 0.70f, 0.06f);
    glyphB1->setPosition(-1.7f, bH * 0.55f, 4.8f);
    glyphB1->setParent(this);

    glyphB2 = new cube();
    glyphB2->setColour(0.70f, 0.58f, 0.20f, 0.85f);
    glyphB2->setDeformation(0.45f, 0.70f, 0.06f);
    glyphB2->setPosition(1.7f, bH * 0.55f, 4.8f);
    glyphB2->setParent(this);
}

void PyramidStructure::hide()
{
    baseLeft->hide(); baseRight->hide(); baseBack->hide(); lintel->hide();
    midBlock->hide(); topBlock->hide(); capstone->hide();
    doorway->hide(); doorFrameL->hide(); doorFrameR->hide();
    glyphA1->hide(); glyphA2->hide(); glyphB1->hide(); glyphB2->hide();
}

void PyramidStructure::show()
{
    baseLeft->show(); baseRight->show(); baseBack->show(); lintel->show();
    midBlock->show(); topBlock->show(); capstone->show();
    doorway->show(); doorFrameL->show(); doorFrameR->show();
    glyphA1->show(); glyphA2->show(); glyphB1->show(); glyphB2->show();
}
